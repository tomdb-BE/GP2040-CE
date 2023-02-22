/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2022 Tom De Backer
 */

#ifndef _STARTLEDS_H_
#define _STARTLEDS_H_

#include "BoardConfig.h"
#include <stdint.h>
#include "AnimationStation.hpp"
#include "gpaddon.h"
#include "helper.h"
#include "GamepadState.h"

#ifndef STARTLEDS_ENABLED
#define STARTLEDS_ENABLED 0
#endif
#ifndef STARTLEDS_COUNT
#define STARTLEDS_COUNT 2
#endif
#ifndef STARTLEDS_MAX_BRIGHTNESS
#define STARTLEDS_MAX_BRIGHTNESS 0xFF
#endif
#ifndef STARTLEDS_PWM_MAXLEVEL
#define STARTLEDS_PWM_MAXLEVEL 0xFFFF
#endif
#ifndef STARTLEDS_START1_BUTTON_MASK
#define STARTLEDS_START1_BUTTON_MASK GAMEPAD_MASK_S2
#endif
#ifndef STARTLEDS_COIN1_BUTTON_MASK
#define STARTLEDS_COIN1_BUTTON_MASK GAMEPAD_MASK_S1
#endif
#ifndef STARTLEDS_START2_BUTTON_MASK
#define STARTLEDS_START2_BUTTON_MASK 0
#endif
#ifndef STARTLEDS_COIN2_BUTTON_MASK
#define STARTLEDS_COIN2_BUTTON_MASK 0
#endif
#ifndef STARTLEDS_START_BUTTON_MASKS
#define STARTLEDS_START_BUTTON_MASKS STARTLEDS_START1_BUTTON_MASK | STARTLEDS_START2_BUTTON_MASK
#endif
#ifndef STARTLEDS_COIN_BUTTON_MASKS
#define STARTLEDS_COIN_BUTTON_MASKS STARTLEDS_COIN1_BUTTON_MASK | STARTLEDS_COIN2_BUTTON_MASK
#endif
#ifndef STARTLEDS_EXT_START_MASK
#define STARTLEDS_EXT_START_MASK GAMEPAD_MASK_A2
#endif
#ifndef STARTLEDS_EXT_COIN_MASK
#define STARTLEDS_EXT_COIN_MASK GAMEPAD_MASK_A1
#endif
#ifndef STARTLEDS_EXT_MASKS
#define STARTLEDS_EXT_MASKS STARTLEDS_EXT_START_MASK | STARTLEDS_EXT_COIN_MASK
#endif
#ifndef STARTLEDS_START_PIN1
#define STARTLEDS_START_PIN1 -1
#endif
#ifndef STARTLEDS_COIN_PIN1
#define STARTLEDS_COIN_PIN1 -1
#endif
#ifndef STARTLEDS_START_PIN2
#define STARTLEDS_START_PIN2 -1
#endif
#ifndef STARTLEDS_COIN_PIN2
#define STARTLEDS_COIN_PIN2 -1
#endif
#ifndef STARTLEDS_MARQUEE_PIN
#define STARTLEDS_MARQUEE_PIN -1
#endif
#ifndef STARTLEDS_EXT_START_PIN
#define STARTLEDS_EXT_START_PIN -1
#endif
#ifndef STARTLEDS_EXT_COIN_PIN
#define STARTLEDS_EXT_COIN_PIN -1
#endif
#ifndef STARTLEDS_START_BRIGHTNESS
#define STARTLEDS_START_BRIGHTNESS 0xFF
#endif
#ifndef STARTLEDS_COIN_BRIGHTNESS
#define STARTLEDS_COIN_BRIGHTNESS 0xFF
#endif
#ifndef STARTLEDS_MARQUEE_BRIGHTNESS
#define STARTLEDS_MARQUEE_BRIGHTNESS 0xFF
#endif
#ifndef STARTLEDS_BRIGHTNESS_STEP
#define STARTLEDS_BRIGHTNESS_STEP 5
#endif
#ifndef STARTLEDS_DEBOUNCE_MILLIS
#define STARTLEDS_DEBOUNCE_MILLIS 50
#endif

static const uint8_t STARTLEDS_STATE_LED1 = (1 << 0);
static const uint8_t STARTLEDS_STATE_LED2 = (1 << 1);
static const uint8_t STARTLEDS_STATE_LED3 = (1 << 2);
static const uint8_t STARTLEDS_STATE_LED4 = (1 << 3);
static const uint8_t STARTLEDS_STATE_ALL_ON  = 0xFF;
static const uint8_t STARTLEDS_STATE_ALL_OFF = 0;

typedef enum : uint8_t
{
	STARTLEDS_ANIM_NONE = 255,
	STARTLEDS_ANIM_OFF = 0,
	STARTLEDS_ANIM_SOLID = 1,
	STARTLEDS_ANIM_BLINK = 2,
	STARTLEDS_ANIM_CYCLE = 3,
	STARTLEDS_ANIM_FADE = 4,
} StartLedsAnimationType;

