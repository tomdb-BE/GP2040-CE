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


std::vector<uint> StartLeds::initialize(std::vector<uint> slices, uint8_t * pins, uint8_t mBrightness, StartLedsAnimation initAnimation) 
{
	if (pins[0] == 255)
		return slices;
		
	uint8_t newPin;
	this->brightness = mBrightness;
	this->maxBrightness = mBrightness;
	for (uint8_t i=0; i < STARTLEDS_COUNT; i++)
	{
		newPin = pins[i];
		this->ledPins[i] = newPin;
		if (newPin != 255)
		{
			gpio_set_function(newPin, GPIO_FUNC_PWM);
			uint sliceNum = pwm_gpio_to_slice_num(newPin);
			uint channelNum = pwm_gpio_to_channel(newPin);
			slices.push_back(sliceNum);
			pwm_set_chan_level(sliceNum, channelNum, STARTLEDS_PWM_MAXLEVEL);			
		}
	}
	this->animation = initAnimation;
	this->animate();
	this->ready = true;
	return slices;
}

void StartLeds::display()
{
	this->animate();
	for (int i = 0; i < STARTLEDS_COUNT; i++)
		if (this->ledPins[i] != 255)
			pwm_set_gpio_level(this->ledPins[i], this->ledLevels[i]);
}

inline void StartLeds::animate()
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
	uint8_t startPins[] = {options.startLedsStartPin1, options.startLedsStartPin2, options.startLedsStartPin3, options.startLedsStartPin4};
	uint8_t coinPins[] = {options.startLedsCoinPin1, options.startLedsCoinPin2, options.startLedsCoinPin3, options.startLedsCoinPin4};
	uint8_t marqueePins[] = {options.startLedsMarqueePin, 255, 255, 255};
	uint8_t startBrightness = (float)options.startLedsStartBrightness / 100 * 255;	
	uint8_t coinBrightness = (float)options.startLedsCoinBrightness / 100 * 255;	
	uint8_t marqueeBrightness = (float)options.startLedsMarqueeBrightness / 100 * 255;

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 4.f);

	std::vector<uint> sliceNums;

	sliceNums = this->ledsStart.initialize(sliceNums, startPins, startBrightness, STARTLEDS_BLINK_FAST_ALL); 
	sliceNums = this->ledsCoin.initialize(sliceNums, coinPins, coinBrightness, STARTLEDS_FADE_ALL);
	sliceNums = this->ledsMarquee.initialize(sliceNums, marqueePins, marqueeBrightness, STARTLEDS_ALL_ON);

	for (auto sliceNum : sliceNums)
		pwm_set_enabled(sliceNum, true);
}

void StartLedsAddon::process()
{
    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	uint16_t start1Pressed = gamepad->state.buttons & START1_BUTTON_MASK;
	uint16_t coin1Pressed = gamepad->state.buttons & COIN1_BUTTON_MASK;	

	if (this->ledsStart.isReady())
		this->ledsStart.display();
	if (this->ledsCoin.isReady())
		this->ledsCoin.display();
	if (this->ledsMarquee.isReady())
		this->ledsMarquee.display();


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
