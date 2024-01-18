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
    int32_t pinPower = -1;
    int32_t pinSwitch = -1;
	uint32_t triggerButtonMask = 0;
	uint32_t lastButtonsPressed = 0;
	bool triggeredButton = false;
	bool triggeredSwitch = false;
	bool timedOutSwitch = false;
	bool ready = false;
};

#endif