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
#ifndef STARTLEDS_PWM_MAXLEVEL
#define STARTLEDS_PWM_MAXLEVEL 0xFFFF
#endif
#ifndef START1_BUTTON_MASK
#define START1_BUTTON_MASK GAMEPAD_MASK_S2
#endif
#ifndef COIN1_BUTTON_MASK
#define COIN1_BUTTON_MASK GAMEPAD_MASK_S1
#endif
#ifndef STARTLEDS_COUNT
#define STARTLEDS_COUNT 4
#endif
#ifndef STARTLEDS_INIT_START_STATE
#define STARTLEDS_INIT_START_STATE 0
#endif
#ifndef STARTLEDS_INIT_COIN_STATE
#define STARTLEDS_INIT_COIN_STATE 1
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
#ifndef STARTLEDS_START_PIN3
#define STARTLEDS_START_PIN3 -1
#endif
#ifndef STARTLEDS_COIN_PIN3
#define STARTLEDS_COIN_PIN3 -1
#endif
#ifndef STARTLEDS_START_PIN4
#define STARTLEDS_START_PIN4 -1
#endif
#ifndef STARTLEDS_COIN_PIN4
#define STARTLEDS_COIN_PIN4 -1
#endif
#ifndef STARTLEDS_MARQUEE_PIN
#define STARTLEDS_MARQUEE_PIN -1
#endif
#ifndef STARTLEDS_START_BRIGHTNESS
#define STARTLEDS_START_BRIGHTNESS 50
#endif
#ifndef STARTLEDS_COIN_BRIGHTNESS
#define STARTLEDS_COIN_BRIGHTNESS 50
#endif
#ifndef STARTLEDS_MARQUEE_BRIGHTNESS
#define STARTLEDS_MARQUEE_BRIGHTNESS 50
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
	uint8_t state = STARTLEDS_STATE_ALL_OFF;	
	uint8_t previousState = STARTLEDS_STATE_ALL_OFF;
	uint8_t stateMask = 0xFF;
	StartLedsAnimationType type = STARTLEDS_ANIM_NONE;
	StartLedsAnimationType previousType = STARTLEDS_ANIM_NONE;
	StartLedsAnimationSpeed speed = STARTLEDS_SPEED_OFF;
};

static const StartLedsAnimation STARTLEDS_ALL_OFF {
	state: STARTLEDS_STATE_ALL_OFF,
	type: STARTLEDS_ANIM_OFF
};

static const StartLedsAnimation STARTLEDS_ALL_ON {
	state: STARTLEDS_STATE_ALL_ON,
	type: STARTLEDS_ANIM_SOLID
};

static const StartLedsAnimation STARTLEDS_BLINK_FAST_ALL {
	state: STARTLEDS_STATE_ALL_OFF,
	type: STARTLEDS_ANIM_BLINK,
	speed: STARTLEDS_SPEED_FAST
};

static const StartLedsAnimation STARTLEDS_FADE_ALL {
	state: STARTLEDS_STATE_ALL_ON,
	type: STARTLEDS_ANIM_FADE,
	speed: STARTLEDS_SPEED_LUDICROUS
};


class StartLeds
{
public:
	void setup(const uint8_t * pins, uint8_t mBrightness);
	void display();
	void animate();	
	StartLedsAnimation animation;
protected:
	inline void reset();
	inline uint8_t HandleFade(int16_t nBrightness, int16_t mBrightness);
	const uint8_t * ledPins;	
	absolute_time_t nextAnimationTime = get_absolute_time();
	uint16_t ledLevels[STARTLEDS_COUNT];
	uint8_t brightness;
	uint8_t maxBrightness;	
	bool fadeIn;
};

#define StartLedsName "STARTLEDS"

// Start LED Addon
class StartLedsAddon : public GPAddon
{
public:
	virtual bool available();  // GPAddon
	virtual void setup();
	virtual void preprocess() {}
	virtual void process();
	virtual std::string name() { return StartLedsName; }
protected:
	StartLeds * ledsStart = nullptr;
	StartLeds * ledsCoin = nullptr;
	StartLeds * ledsMarquee = nullptr;	
	bool lastStartPressed[STARTLEDS_COUNT];
	bool lastCoinPressed[STARTLEDS_COUNT];
	uint8_t creditCount = 0;
	absolute_time_t nextButtonCheckTime;
};

#endif
