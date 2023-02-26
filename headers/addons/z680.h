/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2022 Tom De Backer
 */

#ifndef _Z680_H_
#define _Z680_H_

#include "gpaddon.h"

#ifndef Z680_ENABLED
#define Z680_ENABLED 0
#endif
#ifndef Z680_POWER_PIN
#define Z680_POWER_PIN -1
#endif
#ifndef Z680_STATE_PIN
#define Z680_STATE_PIN -1
#endif
#ifndef Z680_VOLUME_UP_PIN
#define Z680_VOLUME_UP_PIN -1
#endif
#ifndef Z680_VOLUME_DOWN_PIN
#define Z680_VOLUME_DOWN_PIN -1
#endif
#ifndef Z680_MUTE_PIN
#define Z680_MUTE_PIN -1
#endif
#ifndef Z680_VOLUME_STEP
#define Z680_VOLUME_STEP 5
#endif
#ifndef Z680_CONTROL_TRIGGER
#define Z680_CONTROL_TRIGGER GAMEPAD_MASK_S1
#endif
#ifndef Z680_CONTROL_POWER
#define Z680_CONTROL_POWER GAMEPAD_MASK_LEFT
#endif
#ifndef Z680_CONTROL_MUTE
#define Z680_CONTROL_MUTE GAMEPAD_MASK_RIGHT
#endif
#ifndef Z680_CONTROL_VOLUME_UP
#define Z680_CONTROL_VOLUME_UP GAMEPAD_MASK_UP
#endif
#ifndef Z680_CONTROL_VOLUME_DOWN
#define Z680_CONTROL_VOLUME_DOWN GAMEPAD_MASK_DOWN
#endif
#ifndef Z680_DEBOUNCE_MILLIS
#define Z680_DEBOUNCE_MILLIS 50
#endif

#define Z680Name "Z680"

// Start Z680 Addon
class Z680Addon : public GPAddon
{
public:
	static Z680Addon& getInstance()
	{
		static Z680Addon instance;
		return instance;
	}
	bool available();
	void setup();
	void preprocess() {}
	void process();
	std::string name() { return Z680Name; }
    bool isReady() {return this->_ready; }
	void volumeUp(uint8_t amount = Z680_VOLUME_STEP) { this->handeVolume(amount, true); }
	void volumeDown(uint8_t amount = Z680_VOLUME_STEP) { this->handeVolume(amount); }
    void powerOn() {if (!this->isOn(false)) this->togglePower(false); }
    void powerOff() {if (this->isOn(true)) this->togglePower(true); }
    void mute();    
    bool isOn(bool setOff = false);
    bool togglePower(bool setOff = false);    
protected:    
    void handeVolume(uint8_t amount, bool up = false);
    bool debounce();
    uint32_t _debounceVolume;    
	uint8_t _pinPower = 0xFF;
    uint8_t _pinVolumeA = 0xFF;
    uint8_t _pinVolumeB = 0xFF;
    uint8_t _pinMute = 0xFF;
    uint8_t _pinPowerState = 0xFF;    
    uint16_t _lastDpadPressed;
    bool _ready;
};

#endif

