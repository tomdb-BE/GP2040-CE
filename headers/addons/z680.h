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
#ifndef Z680_VOLUME_UP_PIN
#define Z680_VOLUME_UP_PIN -1
#endif
#ifndef Z680_VOLUME_DOWN_PIN
#define Z680_VOLUME_DOWN_PIN -1
#endif
#ifndef Z680_MUTE_PIN
#define Z680_MUTE_PIN -1
#endif
#ifndef Z680_POWER_STATE_PIN
#define Z680_POWER_STATE_PIN -1
#endif
#ifndef Z680_BUTTON_MASK
#define Z680_BUTTON_MASK 0
#endif

#ifndef Z680_VOLUME_STEP
#define Z680_VOLUME_STEP 5
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
    virtual bool available();
    virtual void setup();
    virtual void preprocess() {}
    virtual void process();
    virtual std::string name() { return Z680Name; }
    void volumeUp(uint8_t amount = Z680_VOLUME_STEP) { this->handeVolume(amount, false); }
    void volumeDown(uint8_t amount = Z680_VOLUME_STEP) { this->handeVolume(amount, true); }
    void mute();
    bool isReady() { return this->ready; }
    bool isOn();
    bool powerOn() { return this->setPower(); }
    bool powerOff() { return this->setPower(true); }
protected:
    bool setPower(bool setOff = false);
    void handeVolume(uint8_t amount, bool up = false);
    bool debounce(uint32_t * ptrDebounceTime);    
    int32_t pinPower = -1;
    int32_t pinVolumeUp = -1;
    int32_t pinVolumeDown = -1;
    int32_t pinMute = -1;
    int32_t pinPowerState = -1;
    uint32_t triggerButtonMask = 0;   
    uint32_t lastDpadPressed = 0;
    uint32_t debounceVolume = 0;
    bool ready = false;
};

#endif

