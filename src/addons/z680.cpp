#include "addons/z680.h"
#include "storagemanager.h"

bool Z680Addon::available() {
	AddonOptions options = Storage::getInstance().getAddonOptions();
    this->controls.pinPower = options.z680PowerPin;    
    this->controls.pinVolumeUp = options.z680VolumeUpPin;
    this->controls.pinVolumeDown = options.z680VolumeDownPin;
    this->controls.pinMute = options.z680MutePin;
    this->controls.pinPowerState = options.z680PowerStatePin;
    return (options.z680AddonEnabled);
}

void Z680Addon::setup()
{
    // Setup pins
    if (this->controls.pinPower != (uint8_t)-1) {
        this->controls.power = true;
        gpio_init(this->controls.pinPower);
        gpio_set_dir(this->controls.pinPower, GPIO_OUT); // Set as OUTPUT
    }
    if (this->controls.pinVolumeUp != (uint8_t)-1) {
        this->controls.volumeUp = true;
        gpio_init(this->controls.pinVolumeUp);
        gpio_set_dir(this->controls.pinVolumeUp, GPIO_OUT); // Set as OUTPUT
    }
    if (this->controls.pinVolumeDown != (uint8_t)-1) {
        this->controls.volumeDown = true;
        gpio_init(this->controls.pinVolumeDown);
        gpio_set_dir(this->controls.pinVolumeDown, GPIO_OUT); // Set as OUTPUT
    }
    if (this->controls.pinMute != (uint8_t)-1) {
        this->controls.mute = true;
        gpio_init(this->controls.pinMute);
        gpio_set_dir(this->controls.pinMute, GPIO_OUT); // Set as OUTPUT
    }
    if (this->controls.pinPowerState != (uint8_t)-1) {
        this->controls.powerState = true;
        gpio_init(this->controls.pinPowerState);
        gpio_set_dir(this->controls.pinPowerState, GPIO_IN); // Set as INPUT
        gpio_pull_up(this->controls.pinPowerState);          // Set as PULLUP
    }
}

void Z680Addon::process()
{
    bool temp = false;
}