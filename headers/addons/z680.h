/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2022 Tom De Backer
 */

#ifndef _Z680_H_
#define _Z680_H_

#ifndefZ680_ENABLED
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

#include "gpaddon.h"

#define Z680Name "Z680"

// Start Z680 Addon
class Z680 : public GPAddon
{
public:
	virtual bool available();
	virtual void setup();
	virtual void preprocess() {}
	virtual void process();
	virtual std::string name() { return Z680Name; }
protected:
    uint8_t pinPower;
    uint8_t pinPowerState;
    uint8_t pinVolumeUp;
    uint8_t pinVolumeDown;
    uint8_t pinMute;
};

#endif

