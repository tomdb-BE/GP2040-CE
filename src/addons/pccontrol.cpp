#include "addons/pccontrol.h"
#include "addons/z680.h"
#include "storagemanager.h"

bool PcControlAddon::available() {
	AddonOptions options = Storage::getInstance().getAddonOptions();
    return (options.pcControlAddonEnabled);
    return (options.pcControlAddonEnabled);
}

void PcControlAddon::setup()
{
    AddonOptions options = Storage::getInstance().getAddonOptions();
    this->pinPower = options.pcControlPowerPin;
    this->pinSwitch = options.pcControlPowerSwitchPin;
    if (this->pinPower != 0xFF) {
        gpio_init(this->pinPower);
        gpio_set_dir(this->pinPower, GPIO_OUT);
        gpio_put(this->pinPower, 1);
        if (this->pinSwitch != 0xFF) {        
            gpio_init(this->pinSwitch);
            gpio_set_dir(this->pinSwitch, GPIO_IN);
            gpio_pull_up(this->pinSwitch); 
        }
        this->ready=true;
    }
}

void PcControlAddon::process()
{
    if (!this->ready)
        return;

    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	uint16_t buttonsPressed = gamepad->state.buttons & (PCCONTROL_POWER_OFF_MASK);

    if (!buttonsPressed && this->triggeredButton) {
        this->triggeredButton = false;        
        if (time_reached(this->timeoutButtons))
            this->togglePower();
        return;
    }
    if (buttonsPressed) {      
        if (!this->triggeredButton) {
            this->timeoutButtons = make_timeout_time_ms(PCCONTROL_POWER_TOGGLE_MILLIS);
            this->timeoutButtonsForce = make_timeout_time_ms(PCCONTROL_POWER_FORCE_OFF_MILLIS);
            this->triggeredButton = true;
            return;
        }
        if (time_reached(this->timeoutButtons))
            this->forcePowerOff();
        return;  
    }

    
    if (this->pinSwitch == 0xFF)
        return;

    if (!gpio_get(this->pinSwitch)) {                
        if (this->triggeredSwitch) {        
            if (time_reached(this->timeoutSwitch) && !this->timedOutSwitch) {
                gpio_put(this->pinPower, 1);
                this->timedOutSwitch = true;
            }
			return;
        }
        this->triggeredSwitch = true;
        this->timeoutSwitch = make_timeout_time_ms(PCCONTROL_TIMEOUT_SWITCH_MILLIS);
        gpio_put(this->pinPower, 0);
        return;
    }

    if (this->triggeredSwitch)
    {
        this->triggeredSwitch = false;
        this->timedOutSwitch = false;
        gpio_put(this->pinPower, 1);
    }   
}

void PcControlAddon::setPower(uint16_t pressLength) {
    if (!this->ready || this->triggeredSwitch)
        return;    
    gpio_put(this->pinPower, 0);
    sleep_ms(pressLength);
    gpio_put(this->pinPower, 1);    
}