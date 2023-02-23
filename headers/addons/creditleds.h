/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2022 Tom De Backer
 */

#ifndef _CREDITLEDS_H_
#define _CREDITLEDS_H_

#include "BoardConfig.h"
#include <stdint.h>
#include "AnimationStation.hpp"
#include "gpaddon.h"
#include "helper.h"
#include "GamepadState.h"

#ifndef CREDITLEDS_ENABLED
#define CREDITLEDS_ENABLED 0
#endif
#ifndef CREDITLEDS_COUNT
#define CREDITLEDS_COUNT 2
#endif
#ifndef CREDITLEDS_MAX_BRIGHTNESS
#define CREDITLEDS_MAX_BRIGHTNESS 0xFF
#endif
#ifndef CREDITLEDS_PWM_MAXLEVEL
#define CREDITLEDS_PWM_MAXLEVEL 0xFFFF
#endif
#ifndef CREDITLEDS_START1_BUTTON_MASK
#define CREDITLEDS_START1_BUTTON_MASK GAMEPAD_MASK_S2
#endif
#ifndef CREDITLEDS_COIN1_BUTTON_MASK
#define CREDITLEDS_COIN1_BUTTON_MASK GAMEPAD_MASK_S1
#endif
#ifndef CREDITLEDS_START2_BUTTON_MASK
#define CREDITLEDS_START2_BUTTON_MASK 0
#endif
#ifndef CREDITLEDS_COIN2_BUTTON_MASK
#define CREDITLEDS_COIN2_BUTTON_MASK 0
#endif
#ifndef CREDITLEDS_START_BUTTON_MASKS
#define CREDITLEDS_START_BUTTON_MASKS CREDITLEDS_START1_BUTTON_MASK | CREDITLEDS_START2_BUTTON_MASK
#endif
#ifndef CREDITLEDS_COIN_BUTTON_MASKS
#define CREDITLEDS_COIN_BUTTON_MASKS CREDITLEDS_COIN1_BUTTON_MASK | CREDITLEDS_COIN2_BUTTON_MASK
#endif
#ifndef CREDITLEDS_EXT_START_MASK
#define CREDITLEDS_EXT_START_MASK GAMEPAD_MASK_A2
#endif
#ifndef CREDITLEDS_EXT_COIN_MASK
#define CREDITLEDS_EXT_COIN_MASK GAMEPAD_MASK_A1
#endif
#ifndef CREDITLEDS_EXT_MASKS
#define CREDITLEDS_EXT_MASKS CREDITLEDS_EXT_START_MASK | CREDITLEDS_EXT_COIN_MASK
#endif
#ifndef CREDITLEDS_START_PIN1
#define CREDITLEDS_START_PIN1 -1
#endif
#ifndef CREDITLEDS_COIN_PIN1
#define CREDITLEDS_COIN_PIN1 -1
#endif
#ifndef CREDITLEDS_START_PIN2
#define CREDITLEDS_START_PIN2 -1
#endif
#ifndef CREDITLEDS_COIN_PIN2
#define CREDITLEDS_COIN_PIN2 -1
#endif
#ifndef CREDITLEDS_MARQUEE_PIN
#define CREDITLEDS_MARQUEE_PIN -1
#endif
#ifndef CREDITLEDS_EXT_START_PIN
#define CREDITLEDS_EXT_START_PIN -1
#endif
#ifndef CREDITLEDS_EXT_COIN_PIN
#define CREDITLEDS_EXT_COIN_PIN -1
#endif
#ifndef CREDITLEDS_START_BRIGHTNESS
#define CREDITLEDS_START_BRIGHTNESS 0xFF
#endif
#ifndef CREDITLEDS_COIN_BRIGHTNESS
#define CREDITLEDS_COIN_BRIGHTNESS 0xFF
#endif
#ifndef CREDITLEDS_MARQUEE_BRIGHTNESS
#define CREDITLEDS_MARQUEE_BRIGHTNESS 0xFF
#endif
#ifndef CREDITLEDS_BRIGHTNESS_STEP
#define CREDITLEDS_BRIGHTNESS_STEP 5
#endif
#ifndef CREDITLEDS_DEBOUNCE_MILLIS
#define CREDITLEDS_DEBOUNCE_MILLIS 50
#endif

static const uint8_t CREDITLEDS_STATE_LED1 = (1 << 0);
static const uint8_t CREDITLEDS_STATE_LED2 = (1 << 1);
static const uint8_t CREDITLEDS_STATE_LED3 = (1 << 2);
static const uint8_t CREDITLEDS_STATE_LED4 = (1 << 3);
static const uint8_t CREDITLEDS_STATE_ALL_ON  = 0xFF;
static const uint8_t CREDITLEDS_STATE_ALL_OFF = 0;

typedef enum : uint8_t
{
	CREDITLEDS_ANIM_NONE = 255,
	CREDITLEDS_ANIM_OFF = 0,
	CREDITLEDS_ANIM_SOLID = 1,
	CREDITLEDS_ANIM_BLINK = 2,
	CREDITLEDS_ANIM_CYCLE = 3,
	CREDITLEDS_ANIM_FADE = 4,
} CreditLedsAnimationType;

