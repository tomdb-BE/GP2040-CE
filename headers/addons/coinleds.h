/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2022 Tom De Backer
 */

#ifndef _COINLEDS_H_
#define _COINLEDS_H_

#include "gpaddon.h"
#include "BoardConfig.h"
#include <stdint.h>
#include "AnimationStation.hpp"
#include "helper.h"
#include "GamepadState.h"

#ifndef COINLEDS_ENABLED
#define COINLEDS_ENABLED 0
#endif
#ifndef COINLEDS_COUNT
#define COINLEDS_COUNT 4
#endif
#ifndef COINLEDS_PWM_MAXLEVEL
#define COINLEDS_PWM_MAXLEVEL 0xFFFF
#endif
#ifndef START1_BUTTON_MASK
#define START1_BUTTON_MASK GAMEPAD_MASK_S2
#endif
#ifndef COIN1_BUTTON_MASK
#define COIN1_BUTTON_MASK GAMEPAD_MASK_S1
#endif
#ifndef START2_BUTTON_MASK
#define START2_BUTTON_MASK 0
#endif
#ifndef COIN2_BUTTON_MASK
#define COIN2_BUTTON_MASK 0
#endif
#ifndef START3_BUTTON_MASK
#define START3_BUTTON_MASK 0
#endif
#ifndef COIN3_BUTTON_MASK
#define COIN3_BUTTON_MASK 0
#endif
#ifndef START4_BUTTON_MASK
#define START4_BUTTON_MASK 0
#endif
#ifndef COIN4_BUTTON_MASK
#define COIN4_BUTTON_MASK 0
#endif
#ifndef START_BUTTON_MASKS
#define START_BUTTON_MASKS START1_BUTTON_MASK | START2_BUTTON_MASK | START3_BUTTON_MASK | START4_BUTTON_MASK
#endif
#ifndef COIN_BUTTON_MASKS
#define COIN_BUTTON_MASKS COIN1_BUTTON_MASK | COIN2_BUTTON_MASK | COIN3_BUTTON_MASK | COIN4_BUTTON_MASK
#endif
#ifndef COINLEDS_START_PIN1
#define COINLEDS_START_PIN1 -1
#endif
#ifndef COINLEDS_COIN_PIN1
#define COINLEDS_COIN_PIN1 -1
#endif
#ifndef COINLEDS_START_PIN2
#define COINLEDS_START_PIN2 -1
#endif
#ifndef COINLEDS_COIN_PIN2
#define COINLEDS_COIN_PIN2 -1
#endif
#ifndef COINLEDS_START_PIN3
#define COINLEDS_START_PIN3 -1
#endif
#ifndef COINLEDS_COIN_PIN3
#define COINLEDS_COIN_PIN3 -1
#endif
#ifndef COINLEDS_START_PIN4
#define COINLEDS_START_PIN4 -1
#endif
#ifndef COINLEDS_COIN_PIN4
#define COINLEDS_COIN_PIN4 -1
#endif
#ifndef COINLEDS_MARQUEE_PIN
#define COINLEDS_MARQUEE_PIN -1
#endif
#ifndef COINLEDS_EXT_START_PIN_OUT
#define COINLEDS_EXT_START_PIN_OUT -1
#endif
#ifndef COINLEDS_EXT_COIN_PIN_OUT
#define COINLEDS_EXT_COIN_PIN_OUT -1
#endif
#ifndef COINLEDS_EXT_START_MASK
#define COINLEDS_EXT_START_MASK 0
#endif
#ifndef COINLEDS_EXT_COIN_MASK
#define COINLEDS_EXT_COIN_MASK 0
#endif
#ifndef COINLEDS_BRIGHTNESS_STEP
#define COINLEDS_BRIGHTNESS_STEP 5
#endif
#ifndef COINLEDS_DEBOUNCE_MILLIS
#define COINLEDS_DEBOUNCE_MILLIS 50
#endif

static const uint8_t COINLEDS_STATE_LED1 = (1 << 0);
static const uint8_t COINLEDS_STATE_LED2 = (1 << 1);
static const uint8_t COINLEDS_STATE_LED3 = (1 << 2);
static const uint8_t COINLEDS_STATE_LED4 = (1 << 3);
static const uint8_t COINLEDS_STATE_ALL_ON  = 0xFF;
static const uint8_t COINLEDS_STATE_ALL_OFF = 0;

typedef enum : uint8_t
{
	COINLEDS_ANIM_NONE = 255,
	COINLEDS_ANIM_OFF = 0,
	COINLEDS_ANIM_SOLID = 1,
	COINLEDS_ANIM_BLINK = 2,
	COINLEDS_ANIM_CYCLE = 3,
	COINLEDS_ANIM_FADE = 4,
} CoinLedsAnimationType;

