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
#include "coinleds.h"
#include "helper.h"


std::vector<uint> CoinLeds::initialize(std::vector<uint> slices, int32_t * pins, uint8_t mBrightness, CoinLedsAnimation initAnimation) 
{
	if (pins[0] < 0)
		return slices;
		
	int32_t newPin;
	this->brightness = mBrightness;
	this->maxBrightness = mBrightness;
	for (uint8_t i=0; i < COINLEDS_COUNT; i++)
	{
		newPin = pins[i];
		this->ledPins[i] = newPin;
		if (newPin < 0)
		{
			gpio_set_function(newPin, GPIO_FUNC_PWM);
			uint sliceNum = pwm_gpio_to_slice_num(newPin);
			uint channelNum = pwm_gpio_to_channel(newPin);
			slices.push_back(sliceNum);
			pwm_set_chan_level(sliceNum, channelNum, COINLEDS_PWM_MAXLEVEL);			
		}
	}
	this->animation = initAnimation;
	this->animate();
	this->ready = true;
	return slices;
}

void CoinLeds::display()
{
	if (! this->ready)
		return;

	this->animate();
	for (int i = 0; i < COINLEDS_COUNT; i++)
		if (this->ledPins[i] != 0xFF)
			pwm_set_gpio_level(this->ledPins[i], this->ledLevels[i]);
}

inline void CoinLeds::animate()
{
	uint8_t newState = this->animation.currentState;
	uint8_t mask = this->animation.stateMask;
	if (this->animation.currentType != this->animation.previousType)	
		this->reset();			
	else if (!time_reached(this->nextAnimationTime))	
		return;			

	switch (this->animation.currentType)
	{
		case COINLEDS_ANIM_OFF:
			newState = COINLEDS_STATE_ALL_OFF | mask;
			break;

		case COINLEDS_ANIM_SOLID:
			newState = COINLEDS_STATE_ALL_ON & mask;
			break;	

		case COINLEDS_ANIM_BLINK:
			newState = ~newState & mask;
			this->nextAnimationTime = make_timeout_time_ms(this->animation.speed);
			break;

		case COINLEDS_ANIM_FADE:
			newState = newState & mask;
			this->brightness = this->handleBrightness(COINLEDS_BRIGHTNESS_STEP, this->fadeIn);
			this->nextAnimationTime = make_timeout_time_ms(this->animation.speed);
			break;

		default:
			break;
	}

	this->animation.currentState = newState;

	for (uint8_t i = 0; i < COINLEDS_COUNT; i++)
		if (this->ledPins[i] != 0xFF)
			this->ledLevels[i] = (newState & (1 << i)) ? this->brightness * this->brightness : 0;			
}

inline void CoinLeds::reset()
{	
	this->animation.previousType = this->animation.currentType;
	this->nextAnimationTime = make_timeout_time_ms(0);
	this->brightness = this->maxBrightness;
	this->fadeIn = false;
}

uint8_t CoinLeds::handleBrightness(uint8_t amount, bool increase)
{
	int16_t nBrightness = this->brightness;
	uint16_t mBrightness = this->maxBrightness;

	if (increase)	
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

bool CoinLedsAddon::available() {
	return Storage::getInstance().getAddonOptions().coinLedsOptions.enabled;
}

void CoinLedsAddon::setup() {	
	const CoinLedsOptions options = Storage::getInstance().getAddonOptions().coinLedsOptions;	
	int32_t startPins[] = {options.coinLedsStartPin1, options.coinLedsStartPin2, options.coinLedsStartPin3, options.coinLedsStartPin4};
	int32_t coinPins[] = {options.coinLedsCoinPin1, options.coinLedsCoinPin2, options.coinLedsCoinPin3, options.coinLedsCoinPin4};
	int32_t marqueePins[] = {options.coinLedsMarqueePin, -1, -1, -1};
	int32_t extStartPin = options.coinLedsExtStartPin;
	int32_t extCoinPin = options.coinLedsExtCoinPin;
	uint8_t startBrightness = (options.coinLedsStartBrightness >= 100) ? 255 : (float)options.coinLedsStartBrightness / 100 * 255;	
	uint8_t coinBrightness = (options.coinLedsCoinBrightness >= 100) ? 255 : (float)options.coinLedsCoinBrightness / 100 * 255;	
	uint8_t marqueeBrightness = (options.coinLedsMarqueeBrightness >= 100) ? 255 : (float)options.coinLedsMarqueeBrightness / 100 * 255;

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 4.f);

	std::vector<uint> sliceNums;

	sliceNums = this->ledsStart.initialize(sliceNums, startPins, startBrightness, COINLEDS_ALL_OFF); 
	sliceNums = this->ledsCoin.initialize(sliceNums, coinPins, coinBrightness, COINLEDS_ALL_ON);
	sliceNums = this->ledsMarquee.initialize(sliceNums, marqueePins, marqueeBrightness, COINLEDS_ALL_ON);

	for (auto sliceNum : sliceNums)
		pwm_set_enabled(sliceNum, true);
	
	if (extStartPin > 0)
	{
		gpio_init(extStartPin);
    	gpio_set_dir(extStartPin, GPIO_OUT);
		gpio_put(extStartPin, 1);
		this->externalStartPin = extStartPin;
	}

	if (extCoinPin > 0)
	{
		gpio_init(extCoinPin);
    	gpio_set_dir(extCoinPin, GPIO_OUT);
		gpio_put(extCoinPin, 1);
		this->externalCoinPin = extCoinPin;
	}
	this->ready = true;
}

