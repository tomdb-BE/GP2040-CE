/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2022 Tom De Backer
 */

#include <gpaddon.h>

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
#ifndef PCCONTROL_POWER_OFF_MASK
#define PCCONTROL_POWER_OFF_MASK GAMEPAD_MASK_S1 & GAMEPAD_MASK_S2
#endif
#ifndef PCCONTROL_POWER_TOGGLE_MILLIS
#define PCCONTROL_POWER_TOGGLE_MILLIS 100
#endif
#ifndef PCCONTROL_POWER_FORCE_OFF_MILLIS
#define PCCONTROL_POWER_FORCE_OFF_MILLIS 5000
#endif
#ifndef PCCONTROL_TIMEOUT_SWITCH_MILLIS
#define PCCONTROL_TIMEOUT_SWITCH_MILLIS 5000
#endif


#define PcControlName "PcControl"

// Start PC Control Addon
class PcControlAddon : public GPAddon
{
public:
	virtual bool available();
	virtual void setup();
	virtual void preprocess() {}
	virtual void process();
	virtual std::string name() { return PcControlName; }
	void togglePower() {this->setPower(PCCONTROL_POWER_TOGGLE_MILLIS); }
	void forcePowerOff() {this->setPower(PCCONTROL_POWER_FORCE_OFF_MILLIS); }
protected:
	void setPower(uint16_t pressLength);
	absolute_time_t timeoutButtons;
	absolute_time_t timeoutButtonsForce;
	absolute_time_t timeoutSwitch;	    
	uint16_t lastButtonsPressed;
    uint8_t pinPower;
    uint8_t pinSwitch;
	bool triggeredButton;
	bool triggeredSwitch;
	bool timedOutSwitch;
	bool ready;	
};

#endif