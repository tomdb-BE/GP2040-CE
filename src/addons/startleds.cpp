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
	if (pins[0] == 0xFF)
		return slices;
		
	uint8_t newPin;
	this->brightness = mBrightness;
	this->maxBrightness = mBrightness;
	this->maxAnimationBrightness = mBrightness;
	for (uint8_t i=0; i < STARTLEDS_COUNT; i++)
	{
		newPin = pins[i];
		this->ledPins[i] = newPin;
		if (newPin != 0xFF)
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
	if (! this->ready)
		return;

	this->animate();
	for (int i = 0; i < STARTLEDS_COUNT; i++)
		if (this->ledPins[i] != 0xFF)
			pwm_set_gpio_level(this->ledPins[i], this->ledLevels[i]);
}

inline void StartLeds::animate()
{
	uint8_t newState = this->animation.currentState;
	uint8_t mask = this->animation.stateMask;
	if (this->animation.currentType != this->animation.previousType)	
		this->resetAnimation();			
	else if (!time_reached(this->nextAnimationTime))	
		return;			

	switch (this->animation.currentType)
	{
		case STARTLEDS_ANIM_OFF:
			newState = STARTLEDS_STATE_ALL_OFF | mask;
			break;

		case STARTLEDS_ANIM_SOLID:
			newState = STARTLEDS_STATE_ALL_ON & mask;
			break;	

		case STARTLEDS_ANIM_BLINK:
			newState = ~newState & mask;
			this->nextAnimationTime = make_timeout_time_ms(this->animation.speed);
			break;

		case STARTLEDS_ANIM_FADE:
			newState = newState & mask;
			this->brightness = this->handleBrightness();
			this->nextAnimationTime = make_timeout_time_ms(this->animation.speed);
			break;

		default:
			break;
	}

	this->animation.currentState = newState;

	for (uint8_t i = 0; i < STARTLEDS_COUNT; i++)
		if (this->ledPins[i] != 0xFF)
			this->ledLevels[i] = (newState & (1 << i)) ? this->brightness * this->brightness : 0;			
}

inline void StartLeds::resetAnimation()
{	
	this->animation.previousType = this->animation.currentType;	
	this->brightness = this->maxAnimationBrightness;
	this->fadeIn = false;
	if (this->animation.currentType == STARTLEDS_ANIM_FADE)
		this->animation.speed = STARTLEDS_SPEED_LUDICROUS + (this->maxBrightness - this->maxAnimationBrightness);
	this->nextAnimationTime = make_timeout_time_ms(0);
}



uint8_t StartLeds::handleBrightness(uint8_t amount)
{
	int16_t nBrightness = this->brightness;
	int16_t mBrightness = this->maxAnimationBrightness;

	if (this->fadeIn)	
	{				
		nBrightness += amount;
		if (nBrightness >= mBrightness) {
			this->fadeIn = false;
			return mBrightness;
		}
		return nBrightness;
	}
	
	nBrightness -= amount;
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
	uint8_t startPins[] = {options.startLedsStartPin1, options.startLedsStartPin2};
	uint8_t coinPins[] = {options.startLedsCoinPin1, options.startLedsCoinPin2};
	uint8_t marqueePins[] = {options.startLedsMarqueePin, 0xFF};
	uint8_t extStartPin = options.startLedsExtStartPin;
	uint8_t extCoinPin = options.startLedsExtCoinPin;
	uint8_t startBrightness = (float)options.startLedsStartBrightness / 100 * 255;	
	uint8_t coinBrightness = (float)options.startLedsCoinBrightness / 100 * 255;	
	uint8_t marqueeBrightness = (float)options.startLedsMarqueeBrightness / 100 * 255;

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 4.f);

	std::vector<uint> sliceNums;

	sliceNums = this->ledsStart.initialize(sliceNums, startPins, startBrightness, STARTLEDS_ALL_OFF); 
	sliceNums = this->ledsCoin.initialize(sliceNums, coinPins, coinBrightness, STARTLEDS_ALL_ON);
	sliceNums = this->ledsMarquee.initialize(sliceNums, marqueePins, marqueeBrightness, STARTLEDS_ALL_ON);

	for (auto sliceNum : sliceNums)
		pwm_set_enabled(sliceNum, true);
	
	if (extStartPin != 0xFF)
	{
		gpio_init(extStartPin);
    	gpio_set_dir(extStartPin, GPIO_OUT);
		gpio_put(extStartPin, 1);
		this->externalStartPin = extStartPin;
	}

	if (extCoinPin != 0xFF)
	{
		gpio_init(extCoinPin);
    	gpio_set_dir(extCoinPin, GPIO_OUT);
		gpio_put(extCoinPin, 1);
		this->externalCoinPin = extCoinPin;
	}
	this->ready = true;
}

