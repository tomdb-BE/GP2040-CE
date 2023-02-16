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


void StartLeds::setup(const uint8_t * pinConfig, uint8_t mBrightness)
{
	this->ledPins = pinConfig;		
	this->maxBrightness = mBrightness;
	this->brightness = mBrightness;

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 4.f);

	std::vector<uint> sliceNums;

	for (uint8_t i = 0; i < STARTLEDS_COUNT; i++)
	{
		if (pinConfig[i] != 255)
		{
			gpio_set_function(pinConfig[i], GPIO_FUNC_PWM);
			uint sliceNum = pwm_gpio_to_slice_num(pinConfig[i]);
			uint channelNum = pwm_gpio_to_channel(pinConfig[i]);
			sliceNums.push_back(sliceNum);
			pwm_set_chan_level(sliceNum, channelNum, STARTLEDS_PWM_MAXLEVEL);
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

void StartLeds::animate()
{
	uint8_t state = this->animation.state;
	uint8_t mask = this->animation.stateMask;
	if (this->animation.type != this->animation.previousType)	
		this->reset();			
	else if (!time_reached(this->nextAnimationTime))	
		return;			

	switch (this->animation.type)
	{
		case STARTLEDS_ANIM_OFF:
			this->animation.state = STARTLEDS_STATE_ALL_OFF | mask;
			break;

		case STARTLEDS_ANIM_SOLID:
			this->animation.state = STARTLEDS_STATE_ALL_ON & mask;
			break;	

		case STARTLEDS_ANIM_BLINK:
			this->animation.state = ~state & mask;
			this->nextAnimationTime = make_timeout_time_ms(this->animation.speed);
			break;

		case STARTLEDS_ANIM_FADE:
			this->brightness = this->HandleFade(this->brightness, this->maxBrightness);
			this->nextAnimationTime = make_timeout_time_ms(this->animation.speed);
			break;

		default:
			break;
	}

	this->animation.previousState = this->animation.state;

	for (uint8_t i = 0; i < STARTLEDS_COUNT; i++)
		if (this->ledPins[i] != 255)
			this->ledLevels[i] = (this->animation.state & (1 << i) == (1 << i)) ? (this->brightness * this->brightness) : 0;
}

inline void StartLeds::reset()
{	
	this->animation.previousType = this->animation.type;
	this->nextAnimationTime = get_absolute_time();
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
		this->fadeIn = false;
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
	const uint8_t startPins[] = {options.startLedsStartPin1, options.startLedsStartPin2, options.startLedsStartPin3, options.startLedsStartPin4};
	const uint8_t coinPins[] = {options.startLedsCoinPin1, options.startLedsCoinPin2, options.startLedsCoinPin3, options.startLedsCoinPin4};
	const uint8_t marqueePins[] = {options.startLedsMarqueePin, 255, 255, 255};
	const uint8_t startBrightness = (float)options.startLedsStartBrightness / 100 * 255;	
	const uint8_t coinBrightness = (float)options.startLedsCoinBrightness / 100 * 255;	
	const uint8_t marqueeBrightness = (float)options.startLedsMarqueeBrightness / 100 * 255;	

	if (startPins[0] != 255)
		this->ledsStart = new StartLeds();		
		this->ledsStart->animation = STARTLEDS_ALL_ON;
		this->ledsStart->setup(startPins, startBrightness);
	if (coinPins[0] != 255)
		this->ledsCoin = new StartLeds();
		this->ledsCoin->animation = STARTLEDS_FADE_ALL;			
		this->ledsCoin->setup(coinPins, coinBrightness);		
	if (marqueePins[0] != 255)
		this->ledsMarquee = new StartLeds();
		this->ledsMarquee->animation = STARTLEDS_ALL_ON;
		this->ledsMarquee->setup(marqueePins, marqueeBrightness);

this->ledsStart->animate();
	this->ledsCoin->animate();
			this->ledsStart->display();
	this->ledsCoin->display();

	
}

void StartLedsAddon::process()
{

    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	uint16_t start1Pressed = gamepad->state.buttons & START1_BUTTON_MASK;
	uint16_t coin1Pressed = gamepad->state.buttons & COIN1_BUTTON_MASK;	

	if (this->ledsMarquee != nullptr) {
		this->ledsMarquee->display();
		this->ledsMarquee->animate();
	}

	if (this->ledsStart == nullptr || this->ledsCoin == nullptr)
		return;
	
	this->ledsStart->display();
	this->ledsCoin->display();

	this->ledsStart->animate();
	this->ledsCoin->animate();

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
}