typedef enum : uint16_t
{
	CREDITLEDS_SPEED_OFF       = 0,
	CREDITLEDS_SPEED_LUDICROUS = 20,
	CREDITLEDS_SPEED_FASTER    = 100,
	CREDITLEDS_SPEED_FAST      = 250,
	CREDITLEDS_SPEED_NORMAL    = 500,
	CREDITLEDS_SPEED_SLOW      = 1000,
} CreditLedsAnimationSpeed;

struct CreditLedsAnimation
{
	uint8_t currentState = CREDITLEDS_STATE_ALL_OFF;		
	uint8_t stateMask = 0xFF;
	uint8_t prevStateMask = 0xFF;
	CreditLedsAnimationType currentType = CREDITLEDS_ANIM_NONE;
	CreditLedsAnimationType previousType = CREDITLEDS_ANIM_NONE;
	uint16_t speed = CREDITLEDS_SPEED_OFF;
};

static const CreditLedsAnimation CREDITLEDS_ALL_OFF {
	currentState: CREDITLEDS_STATE_ALL_OFF,
	stateMask: 0,
	currentType: CREDITLEDS_ANIM_OFF
};

static const CreditLedsAnimation CREDITLEDS_ALL_ON {
	currentState: CREDITLEDS_STATE_ALL_ON,
	currentType: CREDITLEDS_ANIM_SOLID
};

static const CreditLedsAnimation CREDITLEDS_BLINK_FAST_ALL {
	currentState: CREDITLEDS_STATE_ALL_OFF,
	currentType: CREDITLEDS_ANIM_BLINK,
	speed: CREDITLEDS_SPEED_FAST
};

static const CreditLedsAnimation CREDITLEDS_FADE_ALL {
	currentState: CREDITLEDS_STATE_ALL_ON,
	currentType: CREDITLEDS_ANIM_FADE,
	speed: CREDITLEDS_SPEED_LUDICROUS
};


#define CreditLedsAddonName "CREDITLEDSADDON"

class CreditLeds
{
public:		
	void initAnimation() { this->animation.previousType = CREDITLEDS_ANIM_NONE; }
	void setAnimation(CreditLedsAnimation newAnimation) { this->animation = newAnimation; this->nextAnimationTime = make_timeout_time_ms(0); }	
	void setAnimationBrightness(uint8_t amount = CREDITLEDS_BRIGHTNESS_STEP) { this->handleBrightness(this->maxBrightness, amount); this->maxAnimationBrightness = this->brightness; }	
	uint8_t getAnimationBrightness() {return this->maxAnimationBrightness; }
	bool toggleState() {(this->turnedOff) ? this->turnOn() : this->turnOff(); return !this->turnedOff;}
	bool isReady() {return this->ready; }
	std::vector<uint> initialize(std::vector<uint> slices, uint8_t * pins, uint8_t mBrightness, CreditLedsAnimation initAnimation);
	void display();	
private:
	void turnOn() {this->animation.stateMask = this->animation.prevStateMask; this->nextAnimationTime = make_timeout_time_ms(0); this->turnedOff = false; }
	void turnOff() {this->animation.prevStateMask = this->animation.stateMask; this->animation.stateMask = 0; this->nextAnimationTime = make_timeout_time_ms(0); this->turnedOff = true; }
	void handleBrightness(uint8_t mBrightness, uint8_t amount = CREDITLEDS_BRIGHTNESS_STEP);
	inline void resetAnimation();
	inline void animate();
	const uint8_t maxBrightness = CREDITLEDS_MAX_BRIGHTNESS;
	absolute_time_t nextAnimationTime = make_timeout_time_ms(0);
	uint16_t ledLevels[CREDITLEDS_COUNT];
	uint8_t ledPins[CREDITLEDS_COUNT];
	uint8_t brightness;	
	uint8_t maxAnimationBrightness;
	CreditLedsAnimation animation;
	bool fadeIn;
	bool turnedOff;
	bool ready;
};

class CreditLedsAddon : public GPAddon
{
public:
	virtual bool available();
	virtual void setup();
	virtual void preprocess() {}
	virtual void process();
	virtual std::string name() { return CreditLedsAddonName; }	
private:
	bool debounce();
	void saveBrightness();
	void processBrightness(uint8_t dpadPressedMask);
	void processExternalButtons(uint16_t buttonsPressedMask);
	void processCredits(uint16_t buttonsPressedMask);
	CreditLeds ledsStart;
	CreditLeds ledsCoin;
	CreditLeds ledsMarquee;
	uint8_t creditCount = 0;
	uint16_t lastButtonsPressed;
	uint16_t lastDpadPressed;
	uint32_t debounceBrightness;
	uint8_t externalStartPin = 0xFF;
	uint8_t externalCoinPin	= 0xFF;
	bool ready;
};

#endif
