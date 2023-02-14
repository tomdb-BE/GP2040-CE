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

#define STARTLEDS_COUNT 4
#define STARTLEDS_DELAY_START 1000

#define STARTLEDS_MAX_START_BRIGHTNESS 0xFF
#define STARTLEDS_MAX_START_LEVEL 0xFFFF
#define STARTLEDS_MAX_COIN_BRIGHTNESS 0xFF
#define STARTLEDS_MAX_COIN_LEVEL 0xFFFF
#define STARTLEDS_MAX_MARQUEE_BRIGHTNESS 0xFF
#define STARTLEDS_MAX_MARQUEE_LEVEL 0xFFFF
#define STARTLEDS_INIT_START_STATE 0
#define STARTLEDS_INIT_COIN_STATE 1

#ifndef STARTLEDS_START_PIN1
#define STARTLEDS_START_PIN1 -1
#endif
#ifndef STARTLEDS_START_PIN2
#define STARTLEDS_START_PIN2 -1
#endif
#ifndef STARTLEDS_START_PIN3
#define STARTLEDS_START_PIN3 -1
#endif
#ifndef STARTLEDS_START_PIN4
#define STARTLEDS_START_PIN4 -1
#endif
#ifndef STARTLEDS_COIN_PIN1
#define STARTLEDS_COIN_PIN1 -1
#endif
#ifndef STARTLEDS_COIN_PIN2
#define STARTLEDS_COIN_PIN2 -1
#endif
#ifndef STARTLEDS_COIN_PIN3
#define STARTLEDS_COIN_PIN3 -1
#endif
#ifndef STARTLEDS_COIN_PIN4
#define STARTLEDS_COIN_PIN4 -1
#endif
#ifndef STARTLEDS_MARQUEE_PIN
#define STARTLEDS_MARQUEE_PIN -1
#endif

#define STARTLEDS_MASK_ALL ((1U << STARTLEDS_START_PIN1) | (1U << STARTLEDS_START_PIN2) | (1U << STARTLEDS_START_PIN3) | (1U << STARTLEDS_START_PIN4) | (1U << STARTLEDS_COIN_PIN1) | (1U << STARTLEDS_COIN_PIN2) | (1U << STARTLEDS_COIN_PIN3) | (1U << STARTLEDS_COIN_PIN4))

const int STARTLEDS_START_PINS[] = {STARTLEDS_START_PIN1, STARTLEDS_START_PIN2, STARTLEDS_START_PIN3, STARTLEDS_START_PIN4};
const int STARTLEDS_COIN_PINS[] = {STARTLEDS_COIN_PIN1, STARTLEDS_COIN_PIN2, STARTLEDS_COIN_PIN3, STARTLEDS_COIN_PIN4};

typedef enum
{
	STARTLEDS_TYPE_NONE = -1,
	STARTLEDS_TYPE_PWM = 0,	
} StartLedsType;

typedef enum
{
	STARTLEDS_STATE_LED1 = (1 << 0),
	STARTLEDS_STATE_LED2 = (1 << 1),
	STARTLEDS_STATE_LED3 = (1 << 2),
	STARTLEDS_STATE_LED4 = (1 << 3),
} StartLedsStateMask;

typedef enum
{
	STARTLEDS_ANIM_NONE,
	STARTLEDS_ANIM_OFF,
	STARTLEDS_ANIM_SOLID,
	STARTLEDS_ANIM_BLINK,
	STARTLEDS_ANIM_FADE,
} StartLedsAnimationType;

const StartLedsAnimationType STARTLEDS_ANIMATION_TYPES[] =
{
	STARTLEDS_ANIM_NONE,
	STARTLEDS_ANIM_OFF,
	STARTLEDS_ANIM_SOLID,
	STARTLEDS_ANIM_BLINK,
	STARTLEDS_ANIM_FADE,
};

typedef enum
{
	STARTLEDS_SPEED_OFF       = 0,
	STARTLEDS_SPEED_LUDICROUS = 20,
	STARTLEDS_SPEED_FASTER    = 100,
	STARTLEDS_SPEED_FAST      = 250,
	STARTLEDS_SPEED_NORMAL    = 500,
	STARTLEDS_SPEED_SLOW      = 1000,
} StartLedsAnimationSpeed;

const StartLedsAnimationSpeed STARTLEDS_ANIMATION_SPEEDS[] =
{
	STARTLEDS_SPEED_OFF,
	STARTLEDS_SPEED_LUDICROUS,
	STARTLEDS_SPEED_FASTER,
	STARTLEDS_SPEED_FAST,
	STARTLEDS_SPEED_NORMAL,
	STARTLEDS_SPEED_SLOW,
};

struct StartLedsAnimationState
{
	uint8_t state = 0;
	StartLedsAnimationType animation;
	StartLedsAnimationSpeed speed;
};

class StartLeds
{
public:
	void setup(const int * pins, bool istate, uint64_t mBrightness, uint64_t mLevel);
	void display();
	void animate(StartLedsAnimationState animationState);	

protected:
	void parseState(uint8_t state)
	{
		memcpy(this->lastLedState, this->currentLedState, sizeof(this->currentLedState));
		for (int i = 0; i < STARTLEDS_COUNT; i++)
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

	inline void handleBlink(StartLedsAnimationSpeed speed)
	{
		for (int i = 0; i < STARTLEDS_COUNT; i++)
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

		this->nextAnimationTime = make_timeout_time_ms(STARTLEDS_SPEED_LUDICROUS);
	}
	
	const int * ledPins;	
	absolute_time_t nextAnimationTime;	
	StartLedsAnimationType selectedAnimation = STARTLEDS_ANIM_NONE;
	uint16_t ledLevels[STARTLEDS_COUNT];
	bool lastLedState[STARTLEDS_COUNT] = { };
	bool currentLedState[STARTLEDS_COUNT] = { };
	uint64_t brightness;
	uint64_t maxBrightness;
	uint64_t maxLevel;
	bool fadeIn = false;
	bool initialState = false;
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
	StartLedsAddon() : type(STARTLEDS_TYPE) { }
	void SetAnimationStart(StartLedsStateMask buttonState, StartLedsAnimationType animationType, StartLedsAnimationSpeed animationSpeed = STARTLEDS_SPEED_OFF);
	void SetAnimationCoin(StartLedsStateMask buttonState, StartLedsAnimationType animationType, StartLedsAnimationSpeed animationSpeed = STARTLEDS_SPEED_OFF);

protected:
	StartLeds * ledsStart = nullptr;
	StartLeds * ledsCoin = nullptr;
	StartLeds * ledsMarquee = nullptr;
	StartLedsAnimationState animationStateStart;
	StartLedsAnimationState animationStateCoin;
	StartLedsAnimationState animationStateMarquee;
	StartLedsType type;
	bool lastStartPressed[STARTLEDS_COUNT];
	bool lastCoinPressed[STARTLEDS_COUNT];
	uint8_t creditCount = 0;
	absolute_time_t nextButtonCheckTime;
};

#endif
