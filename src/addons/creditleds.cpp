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
#include "addons/creditleds.h"
#include "helper.h"


inline void CreditLeds::resetAnimation()
{	
	this->animation.previousType = this->animation.currentType;	
	this->brightness = this->maxAnimationBrightness;
	this->fadeIn = false;
	if (this->animation.currentType == CREDITLEDS_ANIM_FADE)
		this->animation.speed = CREDITLEDS_SPEED_LUDICROUS + (this->maxBrightness - this->maxAnimationBrightness) / 2;
	this->nextAnimationTime = make_timeout_time_ms(0);
}

void CreditLeds::handleBrightness(uint8_t mBrightness, uint8_t amount)
{
	uint8_t nBrightness = this->brightness;
	if (this->fadeIn)	
	{					
		nBrightness += amount;	
		if (nBrightness >= mBrightness || nBrightness < this->brightness) {
			this->fadeIn = false;
			nBrightness = mBrightness;
		}		
	}
	else
	{
		nBrightness -= amount;
		if (nBrightness > this->brightness) {
			this->fadeIn = true;
			nBrightness = 0;
		}
	}
	this->brightness = nBrightness;
}

inline void CreditLeds::animate()
{
	uint8_t newState = this->animation.currentState;
	uint8_t mask = this->animation.stateMask;
	if (this->animation.currentType != this->animation.previousType)	
		this->resetAnimation();			
	else if (!time_reached(this->nextAnimationTime))	
		return;			

	switch (this->animation.currentType)
	{
		case CREDITLEDS_ANIM_OFF:
			newState = CREDITLEDS_STATE_ALL_OFF | mask;
			break;

		case CREDITLEDS_ANIM_SOLID:
			newState = CREDITLEDS_STATE_ALL_ON & mask;
			break;	

		case CREDITLEDS_ANIM_BLINK:
			newState = ~newState & mask;
			this->nextAnimationTime = make_timeout_time_ms(this->animation.speed);
			break;

		case CREDITLEDS_ANIM_FADE:
			newState = newState & mask;
			this->handleBrightness(this->maxAnimationBrightness);
			this->nextAnimationTime = make_timeout_time_ms(this->animation.speed);
			break;

		default:
			break;
	}

	this->animation.currentState = newState;

	for (uint8_t i = 0; i < CREDITLEDS_COUNT; i++)
		if (this->ledPins[i] != 0xFF)
			this->ledLevels[i] = (newState & (1 << i)) ? this->brightness * this->brightness : 0;			
}

std::vector<uint> CreditLeds::initialize(std::vector<uint> slices, uint8_t * pins, uint8_t mBrightness, CreditLedsAnimation initAnimation) 
{
	if (pins[0] == 0xFF)
		return slices;
		
	uint8_t newPin;
	this->brightness = mBrightness;	
	this->maxAnimationBrightness = mBrightness;
	for (uint8_t i=0; i < CREDITLEDS_COUNT; i++)
	{
		newPin = pins[i];
		this->ledPins[i] = newPin;
		if (newPin != 0xFF)
		{
			gpio_set_function(newPin, GPIO_FUNC_PWM);
			uint sliceNum = pwm_gpio_to_slice_num(newPin);
			uint channelNum = pwm_gpio_to_channel(newPin);
			slices.push_back(sliceNum);
			pwm_set_chan_level(sliceNum, channelNum, CREDITLEDS_PWM_MAXLEVEL);			
		}
	}
	this->animation = initAnimation;
	this->animate();
	this->ready = true;
	return slices;
}

void CreditLeds::display()
{
	if (! this->ready)
		return;

	this->animate();
	for (int i = 0; i < CREDITLEDS_COUNT; i++)
		if (this->ledPins[i] != 0xFF)
			pwm_set_gpio_level(this->ledPins[i], this->ledLevels[i]);
}

// ADDON

bool CreditLedsAddon::debounce()
{
	if (CREDITLEDS_DEBOUNCE_MILLIS <= 0)
		return true;
				
    uint32_t nowTime = getMillis();

    if ((nowTime - this->debounceBrightness) > CREDITLEDS_DEBOUNCE_MILLIS)
	{
        this->debounceBrightness = nowTime;
		return false;
    }
    return true;
}

void CreditLedsAddon::saveBrightness()
{
	AddonOptions options = Storage::getInstance().getAddonOptions();
	options.creditLedsStartBrightness = this->ledsStart.getAnimationBrightness();
	options.creditLedsCoinBrightness =  this->ledsCoin.getAnimationBrightness();
	options.creditLedsMarqueeBrightness = this->ledsMarquee.getAnimationBrightness();
	Storage::getInstance().setAddonOptions(options);
}

bool CreditLedsAddon::available() {
	AddonOptions options = Storage::getInstance().getAddonOptions();	
	return options.creditLedsAddonEnabled;
}

