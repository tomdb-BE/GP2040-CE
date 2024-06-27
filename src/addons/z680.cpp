/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2024 Tom De Backer
 */

#include "addons/z680.h"
#include "storagemanager.h"
#include "hardware/gpio.h"
#include "helper.h"

bool Z680Addon::available() {
    const Z680Options& options = Storage::getInstance().getAddonOptions().z680Options;
    return options.enabled;
}

void Z680Addon::setup()
{
    const Z680Options& options = Storage::getInstance().getAddonOptions().z680Options;            
    
    if (isValidPin(options.z680PowerPin)) {
        this->pinPower = options.z680PowerPin;
        gpio_init(this->pinPower);
        gpio_set_dir(this->pinPower, GPIO_OUT);
    }
    if (isValidPin(options.z680VolumeUpPin)) {
        this->pinVolumeUp = options.z680VolumeUpPin;
        gpio_init(this->pinVolumeUp);
        gpio_set_dir(this->pinVolumeUp, GPIO_OUT);
    }
    if (isValidPin(options.z680VolumeDownPin)) {
        this->pinVolumeDown = options.z680VolumeDownPin;
        gpio_init(this->pinVolumeDown);
        gpio_set_dir(this->pinVolumeDown, GPIO_OUT);
    }
    if (isValidPin(options.z680MutePin)) {
        this->pinMute = options.z680MutePin;
        gpio_init(this->pinMute);
        gpio_set_dir(this->pinMute, GPIO_OUT);
    }
    if (isValidPin(options.z680PowerStatePin)) {
        this->pinPowerState = options.z680PowerStatePin;
        gpio_init(this->pinPowerState);
        gpio_set_dir(this->pinPowerState, GPIO_IN);
    }

    this->triggerButtonMask = options.z680ButtonMask;
    this->ready = true;
}

void Z680Addon::process()
{
    if (!this->ready)
        return;

    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();
    uint8_t dpadPressed = gamepad->state.dpad;
    
    if (!(gamepad->state.buttons & this->triggerButtonMask) || !dpadPressed)
    {
        this->lastDpadPressed = dpadPressed;
        return;
    }

    if ((dpadPressed & Z680_CONTROL_VOLUME_UP) || (dpadPressed & Z680_CONTROL_VOLUME_DOWN))
    {
        if (this->debounce(&this->debounceVolume))
    	    return;
	(dpadPressed & Z680_CONTROL_VOLUME_UP) ? this->volumeUp() : this->volumeDown();
        return;
    }

    if (this->lastDpadPressed == dpadPressed)
        return;

    this->lastDpadPressed = dpadPressed;

    if ((dpadPressed & Z680_CONTROL_POWER))
        (this->isOn()) ? this->powerOff() : this->powerOn();
    else if ((dpadPressed & Z680_CONTROL_MUTE))
        this->mute();
}

bool Z680Addon::isOn()
{
    if (this->pinPowerState < 0)
        return true;
    return(!gpio_get(this->pinPowerState));
}

bool Z680Addon::debounce(uint32_t * debounceTime)
{
    if (Z680_DEBOUNCE_MILLIS <= 0)
	return true;
				
    uint32_t nowTime = getMillis();

    if ((nowTime - *debounceTime) > Z680_DEBOUNCE_MILLIS)
    {
        *debounceTime = nowTime;
	return false;
    }
    return true;
}

bool Z680Addon::setPower(bool setOff)
{
    if (this->pinPower < 0)
        return false;

    if (this->isOn()^setOff)
        return true;

    gpio_put(this->pinPower, 1);
    sleep_ms(50);
    gpio_put(this->pinPower, 0);
    return true;
}

void Z680Addon::mute()
{
    if (this->pinMute < 0)
        return;

    if (!this->powerOn())
        return;

    gpio_put(this->pinMute, 1);
    sleep_ms(50);
    gpio_put(this->pinMute, 0);
}

void Z680Addon::handeVolume(uint8_t amount, bool up)
{
    if (this->pinVolumeUp < 0 || this->pinVolumeDown < 0)
        return;

    if (!this->powerOn())
        return;

    bool pinValue = 1;

    for (uint8_t i = 0; i < amount; i++) {
	gpio_put(this->pinVolumeUp, pinValue^up);
	gpio_put(this->pinVolumeDown, pinValue);
        pinValue ^= true;
        sleep_ms(10);
    }

    gpio_put(this->pinVolumeUp, 0);
    gpio_put(this->pinVolumeDown, 0);
}
