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
#ifndef PCCONTROL_SWITCH_MASK
#define PCCONTROL_SWITCH_MASK 0
#endif
#ifndef PCCONTROL_BUTTON_MASK_1
#define PCCONTROL_BUTTON_MASK_1 0
#endif
#ifndef PCCONTROL_BUTTON_MASK_2
#define PCCONTROL_BUTTON_MASK_2 0
#endif

#ifndef PCCONTROL_POWER_TOGGLE_MILLIS
#define PCCONTROL_POWER_TOGGLE_MILLIS 700
#endif
#ifndef PCCONTROL_POWER_FORCE_OFF_MILLIS
#define PCCONTROL_POWER_FORCE_OFF_MILLIS 7000
#endif
#ifndef PCCONTROL_SWITCH_TOGGLE_MILLIS
#define PCCONTROL_SWITCH_TOGGLE_MILLIS 100
#endif
#ifndef PCCONTROL_BUTTONS_TOGGLE_MILLIS
#define PCCONTROL_BUTTONS_TOGGLE_MILLIS 4000
#endif
#ifndef PCCONTROL_BUTTONS_FORCE_MILLIS
#define PCCONTROL_BUTTONS_FORCE_OFF_MILLIS 10000
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
	void setPower(uint32_t pressLength);
	absolute_time_t timeoutButtons = make_timeout_time_ms(0);
	absolute_time_t timeoutButtonsForce = make_timeout_time_ms(0);
	absolute_time_t timeoutSwitchActive = make_timeout_time_ms(0);
    int32_t pinPower = -1;    
	uint32_t pcControlButtonMask = 0;	
	uint32_t pcControlSwitchMask = 0;
	bool triggered = false;
	bool triggeredSwitchActive = false;
	bool ready = false;
};

#endif