void CreditLedsAddon::setup() {	
	AddonOptions options = Storage::getInstance().getAddonOptions();
	uint8_t startPins[] = {options.creditLedsStartPin1, options.creditLedsStartPin2};
	uint8_t coinPins[] = {options.creditLedsCoinPin1, options.creditLedsCoinPin2};
	uint8_t marqueePins[] = {options.creditLedsMarqueePin, 0xFF};
	uint8_t extStartPin = options.creditLedsExtStartPin;
	uint8_t extCoinPin = options.creditLedsExtCoinPin;
	uint8_t startBrightness = options.creditLedsStartBrightness;	
	uint8_t coinBrightness = options.creditLedsCoinBrightness;	
	uint8_t marqueeBrightness = options.creditLedsMarqueeBrightness;

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 4.f);

	std::vector<uint> sliceNums;

	sliceNums = this->ledsStart.initialize(sliceNums, startPins, startBrightness, CREDITLEDS_ALL_OFF); 
	sliceNums = this->ledsCoin.initialize(sliceNums, coinPins, coinBrightness, CREDITLEDS_ALL_ON);
	sliceNums = this->ledsMarquee.initialize(sliceNums, marqueePins, marqueeBrightness, CREDITLEDS_ALL_ON);

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

void CreditLedsAddon::processBrightness(uint8_t dpadPressedMask)
{	
	if (dpadPressedMask	== GAMEPAD_MASK_DOWN && this->lastDpadPressed != GAMEPAD_MASK_DOWN) {		
		this->ledsMarquee.toggleState();
		this->ledsCoin.toggleState();
		if (this->ledsStart.toggleState())
			this->saveBrightness();
		return;
	}

	if (this->debounce())
		return;
	
	switch (dpadPressedMask)
	{
		case GAMEPAD_MASK_UP:    this->ledsMarquee.setAnimationBrightness(); break;			
		case GAMEPAD_MASK_LEFT:  this->ledsCoin.setAnimationBrightness();    break;			
		case GAMEPAD_MASK_RIGHT: this->ledsStart.setAnimationBrightness();   break;
	}	
}

void CreditLedsAddon::processExternalButtons(uint16_t buttonsPressedMask)
{
	if (this->externalStartPin != 0xFF)
		(buttonsPressedMask & CREDITLEDS_EXT_START_MASK) ?  gpio_put(this->externalStartPin, 1) : gpio_put(this->externalStartPin, 0);
	if (this->externalCoinPin != 0xFF)
		(buttonsPressedMask & CREDITLEDS_EXT_COIN_MASK) ?  gpio_put(this->externalCoinPin, 1) : gpio_put(this->externalCoinPin, 0);
}

void CreditLedsAddon::processCredits(uint16_t buttonsPressedMask)
{	
	if (buttonsPressedMask & CREDITLEDS_COIN_BUTTON_MASKS)
	{				
		if (this->creditCount == 0)
		{
			this->ledsStart.setAnimation(CREDITLEDS_BLINK_FAST_ALL);
			this->ledsCoin.setAnimation(CREDITLEDS_ALL_ON);
		}
		if (this->creditCount < 0xFF)
			this->creditCount++;
		return;
	}
	if (buttonsPressedMask & CREDITLEDS_START_BUTTON_MASKS) 
	{
		if (this->creditCount == 0) {
			this->ledsCoin.setAnimation(CREDITLEDS_FADE_ALL);
			return;
		}
		this->creditCount--;
		if (this->creditCount == 0)		
			this->ledsStart.setAnimation(CREDITLEDS_ALL_OFF);
		else		
			this->ledsStart.setAnimation(CREDITLEDS_ALL_ON);
		return;
	}
}

void CreditLedsAddon::process()
{
    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	uint16_t buttonsPressed = gamepad->state.buttons & (CREDITLEDS_START_BUTTON_MASKS | CREDITLEDS_COIN_BUTTON_MASKS | CREDITLEDS_EXT_MASKS);
	uint8_t dpadPressed = gamepad->state.dpad & GAMEPAD_MASK_DPAD;

	this->ledsStart.display();
	this->ledsCoin.display();
	this->ledsMarquee.display();

	if (!this->ready)
		return;	

	if (dpadPressed)
	{
		if (buttonsPressed & CREDITLEDS_COIN_BUTTON_MASKS)
			this->processBrightness(dpadPressed);
		this->lastDpadPressed = dpadPressed;
		this->lastButtonsPressed = buttonsPressed;	
		return;
	}
	this->lastDpadPressed = 0;

	if (buttonsPressed == this->lastButtonsPressed)
		return;
	this->lastButtonsPressed = buttonsPressed;

	this->processExternalButtons(buttonsPressed);
	if (buttonsPressed)
		this->processCredits(buttonsPressed);

}