void StartLedsAddon::process()
{
    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	uint16_t buttonsPressed = gamepad->state.buttons & (START_BUTTON_MASKS | COIN_BUTTON_MASKS | STARTLEDS_EXT_MASKS);
	uint8_t dpadPressed = gamepad->state.dpad & GAMEPAD_MASK_DPAD;

	this->ledsStart.display();
	this->ledsCoin.display();
	this->ledsMarquee.display();

	if (!this->ready)
		return;

	if ((buttonsPressed & COIN_BUTTON_MASKS) && dpadPressed)
	{
		this->lastButtonsPressed = buttonsPressed;		
		if (dpadPressed	== GAMEPAD_MASK_DOWN && this->lastDpadPressed != GAMEPAD_MASK_DOWN) {
			this->ledsMarquee.toggleState();
			this->ledsCoin.toggleState();
			this->ledsStart.toggleState();
			this->lastDpadPressed = dpadPressed;
			return;
		}
		this->lastDpadPressed = dpadPressed;
		if (this->debounce())
			return;
		switch (dpadPressed)
		{
			case GAMEPAD_MASK_UP:                        this->ledsMarquee.brightnessCycle();  break;			
			case GAMEPAD_MASK_LEFT:                      this->ledsCoin.brightnessCycle();     break;			
			case GAMEPAD_MASK_RIGHT:                     this->ledsStart.brightnessCycle();    break;
		}
		return;
	}

	this->lastDpadPressed = dpadPressed;

	if (buttonsPressed == this->lastButtonsPressed)
		return;
	
	this->lastButtonsPressed = buttonsPressed;

	if ((buttonsPressed & STARTLEDS_EXT_MASKS))
		if (this->externalStartPin != 0xFF)
			(buttonsPressed & STARTLEDS_EXT_START_MASK) ?  gpio_put(this->externalStartPin, 1) : gpio_put(this->externalStartPin, 0);
		if (this->externalCoinPin != 0xFF)
			(buttonsPressed & STARTLEDS_EXT_COIN_MASK) ?  gpio_put(this->externalCoinPin, 1) : gpio_put(this->externalCoinPin, 0);			

	if (!this->ledsStart.isReady() || !this->ledsCoin.isReady() || dpadPressed)
		return;

	if (buttonsPressed & COIN_BUTTON_MASKS)
	{				
		if (this->creditCount == 0)
		{
			this->ledsStart.setAnimation(STARTLEDS_BLINK_FAST_ALL);
			this->ledsCoin.setAnimation(STARTLEDS_ALL_ON);
		}
		if (this->creditCount < 0xFF)
			this->creditCount++;
	}
	else if (buttonsPressed & START_BUTTON_MASKS) 
	{
		if (this->creditCount == 0) {
			this->ledsCoin.setAnimation(STARTLEDS_FADE_ALL);
			return;
		}
		this->creditCount--;
		if (this->creditCount == 0)		
			this->ledsStart.setAnimation(STARTLEDS_ALL_OFF);
		else		
			this->ledsStart.setAnimation(STARTLEDS_ALL_ON);
	}	
}

bool StartLedsAddon::debounce()
{
	if (STARTLEDS_DEBOUNCE_MILLIS <= 0)
		return true;
				
    uint32_t nowTime = getMillis();

    if ((nowTime - this->debounceBrightness) > STARTLEDS_DEBOUNCE_MILLIS)
	{
        this->debounceBrightness = nowTime;
		return false;
    }
    return true;
}
