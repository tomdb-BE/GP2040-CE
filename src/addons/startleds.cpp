/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2022 Tom De Backer
 */

// Pico Includes
#include <vector>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "GamepadEnums.h"
#include "storagemanager.h"

// GP2040 Includes
#include "addons/startleds.h"
#include "helper.h"


void StartLEDs::setup(const int * pinConfig, bool iState, uint64_t mBrightness, uint64_t mLevel)
{
	this->ledPins = pinConfig;		
	this->maxBrightness = mBrightness;
	this->brightness = mBrightness;
	this->maxLevel = mLevel;
	this->initialState = iState;

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 4.f);

	std::vector<uint> sliceNums;

	for (int i = 0; i < STARTLED_COUNT; i++)
	{
		if (pinConfig[i] > -1)
		{
			gpio_set_function(pinConfig[i], GPIO_FUNC_PWM);
			uint sliceNum = pwm_gpio_to_slice_num(pinConfig[i]);
			uint channelNum = pwm_gpio_to_channel(pinConfig[i]);
			sliceNums.push_back(sliceNum);
			pwm_set_chan_level(sliceNum, channelNum, mLevel);
		}	
	}	

	for (auto sliceNum : sliceNums)
		pwm_set_enabled(sliceNum, true);

}

void StartLEDs::display()
{
	for (int i = 0; i < STARTLED_COUNT; i++)
		if (this->ledPins[i] > -1)
			pwm_set_gpio_level(this->ledPins[i], this->ledLevels[i]);
}

void StartLEDs::animate(StartLEDAnimationState animationState)
{
	// Reset state and bypass timer check if animation changed
	if (animationState.animation != this->selectedAnimation)
	{
		this->reset();
		this->selectedAnimation = animationState.animation;
	}
	else if (!time_reached(this->nextAnimationTime))
	{
		return;
	}

	this->parseState(animationState.state);	

	switch (this->selectedAnimation)
	{
		case STARTLED_ANIM_BLINK:
			this->handleBlink(animationState.speed);
			break;

		case STARTLED_ANIM_FADE:
			this->handleFade();
			break;

		case STARTLED_ANIM_OFF:
			for (int i = 0; i < STARTLED_COUNT; i++)
				this->currentLedState[i] = 1;	
			this->brightness = this->maxBrightness;
			break;

		case STARTLED_ANIM_SOLID:
			for (int i = 0; i < STARTLED_COUNT; i++)
				this->currentLedState[i] = 1;		
			this->brightness = 0;
			break;						

		default:
			break;
	}

	for (int i = 0; i < STARTLED_COUNT; i++)
		if (this->ledPins[i] > -1)
			this->ledLevels[i] = this->maxLevel - (this->currentLedState[i] ? (this->brightness * this->brightness) : 0);
}

bool StartLEDAddon::available() {
	return STARTLED_TYPE != STARTLED_TYPE_NONE;
}

void StartLEDAddon::setup() {
	switch (STARTLED_TYPE)
	{
		case STARTLED_TYPE_PWM:
			this->ledsStart = new StartLEDs();
			this->ledsCoin = new StartLEDs();
			break;
	}

	if (ledsStart != nullptr)
		this->ledsStart->setup(STARTLED_START_PINS, false, STARTLED_MAX_START_BRIGHTNESS, STARTLED_MAX_START_LEVEL);	
	if (ledsCoin != nullptr)
		this->ledsCoin->setup(STARTLED_COIN_PINS, true, STARTLED_MAX_COIN_BRIGHTNESS, STARTLED_MAX_COIN_LEVEL);
	
	this->animationStateStart.animation = STARTLED_ANIM_OFF;
	this->animationStateCoin.animation = STARTLED_ANIM_SOLID;
	this->nextButtonCheckTime = make_timeout_time_ms(STARTLED_DELAY_START);
}

void StartLEDAddon::process()
{

	if (!time_reached(this->nextButtonCheckTime)) return;

    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	uint16_t start1Pressed = gamepad->state.buttons & START1_BUTTON_MASK;
	uint16_t coin1Pressed = gamepad->state.buttons & COIN1_BUTTON_MASK;

	this->SetAnimationStart(STARTLED_STATE_LED2, STARTLED_ANIM_BLINK, STARTLED_SPEED_FAST);

	if (this->ledsStart != nullptr)
		this->ledsStart->display();
	if (this->ledsCoin != nullptr)
		this->ledsCoin->display();

	if (coin1Pressed) {	
		this->lastCoinPressed[0] = true;
	}	
	else if (this->lastCoinPressed[0]) {
		this->SetAnimationCoin(STARTLED_STATE_LED1, STARTLED_ANIM_SOLID);
		this->lastCoinPressed[0] = false;
		if (this->creditCount == 0)
			this->SetAnimationStart(STARTLED_STATE_LED1, STARTLED_ANIM_BLINK, STARTLED_SPEED_FAST);
		else if (this->animationStateStart.animation != STARTLED_ANIM_BLINK)
			this->SetAnimationStart(STARTLED_STATE_LED1, STARTLED_ANIM_SOLID);
		if (this->creditCount < 255)
			this->creditCount++;		
	}

	if (start1Pressed) {
		this->lastStartPressed[0] = true;		
	}
	else if (this->lastStartPressed[0]) {
		this->lastStartPressed[0] = false;
		if (this->creditCount == 0) {
			this->SetAnimationStart(STARTLED_STATE_LED1, STARTLED_ANIM_OFF);
			this->SetAnimationCoin(STARTLED_STATE_LED1, STARTLED_ANIM_BLINK, STARTLED_SPEED_FAST);								
		}			
		else {
			this->creditCount--;
			if (this->creditCount == 0)
				this->SetAnimationStart(STARTLED_STATE_LED1, STARTLED_ANIM_OFF);
			else				
				this->SetAnimationStart(STARTLED_STATE_LED1, STARTLED_ANIM_SOLID);
		}		
	}	

	this->ledsStart->animate(this->animationStateStart);
	this->ledsCoin->animate(this->animationStateCoin);
}

void StartLEDAddon::SetAnimationStart(StartLEDStateMask buttonStateMask, StartLEDAnimationType animationType, StartLEDAnimationSpeed animationSpeed)
{
	this->animationStateStart.state = (this->animationStateStart.state | buttonStateMask);
	this->animationStateStart.animation = animationType;
	this->animationStateStart.speed = animationSpeed;
}

void StartLEDAddon::SetAnimationCoin(StartLEDStateMask buttonStateMask, StartLEDAnimationType animationType, StartLEDAnimationSpeed animationSpeed)
{
	this->animationStateCoin.state = (this->animationStateCoin.state | buttonStateMask);
	this->animationStateCoin.animation = animationType;
	this->animationStateCoin.speed = animationSpeed;
}