typedef enum : uint16_t
{
	STARTLEDS_SPEED_OFF       = 0,
	STARTLEDS_SPEED_LUDICROUS = 20,
	STARTLEDS_SPEED_FASTER    = 100,
	STARTLEDS_SPEED_FAST      = 250,
	STARTLEDS_SPEED_NORMAL    = 500,
	STARTLEDS_SPEED_SLOW      = 1000,
} StartLedsAnimationSpeed;

struct StartLedsAnimation
{
	uint8_t currentState = STARTLEDS_STATE_ALL_OFF;		
	uint8_t stateMask = 0xFF;
	uint8_t prevStateMask = 0xFF;
	StartLedsAnimationType currentType = STARTLEDS_ANIM_NONE;
	StartLedsAnimationType previousType = STARTLEDS_ANIM_NONE;
	uint16_t speed = STARTLEDS_SPEED_OFF;
};

static const StartLedsAnimation STARTLEDS_ALL_OFF {
	currentState: STARTLEDS_STATE_ALL_OFF,
	stateMask: 0,
	currentType: STARTLEDS_ANIM_OFF
};

static const StartLedsAnimation STARTLEDS_ALL_ON {
	currentState: STARTLEDS_STATE_ALL_ON,
	currentType: STARTLEDS_ANIM_SOLID
};

static const StartLedsAnimation STARTLEDS_BLINK_FAST_ALL {
	currentState: STARTLEDS_STATE_ALL_OFF,
	currentType: STARTLEDS_ANIM_BLINK,
	speed: STARTLEDS_SPEED_FAST
};

static const StartLedsAnimation STARTLEDS_FADE_ALL {
	currentState: STARTLEDS_STATE_ALL_ON,
	currentType: STARTLEDS_ANIM_FADE,
	speed: STARTLEDS_SPEED_LUDICROUS
};


#define StartLedsAddonName "STARTLEDSADDON"

class StartLeds
{
public:		
	void initAnimation() { this->animation.previousType = STARTLEDS_ANIM_NONE; }
	void setAnimation(StartLedsAnimation newAnimation) { this->animation = newAnimation; this->nextAnimationTime = make_timeout_time_ms(0); }	
	void setAnimationBrightness(uint8_t amount = STARTLEDS_BRIGHTNESS_STEP) { this->handleBrightness(this->maxBrightness, amount); this->maxAnimationBrightness = this->brightness; }	
	uint8_t getAnimationBrightness() {return this->maxAnimationBrightness; }
	bool toggleState() {(this->turnedOff) ? this->turnOn() : this->turnOff(); return !this->turnedOff;}
	bool isReady() {return this->ready; }
	std::vector<uint> initialize(std::vector<uint> slices, uint8_t * pins, uint8_t mBrightness, StartLedsAnimation initAnimation);
	void display();	
private:
	void turnOn() {this->animation.stateMask = this->animation.prevStateMask; this->nextAnimationTime = make_timeout_time_ms(0); this->turnedOff = false; }
	void turnOff() {this->animation.prevStateMask = this->animation.stateMask; this->animation.stateMask = 0; this->nextAnimationTime = make_timeout_time_ms(0); this->turnedOff = true; }
	void handleBrightness(uint8_t mBrightness, uint8_t amount = STARTLEDS_BRIGHTNESS_STEP);
	inline void resetAnimation();
	inline void animate();
	const uint8_t maxBrightness = STARTLEDS_MAX_BRIGHTNESS;
	absolute_time_t nextAnimationTime = make_timeout_time_ms(0);
	uint16_t ledLevels[STARTLEDS_COUNT];
	uint8_t ledPins[STARTLEDS_COUNT];
	uint8_t brightness;	
	uint8_t maxAnimationBrightness;
	StartLedsAnimation animation;
	bool fadeIn;
	bool turnedOff;
	bool ready;
};

class StartLedsAddon : public GPAddon
{
public:
	virtual bool available();
	virtual void setup();
	virtual void preprocess() {}
	virtual void process();
	virtual std::string name() { return StartLedsAddonName; }	
private:
	bool debounce();
	void saveBrightness();
	void processBrightness(uint8_t dpadPressedMask);
	void processExternalButtons(uint16_t buttonsPressedMask);
	void processCredits(uint16_t buttonsPressedMask);
	StartLeds ledsStart;
	StartLeds ledsCoin;
	StartLeds ledsMarquee;
	uint8_t creditCount = 0;
	uint16_t lastButtonsPressed;
	uint16_t lastDpadPressed;
	uint32_t debounceBrightness;
	uint8_t externalStartPin = 0xFF;
	uint8_t externalCoinPin	= 0xFF;
	bool ready;
};

#endif
