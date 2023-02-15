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


void StartLeds::setup(const int * pinConfig, bool iState, uint8_t mBrightness, uint16_t mLevel)
{
	this->ledPins = pinConfig;		
	this->maxBrightness = mBrightness;
	this->brightness = mBrightness;
	this->maxLevel = mLevel;
	this->initialState = iState;

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 4.f);

	std::vector<uint> sliceNums;

	for (int i = 0; i < STARTLEDS_COUNT; i++)
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

void StartLeds::display()
{
	for (int i = 0; i < STARTLEDS_COUNT; i++)
		if (this->ledPins[i] > -1)
			pwm_set_gpio_level(this->ledPins[i], this->ledLevels[i]);
}

void StartLeds::animate(StartLedsAnimationState animationState)
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
		case STARTLEDS_ANIM_BLINK:
			this->handleBlink(animationState.speed);
			break;

		case STARTLEDS_ANIM_FADE:
			this->handleFade();
			break;

		case STARTLEDS_ANIM_OFF:
			for (int i = 0; i < STARTLEDS_COUNT; i++)
				this->currentLedState[i] = 1;	
			this->brightness = this->maxBrightness;
			break;

		case STARTLEDS_ANIM_SOLID:
			for (int i = 0; i < STARTLEDS_COUNT; i++)
				this->currentLedState[i] = 1;		
			this->brightness = 0;
			break;						

		default:
			break;
	}

	for (int i = 0; i < STARTLEDS_COUNT; i++)
		if (this->ledPins[i] > -1)
			this->ledLevels[i] = this->maxLevel - (this->currentLedState[i] ? (this->brightness * this->brightness) : 0);
}

bool StartLedsAddon::available() {
	AddonOptions options = Storage::getInstance().getAddonOptions();	
	return options.startLedsAddonEnabled;
}

void StartLedsAddon::setup() {
	AddonOptions options = Storage::getInstance().getAddonOptions();
	const int startPins[] = {options.startLedsStartPin1, options.startLedsStartPin2, options.startLedsStartPin3, options.startLedsStartPin4};
	const int coinPins[] = {options.startLedsCoinPin1, options.startLedsCoinPin2, options.startLedsCoinPin3, options.startLedsCoinPin4};
	const int marqueePins[] = {options.startLedsMarqueePin, -1, -1, -1};
	uint8_t startBrightness = options.startLedsStartBrightness / 100 * 255;
	uint16_t startLevel = startBrightness * 256;
	uint8_t coinBrightness = options.startLedsCoinBrightness / 100 * 255;
	uint16_t coinLevel = coinBrightness * 256;		
	uint8_t marqueeBrightness = options.startLedsMarqueeBrightness / 100 * 255;
	uint16_t marqueeLevel = marqueeBrightness * 256;

	this->ledsStart = new StartLeds();
	this->ledsCoin = new StartLeds();
	this->ledsMarquee = new StartLeds();

	if (ledsStart != nullptr)
		this->ledsStart->setup(startPins, false, startBrightness, startLevel);	
	if (ledsCoin != nullptr)
		this->ledsCoin->setup(coinPins, true, coinBrightness, coinLevel);
	if (marqueePins != nullptr)
		this->ledsMarquee->setup(marqueePins, true, marqueeBrightness, marqueeLevel);
	
	this->animationStateStart.animation = STARTLEDS_ANIM_OFF;
	this->animationStateCoin.animation = STARTLEDS_ANIM_SOLID;
	this->animationStateMarquee.animation = STARTLEDS_ANIM_SOLID;
}

void StartLedsAddon::process()
{

    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	uint16_t start1Pressed = gamepad->state.buttons & START1_BUTTON_MASK;
	uint16_t coin1Pressed = gamepad->state.buttons & COIN1_BUTTON_MASK;

	this->SetAnimationStart(STARTLEDS_STATE_LED2, STARTLEDS_ANIM_BLINK, STARTLEDS_SPEED_FAST);

	if (this->ledsStart != nullptr)
		this->ledsStart->display();
	if (this->ledsCoin != nullptr)
		this->ledsCoin->display();
	if (this->ledsMarquee != nullptr)
		this->ledsMarquee->display();		

	if (coin1Pressed) {	
		this->lastCoinPressed[0] = true;
	}	
	else if (this->lastCoinPressed[0]) {
		this->SetAnimationCoin(STARTLEDS_STATE_LED1, STARTLEDS_ANIM_SOLID);
		this->lastCoinPressed[0] = false;
		if (this->creditCount == 0)
			this->SetAnimationStart(STARTLEDS_STATE_LED1, STARTLEDS_ANIM_BLINK, STARTLEDS_SPEED_FAST);
		else if (this->animationStateStart.animation != STARTLEDS_ANIM_BLINK)
			this->SetAnimationStart(STARTLEDS_STATE_LED1, STARTLEDS_ANIM_SOLID);
		if (this->creditCount < 255)
			this->creditCount++;		
	}

	if (start1Pressed) {
		this->lastStartPressed[0] = true;		
	}
	else if (this->lastStartPressed[0]) {
		this->lastStartPressed[0] = false;
		if (this->creditCount == 0) {
			this->SetAnimationStart(STARTLEDS_STATE_LED1, STARTLEDS_ANIM_OFF);
			this->SetAnimationCoin(STARTLEDS_STATE_LED1, STARTLEDS_ANIM_BLINK, STARTLEDS_SPEED_FAST);								
		}			
		else {
			this->creditCount--;
			if (this->creditCount == 0)
				this->SetAnimationStart(STARTLEDS_STATE_LED1, STARTLEDS_ANIM_OFF);
			else				
				this->SetAnimationStart(STARTLEDS_STATE_LED1, STARTLEDS_ANIM_SOLID);
		}		
	}	

	this->ledsStart->animate(this->animationStateStart);
	this->ledsCoin->animate(this->animationStateCoin);
	this->ledsCoin->animate(this->animationStateMarquee);
}


void StartLedsAddon::SetAnimationStart(StartLedsStateMask buttonStateMask, StartLedsAnimationType animationType, StartLedsAnimationSpeed animationSpeed)
{
	this->animationStateStart.state = (this->animationStateStart.state | buttonStateMask);
	this->animationStateStart.animation = animationType;
	this->animationStateStart.speed = animationSpeed;
}

void StartLedsAddon::SetAnimationCoin(StartLedsStateMask buttonStateMask, StartLedsAnimationType animationType, StartLedsAnimationSpeed animationSpeed)
{
	this->animationStateCoin.state = (this->animationStateCoin.state | buttonStateMask);
	this->animationStateCoin.animation = animationType;
	this->animationStateCoin.speed = animationSpeed;
}

