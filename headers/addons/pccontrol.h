/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2022 Tom De Backer
 */

#include "gpaddon.h"

#ifndef _PCCONTROL_H_
#define _PCCONTROL_H_

#ifndef PCCONTROL_ENABLED
#define PCCONTROL_ENABLED 0
#endif
#ifndef PCCONTROL_POWER_PIN
#define PCCONTROL_POWER_PIN -1
#endif
#ifndef PCCONTROL_SWITCH_PIN
#define PCCONTROL_SWITCH_PIN -1
#endif
#ifndef PCCONTROL_SWITCH_TOGGLE_MILLIS
#define PCCONTROL_SWITCH_TOGGLE_MILLIS 1000
#endif
#ifndef PCCONTROL_SWITCH_FORCE_OFF_MILLIS
#define PCCONTROL_SWITCH_FORCE_OFF_MILLIS 5000
#endif
#ifndef PCCONTROL_BUTTON_TOGGLE_MILLIS
#define PCCONTROL_BUTTON_TOGGLE_MILLIS 3000
#endif
#ifndef PCCONTROL_BUTTON_FORCE_OFF_MILLIS
#define PCCONTROL_BUTTON_FORCE_OFF_MILLIS 5000
#endif
#ifndef PCCONTROL_BUTTON1_MASK
#define PCCONTROL_BUTTON1_MASK GAMEPAD_MASK_S1
#endif
#ifndef PCCONTROL_BUTTON2_MASK
#define PCCONTROL_BUTTON2_MASK GAMEPAD_MASK_S2
#endif

#define PcControlName "PcControl"

// Start PC Control Addon
class PcControlAddon : public GPAddon
{
public:
	bool available();
	void setup();
	void preprocess() {}
	void process();
	std::string name() { return PcControlName; }
	void togglePower() {this->setPower(PCCONTROL_SWITCH_TOGGLE_MILLIS); }
	void forcePowerOff() {this->setPower(PCCONTROL_SWITCH_FORCE_OFF_MILLIS); }
private:	
	bool handleState(bool currentState, bool triggered, uint32_t timeout, uint32_t timeoutForced);
	void setPower(uint16_t pressLength);
	absolute_time_t _heldTimeout;
	absolute_time_t _heldTimeoutForced;	
    uint8_t _pinPower;
    uint8_t _pinSwitch;
	bool _triggeredButton;
	bool _triggeredSwitch;	
	bool _ready;	
};

#endif