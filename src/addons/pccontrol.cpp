#include "addons/pccontrol.h"
#include "storagemanager.h"

bool PcControlAddon::available() {
	AddonOptions options = Storage::getInstance().getAddonOptions();
    return (options.pcControlAddonEnabled);
}

void PcControlAddon::setup()
{
    AddonOptions options = Storage::getInstance().getAddonOptions();
    this->_pinPower = options.pcControlPowerPin;
    this->_pinSwitch = options.pcControlPowerSwitchPin;
    if (this->_pinPower != 0xFF) {
        gpio_init(this->_pinPower);
        gpio_set_dir(this->_pinPower, GPIO_OUT);
        gpio_put(this->_pinPower, 0);
        if (this->_pinSwitch != 0xFF) {        
            gpio_init(this->_pinSwitch);
            gpio_set_dir(this->_pinSwitch, GPIO_IN);
            gpio_pull_up(this->_pinSwitch); 
        }
        this->_ready=true;
    }
}

void PcControlAddon::process()
{
    if (!this->_ready)
        return;
    
    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    bool buttonsPressed = (gamepad->state.buttons & (PCCONTROL_POWER_OFF_MASK)) ? true : false;

    if (!this->_triggeredSwitch)
        this->_triggeredButton = this->handleState(
            buttonsPressed, 
            this->_triggeredButton,
            PCCONTROL_BUTTON_TOGGLE_MILLIS,
            PCCONTROL_BUTTON_FORCE_OFF_MILLIS
        );


    if (this->_pinSwitch != 0xFF && !this->_triggeredButton)
        this->_triggeredSwitch = this->handleState(
            !gpio_get(this->_pinSwitch), 
            this->_triggeredSwitch,
            PCCONTROL_SWITCH_TOGGLE_MILLIS,
            PCCONTROL_SWITCH_FORCE_OFF_MILLIS        
        );
}

bool PcControlAddon::handleState(bool currentState, bool triggered, uint32_t timeout, uint32_t timeoutForced) {    
    if (currentState) {
        if (!triggered) {             
            this->_heldTimeout = make_timeout_time_ms(timeout);
            this->_heldTimeoutForced = make_timeout_time_ms(timeoutForced);            
            return 1;
        }
        if (time_reached(this->_heldTimeoutForced)) {
            this->forcePowerOff();
            return 0;
        }
        return 1;
    }
 
    if (triggered && time_reached(this->_heldTimeout)) {
        this->togglePower();
        return 0;
    }  

    return triggered;
}

void PcControlAddon::setPower(uint16_t pressLength) {
    gpio_put(this->_pinPower, 1);
    sleep_ms(pressLength);
    gpio_put(this->_pinPower, 0);
}
