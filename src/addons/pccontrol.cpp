/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2024 Tom De Backer
 */

#include "addons/pccontrol.h"
#include "storagemanager.h"
#include "hardware/gpio.h"
#include "helper.h"
#include <cstdint>

bool PcControlAddon::available() {
	const PcControlOptions& options = Storage::getInstance().getAddonOptions().pcControlOptions;
	return options.enabled;	
}

void PcControlAddon::setup()
{
    const PcControlOptions& options = Storage::getInstance().getAddonOptions().pcControlOptions;     
    
    if (isValidPin(options.pcControlPowerPin)) {
        this->pinPower = options.pcControlPowerPin;
        gpio_init(this->pinPower);
        gpio_set_dir(this->pinPower, GPIO_OUT);
        gpio_put(this->pinPower, 0);
        this->pcControlButtonMask = options.pcControlButtonMask1 | options.pcControlButtonMask2;        
        this->pcControlSwitchMask = options.pcControlSwitchMask;
        this->ready = true;
    }
}

void PcControlAddon::process()
{
    if (!this->ready)
        return;

    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
    if (this->triggeredSwitchActive)
    {
        if (!time_reached(this->timeoutSwitchActive))
            return;
        gpio_put(this->pinPower, 0);
        this->triggered = false;            
        this->triggeredSwitchActive = false;        
        return;
    }   

    // If assigned buttons and switch NOT active
	if (((gamepad->state.buttons & this->pcControlButtonMask) != this->pcControlButtonMask) 
                            && !(gamepad->state.buttons & this->pcControlSwitchMask)) {    
        if (this->triggered && time_reached(this->timeoutButtons))
            this->togglePower();
        else this->triggered = false;
        return;
    }
  
    if (!this->triggered) {
        this->triggered = true;
        if (gamepad->state.buttons & this->pcControlSwitchMask)
            this->timeoutButtons = make_timeout_time_ms(PCCONTROL_SWITCH_TOGGLE_MILLIS);
        else this->timeoutButtons = make_timeout_time_ms(PCCONTROL_BUTTONS_TOGGLE_MILLIS);
        this->timeoutButtonsForce = make_timeout_time_ms(PCCONTROL_BUTTONS_FORCE_OFF_MILLIS);            
        return;
    }

    if (time_reached(this->timeoutButtonsForce))
        this->forcePowerOff();             
}

void PcControlAddon::setPower(uint32_t pressLength) {
    if (!this->ready || this->triggeredSwitchActive)
        return;    
    this->triggeredSwitchActive = true;
    this->triggered = false;
    gpio_put(this->pinPower, 1);
    this->timeoutSwitchActive = make_timeout_time_ms(pressLength);
}