void CoinLedsAddon::process()
{
    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	uint16_t buttonsPressed = gamepad->state.buttons & (START_BUTTON_MASKS | COIN_BUTTON_MASKS | COINLEDS_EXT_MASKS);
	uint16_t dpadPressed = gamepad->state.dpad & GAMEPAD_MASK_DPAD;

	this->ledsStart.display();
	this->ledsCoin.display();
	this->ledsMarquee.display();

	if (!(buttonsPressed && this->ready))
		return;

	if ((buttonsPressed & COIN_BUTTON_MASKS) && dpadPressed)
	{
		if (this->debounce(&this->debounceMarqueeBrightness) || !this->ledsMarquee.isReady())
			return;
		if (dpadPressed & GAMEPAD_MASK_UP)
			this->ledsMarquee.brightnessUp();
		if (dpadPressed & GAMEPAD_MASK_DOWN)
			this->ledsMarquee.brightnessDown();
		return;
	}

	if (buttonsPressed == this->lastButtonsPressed)
		return;
	
	this->lastButtonsPressed = buttonsPressed;

	if ((buttonsPressed & COINLEDS_EXT_MASKS))
		if (this->externalStartPin != 0xFF)
			(buttonsPressed & COINLEDS_EXT_START_MASK) ?  gpio_put(this->externalStartPin, 1) : gpio_put(this->externalStartPin, 0);
		if (this->externalCoinPin != 0xFF)
			(buttonsPressed & COINLEDS_EXT_COIN_MASK) ?  gpio_put(this->externalCoinPin, 1) : gpio_put(this->externalCoinPin, 0);			

	if (!this->ledsStart.isReady() || !this->ledsCoin.isReady() || dpadPressed)
		return;

	if (buttonsPressed & COIN_BUTTON_MASKS)
	{				
		if (this->creditCount == 0)
		{
			this->ledsStart.setAnimation(COINLEDS_BLINK_FAST_ALL);
			this->ledsCoin.setAnimation(COINLEDS_ALL_ON);
		}
		if (this->creditCount < 0xFF)
			this->creditCount++;
	}
	else if (buttonsPressed & START_BUTTON_MASKS) 
	{
		if (this->creditCount > 0)
			this->creditCount--;				
		if (this->creditCount == 0)
		{
			this->ledsStart.setAnimation(COINLEDS_ALL_OFF);
			this->ledsCoin.setAnimation(COINLEDS_FADE_ALL);
		}
		else
		{
			this->ledsStart.setAnimation(COINLEDS_ALL_ON);
		}
	}	
}

bool CoinLedsAddon::debounce(uint32_t * debounceTime)
{
	if (COINLEDS_DEBOUNCE_MILLIS <= 0)
		return true;
				
    uint32_t nowTime = getMillis();

    if ((nowTime - *debounceTime) > COINLEDS_DEBOUNCE_MILLIS)
	{
        *debounceTime = nowTime;
		return false;
    }
    return true;
}
