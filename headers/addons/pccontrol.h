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
protected:
	bool switchEnabled = false;
    uint8_t pinPower;
    uint8_t pinSwitch;	
};

#endif