typedef enum : uint16_t
{
	COINLEDS_SPEED_OFF       = 0,
	COINLEDS_SPEED_LUDICROUS = 20,
	COINLEDS_SPEED_FASTER    = 100,
	COINLEDS_SPEED_FAST      = 250,
	COINLEDS_SPEED_NORMAL    = 500,
	COINLEDS_SPEED_SLOW      = 1000,
} CoinLedsAnimationSpeed;

struct CoinLedsAnimation
{
	uint8_t currentState = COINLEDS_STATE_ALL_OFF;		
	uint8_t stateMask = 0xFF;
	CoinLedsAnimationType currentType = COINLEDS_ANIM_NONE;
	CoinLedsAnimationType previousType = COINLEDS_ANIM_NONE;
	CoinLedsAnimationSpeed speed = COINLEDS_SPEED_OFF;
};

static const CoinLedsAnimation COINLEDS_ALL_OFF {
	currentState: COINLEDS_STATE_ALL_OFF,
	stateMask: 0,
	currentType: COINLEDS_ANIM_OFF
};

static const CoinLedsAnimation COINLEDS_ALL_ON {
	currentState: COINLEDS_STATE_ALL_ON,
	currentType: COINLEDS_ANIM_SOLID
};

static const CoinLedsAnimation COINLEDS_BLINK_FAST_ALL {
	currentState: COINLEDS_STATE_ALL_OFF,
	currentType: COINLEDS_ANIM_BLINK,
	speed: COINLEDS_SPEED_FAST
};

static const CoinLedsAnimation COINLEDS_FADE_ALL {
	currentState: COINLEDS_STATE_ALL_ON,
	currentType: COINLEDS_ANIM_FADE,
	speed: COINLEDS_SPEED_LUDICROUS
};


#define CoinLedsAddonName "COINLEDSADDON"

class CoinLeds
{
public:	
	std::vector<uint> initialize(std::vector<uint> slices, int32_t * pins, uint8_t mBrightness, CoinLedsAnimation initAnimation);
	void display();
	void update() {this->nextAnimationTime = make_timeout_time_ms(0);}
	void setAnimation(CoinLedsAnimation newAnimation) { this->animation = newAnimation; this->update(); }
	void setType(CoinLedsAnimationType newType) { this->animation.currentType = newType; }
	void setSpeed(CoinLedsAnimationSpeed newSpeed) { this->animation.speed = newSpeed; this->update(); }
	void setMask(uint8_t newMask) { this->animation.stateMask = newMask; this->update(); }
	void brightnessUp(uint8_t amount = COINLEDS_BRIGHTNESS_STEP) { this->brightness = this->handleBrightness(amount, true); }
	void brightnessDown(uint8_t amount = COINLEDS_BRIGHTNESS_STEP) { this->brightness = this->handleBrightness(amount); }
	CoinLedsAnimation getAnimation() { return this->animation; }
	CoinLedsAnimationSpeed getSpeed() { return this->animation.speed; }
	CoinLedsAnimationType getType() { return this->animation.currentType; }
	uint8_t getMask() { return this->animation.stateMask; }
	bool isReady() {return this->ready;}
private:
	inline void animate();
	inline void reset();
	uint8_t handleBrightness(uint8_t amount, bool negative = false);	
	absolute_time_t nextAnimationTime = make_timeout_time_ms(0);
	uint16_t ledLevels[COINLEDS_COUNT];
	int32_t ledPins[COINLEDS_COUNT];
	uint8_t brightness = 0;
	uint8_t maxBrightness = 0;
	CoinLedsAnimation animation;
	bool fadeIn = false;
	bool ready = false;
};

class CoinLedsAddon : public GPAddon
{
public:
	virtual bool available();
	virtual void setup();
	virtual void preprocess() {}
	virtual void process();
	virtual std::string name() { return CoinLedsAddonName; }	
private:	
	CoinLeds ledsStart;
	CoinLeds ledsCoin;
	CoinLeds ledsMarquee;
	bool debounce(uint32_t * ptrDebounceTime);
	bool externalStartButtonEnabled = false;
	bool externalCoinButtonEnabled = false;
	uint8_t creditCount = 0;	
	uint32_t debounceMarqueeBrightness = 0;	
	uint32_t lastButtonsPressed = 0;
	uint32_t allMasks = 0;
	uint32_t startMasks = 0;
	uint32_t coinMasks = 0;	
	uint32_t externalStartMask = COINLEDS_EXT_START_MASK;
	uint32_t externalCoinMask = COINLEDS_EXT_COIN_MASK;
	int32_t externalStartPinOut = COINLEDS_EXT_START_PIN_OUT;	
	int32_t externalCoinPinOut = COINLEDS_EXT_COIN_PIN_OUT;
	bool ready = false;
};

#endif
