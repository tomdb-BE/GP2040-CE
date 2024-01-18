/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2024 Tom De Backer
 */

#include "addons/coinleds.h"
#include "storagemanager.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "helper.h"

//CoinLeds Class

// Constructor 
CoinLeds::CoinLeds(int32_t * pins, uint8_t mBrightness, CoinLedsAnimation initAnimation) {
	int32_t newPin = -1;
	uint sliceNum = 0;
	uint channelNum = 0;

	for (uint8_t i=0; i < COINLEDS_COUNT; i++)
	{	
		newPin = pins[i];
		if (isValidPin(newPin)) {		
			gpio_set_function(newPin, GPIO_FUNC_PWM);
			sliceNum = pwm_gpio_to_slice_num(newPin);
			channelNum = pwm_gpio_to_channel(newPin);			
			pwm_set_chan_level(sliceNum, channelNum, COINLEDS_PWM_MAXLEVEL);
			pwm_set_enabled(sliceNum, true);
			this->ledPins[i] = newPin;
			this->ready = true;
		}
	}
	
	this->brightness = mBrightness;
	this->maxBrightness = mBrightness;
	this->animation = initAnimation;	
	this->animate();
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
			newState = 0xFF & mask;
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
		if (this->ledPins[i] > 0)
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


//CoinLedsAddon Class

bool CoinLedsAddon::available() {
	const CoinLedsOptions& options = Storage::getInstance().getAddonOptions().coinLedsOptions;
	return options.enabled;
}

void CoinLedsAddon::setup() {	
	const CoinLedsOptions& options = Storage::getInstance().getAddonOptions().coinLedsOptions;	
	int32_t startPins[] = {options.coinLedsStartPin1, options.coinLedsStartPin2, options.coinLedsStartPin3, options.coinLedsStartPin4};
	int32_t coinPins[] = {options.coinLedsCoinPin1, options.coinLedsCoinPin2, options.coinLedsCoinPin3, options.coinLedsCoinPin4};
	int32_t marqueePins[] = {options.coinLedsMarqueePin, -1, -1, -1};
	uint8_t startBrightness = (options.coinLedsStartBrightness >= 100) ? 255 : (float)options.coinLedsStartBrightness / 100 * 255;	
	uint8_t coinBrightness = (options.coinLedsCoinBrightness >= 100) ? 255 : (float)options.coinLedsCoinBrightness / 100 * 255;	
	uint8_t marqueeBrightness = (options.coinLedsMarqueeBrightness >= 100) ? 255 : (float)options.coinLedsMarqueeBrightness / 100 * 255;

	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 4.f);

	this->ledsStart = new CoinLeds(startPins, startBrightness, COINLEDS_ALL_OFF);
	this->ledsCoin = new CoinLeds(coinPins, coinBrightness, COINLEDS_ALL_ON);
	this->ledsMarquee = new CoinLeds(marqueePins, marqueeBrightness, COINLEDS_ALL_ON);
	
	if (isValidPin(options.coinLedsExtStartPinOut))
	{
		this->externalStartPinOut = options.coinLedsExtStartPinOut;
		this->externalStartMask = options.coinLedsExtStartMask;
		gpio_init(this->externalStartPinOut);
    	gpio_set_dir(this->externalStartPinOut, GPIO_OUT);
		gpio_put(this->externalStartPinOut, 1);
	}

	if (isValidPin(options.coinLedsExtCoinPinOut))
	{
		this->externalCoinPinOut = options.coinLedsExtCoinPinOut;
		this->externalCoinMask = options.coinLedsExtCoinMask;
		gpio_init(this->externalCoinPinOut);
    	gpio_set_dir(this->externalCoinPinOut, GPIO_OUT);
		gpio_put(this->externalCoinPinOut, 1);
	}

	this->startMasks = options.coinLedsStartMask1 | options.coinLedsStartMask2 | options.coinLedsStartMask3 | options.coinLedsStartMask4;
	this->coinMasks = options.coinLedsCoinMask1 | options.coinLedsCoinMask2 | options.coinLedsCoinMask3 | options.coinLedsCoinMask4;
	this->allMasks = this->startMasks | this->coinMasks | this->externalStartMask | this->externalCoinMask;

	this->ready = true;
}

void CoinLedsAddon::process()
{
    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	uint32_t buttonsPressed = gamepad->state.buttons & this->allMasks;
	uint32_t dpadPressed = gamepad->state.dpad & GAMEPAD_MASK_DPAD;
	uint32_t newButtonsPressed;

	this->ledsStart->display();
	this->ledsCoin->display();
	this->ledsMarquee->display();

	if (!(buttonsPressed && this->ready))
		return;

	if (dpadPressed && (buttonsPressed & this->coinMasks))
	{
		if (this->debounce(&this->debounceMarqueeBrightness) || !this->ledsMarquee->isReady())
			return;
		if (dpadPressed & GAMEPAD_MASK_UP)
			this->ledsMarquee->brightnessUp();
		if (dpadPressed & GAMEPAD_MASK_DOWN)
			this->ledsMarquee->brightnessDown();
		return;
	}

	newButtonsPressed = buttonsPressed ^ this->lastButtonsPressed;
	if (!newButtonsPressed)
		return;

	this->lastButtonsPressed = buttonsPressed;
	
	if (this->externalStartPinOut > 0 && (newButtonsPressed & this->externalStartMask))
	{
		this->externalStartButtonEnabled = true;
		gpio_put(this->externalStartPinOut, 0);
		
	}
	else if (this->externalStartButtonEnabled)
	{
		this->externalStartButtonEnabled = false;
		gpio_put(this->externalStartPinOut, 1);		
	}

	if (this->externalCoinPinOut > 0 && (newButtonsPressed & this->externalCoinMask))
	{
		this->externalCoinButtonEnabled = true;
		gpio_put(this->externalCoinPinOut, 0);
		
	}
	else if (this->externalCoinButtonEnabled)
	{
		this->externalCoinButtonEnabled = false;
		gpio_put(this->externalCoinPinOut, 1);		
	}	

	if (dpadPressed || !this->ledsStart->isReady() || !this->ledsCoin->isReady())
		return;

	if ((buttonsPressed & this->coinMasks))
	{				
		if (this->creditCount == 0)
		{
			this->ledsStart->setAnimation(COINLEDS_BLINK_FAST_ALL);
			this->ledsCoin->setAnimation(COINLEDS_ALL_ON);
		}
		if (this->creditCount < 0xFF)
			this->creditCount++;
	}
	else if ((buttonsPressed & this->startMasks))
	{
		if (this->creditCount > 0)
			this->creditCount--;				
		if (this->creditCount == 0)
		{
			this->ledsStart->setAnimation(COINLEDS_ALL_OFF);
			this->ledsCoin->setAnimation(COINLEDS_FADE_ALL);
		}
		else
		{
			this->ledsStart->setAnimation(COINLEDS_ALL_ON);
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
