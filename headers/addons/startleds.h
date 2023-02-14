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

#ifndef START1_BUTTON_MASK
#define START1_BUTTON_MASK GAMEPAD_MASK_S2
#endif
#ifndef COIN1_BUTTON_MASK
#define COIN1_BUTTON_MASK GAMEPAD_MASK_S1
#endif

#define STARTLED_COUNT 4
#define STARTLED_DELAY_START 1000

#define STARTLED_MAX_START_BRIGHTNESS 0xFF
#define STARTLED_MAX_START_LEVEL 0xFFFF
#define STARTLED_MAX_COIN_BRIGHTNESS 0xFF
#define STARTLED_MAX_COIN_LEVEL 0xFFFF
#define STARTLED_INIT_START_STATE 0
#define STARTLED_INIT_COIN_STATE 1

#ifndef STARTLED_START1_PIN
#define STARTLED_START1_PIN -1
#endif
#ifndef STARTLED_START2_PIN
#define STARTLED_START2_PIN -1
#endif
#ifndef STARTLED_START3_PIN
#define STARTLED_START3_PIN -1
#endif
#ifndef STARTLED_START4_PIN
#define STARTLED_START4_PIN -1
#endif
#ifndef STARTLED_COIN1_PIN
#define STARTLED_COIN1_PIN -1
#endif
#ifndef STARTLED_COIN2_PIN
#define STARTLED_COIN2_PIN -1
#endif
#ifndef STARTLED_COIN3_PIN
#define STARTLED_COIN3_PIN -1
#endif
#ifndef STARTLED_COIN4_PIN
#define STARTLED_COIN4_PIN -1
#endif

#define STARTLED_MASK_ALL ((1U << STARTLED_START1_PIN) | (1U << STARTLED_START2_PIN) | (1U << STARTLED_START3_PIN) | (1U << STARTLED_START4_PIN) | (1U << STARTLED_COIN1_PIN) | (1U << STARTLED_COIN2_PIN) | (1U << STARTLED_COIN3_PIN) | (1U << STARTLED_COIN4_PIN))

const int STARTLED_START_PINS[] = {STARTLED_START1_PIN, STARTLED_START2_PIN, STARTLED_START3_PIN, STARTLED_START4_PIN};
const int STARTLED_COIN_PINS[] = {STARTLED_COIN1_PIN, STARTLED_COIN2_PIN, STARTLED_COIN3_PIN, STARTLED_COIN4_PIN};

typedef enum
{
	STARTLED_TYPE_NONE = -1,
	STARTLED_TYPE_PWM = 0,	
} StartLEDType;

typedef enum
{
	STARTLED_STATE_LED1 = (1 << 0),
	STARTLED_STATE_LED2 = (1 << 1),
	STARTLED_STATE_LED3 = (1 << 2),
	STARTLED_STATE_LED4 = (1 << 3),
} StartLEDStateMask;

typedef enum
{
	STARTLED_ANIM_NONE,
	STARTLED_ANIM_OFF,
	STARTLED_ANIM_SOLID,
	STARTLED_ANIM_BLINK,
	STARTLED_ANIM_FADE,
} StartLEDAnimationType;

const StartLEDAnimationType STARTLED_ANIMATION_TYPES[] =
{
	STARTLED_ANIM_NONE,
	STARTLED_ANIM_OFF,
	STARTLED_ANIM_SOLID,
	STARTLED_ANIM_BLINK,
	STARTLED_ANIM_FADE,
};

typedef enum
{
	STARTLED_SPEED_OFF       = 0,
	STARTLED_SPEED_LUDICROUS = 20,
	STARTLED_SPEED_FASTER    = 100,
	STARTLED_SPEED_FAST      = 250,
	STARTLED_SPEED_NORMAL    = 500,
	STARTLED_SPEED_SLOW      = 1000,
} StartLEDAnimationSpeed;

const StartLEDAnimationSpeed STARTLED_ANIMATION_SPEEDS[] =
{
	STARTLED_SPEED_OFF,
	STARTLED_SPEED_LUDICROUS,
	STARTLED_SPEED_FASTER,
	STARTLED_SPEED_FAST,
	STARTLED_SPEED_NORMAL,
	STARTLED_SPEED_SLOW,
};

struct StartLEDAnimationState
{
	uint8_t state = 0;
	StartLEDAnimationType animation;
	StartLEDAnimationSpeed speed;
};

class StartLEDs
{
public:
	void setup(const int * pins, bool istate, uint64_t mBrightness, uint64_t mLevel);
	void display();
	void animate(StartLEDAnimationState animationState);	

protected:
	void parseState(uint8_t state)
	{
		memcpy(this->lastLedState, this->currentLedState, sizeof(this->currentLedState));
		for (int i = 0; i < STARTLED_COUNT; i++)
			this->currentLedState[i] = (state & (1 << i)) == (1 << i);
	}

	inline void reset()
	{
		memset(this->lastLedState, this->initialState, sizeof(this->lastLedState));
		memset(this->currentLedState, this->initialState, sizeof(this->currentLedState));
		this->nextAnimationTime = get_absolute_time();
		this->brightness = this->maxBrightness;
		this->fadeIn = false;
	}

	inline void handleBlink(StartLEDAnimationSpeed speed)
	{
		for (int i = 0; i < STARTLED_COUNT; i++)
		{
			if (this->lastLedState[i])
				this->currentLedState[i] = false;
		}
		this->nextAnimationTime = make_timeout_time_ms(speed);		
	}

	inline void handleFade()
	{
		if (this->fadeIn)
		{
			this->brightness += 5;
			if (this->brightness == this->maxBrightness)
				this->fadeIn = false;
		}
		else
		{
			this->brightness -= 5;
			if (this->brightness == 0)
				this->fadeIn= true;
		}

		this->nextAnimationTime = make_timeout_time_ms(STARTLED_SPEED_LUDICROUS);
	}
	
	const int * ledPins;	
	absolute_time_t nextAnimationTime;	
	StartLEDAnimationType selectedAnimation = STARTLED_ANIM_NONE;
	uint16_t ledLevels[STARTLED_COUNT];
	bool lastLedState[STARTLED_COUNT] = { };
	bool currentLedState[STARTLED_COUNT] = { };
	uint64_t brightness;
	uint64_t maxBrightness;
	uint64_t maxLevel;
	bool fadeIn = false;
	bool initialState = false;
};

#define StartLEDName "STARTLED"

// Start LED Addon
class StartLEDAddon : public GPAddon
{
public:
	virtual bool available();  // GPAddon
	virtual void setup();
	virtual void process();
	virtual std::string name() { return StartLEDName; }
	StartLEDAddon() : type(STARTLED_TYPE) { }
	StartLEDAddon(StartLEDType type) : type(type) { }
	void SetAnimationStart(StartLEDStateMask buttonState, StartLEDAnimationType animationType, StartLEDAnimationSpeed animationSpeed = STARTLED_SPEED_OFF);
	void SetAnimationCoin(StartLEDStateMask buttonState, StartLEDAnimationType animationType, StartLEDAnimationSpeed animationSpeed = STARTLED_SPEED_OFF);

protected:
	StartLEDs * ledsStart = nullptr;
	StartLEDs * ledsCoin = nullptr;
	StartLEDAnimationState animationStateStart;
	StartLEDAnimationState animationStateCoin;
	StartLEDType type;
	bool lastStartPressed[STARTLED_COUNT];
	bool lastCoinPressed[STARTLED_COUNT];
	uint8_t creditCount = 0;
	absolute_time_t nextButtonCheckTime;
};

#endif
