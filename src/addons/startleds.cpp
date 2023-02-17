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


void StartLeds::display()
{
	for (int i = 0; i < STARTLEDS_COUNT; i++)
		if (this->ledPins[i] != 255)
			pwm_set_gpio_level(this->ledPins[i], this->ledLevels[i]);
}

void StartLeds::animate()
{
	uint8_t newState = this->animation.currentState;
	uint8_t mask = this->animation.stateMask;
	if (this->animation.currentType != this->animation.previousType)	
		this->reset();			
	else if (!time_reached(this->nextAnimationTime))	
		return;			

	switch (this->animation.currentType)
	{
		case STARTLEDS_ANIM_OFF:
			newState = STARTLEDS_STATE_ALL_OFF;
			break;

		case STARTLEDS_ANIM_SOLID:
			newState = STARTLEDS_STATE_ALL_ON;
			break;	

		case STARTLEDS_ANIM_BLINK:
			newState = ~newState;
			this->nextAnimationTime = make_timeout_time_ms(this->animation.speed);
			break;

		case STARTLEDS_ANIM_FADE:
			this->brightness = this->HandleFade(this->brightness, this->maxBrightness);
			this->nextAnimationTime = make_timeout_time_ms(this->animation.speed);
			break;

		default:
			break;
	}	
	this->animation.currentState = newState;
	for (uint8_t i = 0; i < STARTLEDS_COUNT; i++)
		if (this->ledPins[i] != 255)
			this->ledLevels[i] = (newState & (1 << i) == (1 << i)) ? this->brightness * this->brightness : 0;
			//this->ledLevels[i] = this->brightness * this->brightness;
}

inline void StartLeds::reset()
{	
	this->animation.previousType = this->animation.currentType;
	this->nextAnimationTime = make_timeout_time_ms(0);
	this->brightness = this->maxBrightness;
	this->fadeIn = false;
}

inline uint8_t StartLeds::HandleFade(int16_t nBrightness, int16_t mBrightness)
{
	if (this->fadeIn)	
	{				
		nBrightness += 5;
		if (nBrightness >= mBrightness) {
			this->fadeIn = false;
			return mBrightness;
		}
		return nBrightness;
	}
	
	nBrightness -= 5;
	if (nBrightness <= 0) {
		this->fadeIn = true;
		return 0;
	}
	return nBrightness;
}

bool StartLedsAddon::available() {
	AddonOptions options = Storage::getInstance().getAddonOptions();	
	return options.startLedsAddonEnabled;
}

void StartLedsAddon::setup() {	
	AddonOptions options = Storage::getInstance().getAddonOptions();
	uint8_t configuredPins[STARTLEDS_COUNT * 3];
	uint8_t startPins[] = {22, 255, 255, 255};
	uint8_t coinPins[] = {21, 255, 255, 255};
	uint8_t marqueePins[] = {255, 255, 255, 255};
	uint8_t startBrightness = (float)options.startLedsStartBrightness / 100 * 255;	
	uint8_t coinBrightness = (float)options.startLedsCoinBrightness / 100 * 255;	
	uint8_t marqueeBrightness = (float)options.startLedsMarqueeBrightness / 100 * 255;

	for (uint8_t j = 0; j < STARTLEDS_COUNT; j++) {
		 configuredPins[j] = startPins[j];
		 configuredPins[j + STARTLEDS_COUNT] = coinPins[j];
		 configuredPins[j + STARTLEDS_COUNT * 2] = marqueePins[j];
	}

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 4.f);

	std::vector<uint> sliceNums;

	for (uint8_t i = 0; i < STARTLEDS_COUNT * 3; i++)
	{
		if (configuredPins[i] != 255)
		{
			gpio_set_function(configuredPins[i], GPIO_FUNC_PWM);
			uint sliceNum = pwm_gpio_to_slice_num(configuredPins[i]);
			uint channelNum = pwm_gpio_to_channel(configuredPins[i]);
			sliceNums.push_back(sliceNum);
			pwm_set_chan_level(sliceNum, channelNum, STARTLEDS_PWM_MAXLEVEL);			
		}	
	}

	for (auto sliceNum : sliceNums)
		pwm_set_enabled(sliceNum, true);

	if (startPins[0] != 255)
	{			
		this->ledsStart.initialize(startPins, startBrightness); 
		this->ledsStart.SetAnimation(STARTLEDS_FADE_ALL);
		this->ledsStart.animate();
	}
	if (coinPins[0] != 255)
	{
		this->ledsCoin.initialize(coinPins, coinBrightness);
		this->ledsCoin.SetAnimation(STARTLEDS_BLINK_FAST_ALL);
		this->ledsCoin.animate();
	}
	if (marqueePins[0] != 255)
	{		
		this->ledsMarquee.initialize(marqueePins, marqueeBrightness);
		this->ledsMarquee.SetAnimation(STARTLEDS_BLINK_FAST_ALL);
		this->ledsMarquee.animate();
		
	}
	this->setupDone = true;

}

void StartLedsAddon::process()
{
	if (! this->setupDone) return;
    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	uint16_t start1Pressed = gamepad->state.buttons & START1_BUTTON_MASK;
	uint16_t coin1Pressed = gamepad->state.buttons & COIN1_BUTTON_MASK;	

	
	this->ledsStart.display();
	this->ledsCoin.display();

	this->ledsStart.animate();
	this->ledsCoin.animate();
/**
	return;
	if (this->ledsStart == nullptr || this->ledsCoin == nullptr)
		return;
	if (coin1Pressed) {	
		this->lastCoinPressed[0] = true;
	}	
	else if (this->lastCoinPressed[0]) {
		this->ledsCoin->animation = STARTLEDS_ALL_ON;
		this->lastCoinPressed[0] = false;
		if (this->creditCount == 0)
			ledsStart->animation = STARTLEDS_BLINK_FAST_ALL;
		else if (this->ledsStart->animation.type != STARTLEDS_BLINK_FAST_ALL.type)
			this->ledsStart->animation = STARTLEDS_ALL_ON;
		if (this->creditCount < 255)
			this->creditCount++;		
	}

	if (start1Pressed) {
		this->lastStartPressed[0] = true;		
	}
	else if (this->lastStartPressed[0]) {
		this->lastStartPressed[0] = false;
		if (this->creditCount == 0) {
			this->ledsStart->animation = STARTLEDS_ALL_OFF;
			this->ledsCoin->animation = STARTLEDS_FADE_ALL;
		}			
		else {
			this->creditCount--;
			if (this->creditCount == 0)
				this->ledsStart->animation = STARTLEDS_ALL_ON;
			else				
				this->ledsStart->animation = STARTLEDS_ALL_OFF;
		}		
	}	

	this->ledsStart->animate();
	this->ledsCoin->animate();	
	**/
}
