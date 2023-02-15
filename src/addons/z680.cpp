#include "addons/z680.h"
#include "storagemanager.h"

bool Z680::available() {
	AddonOptions options = Storage::getInstance().getAddonOptions();
    this->pinPower = options.z680PowerPin;
    this->pinPowerState = options.z680PowerStatePin;
    this->pinVolumeUp = options.z680VolumeUpPin;
    this->pinVolumeDown = options.z680VolumeDownPin;
    this->pinMute = options.z680MutePin;
    return (options.z680AddonEnabled);
}

void Z680::setup()
{
    // Setup pins
    gpio_init(this->pinPower);
    gpio_set_dir(this->pinPower, GPIO_OUT); // Set as OUTPUT
    gpio_init(this->pinVolumeUp);
    gpio_set_dir(this->pinVolumeUp, GPIO_OUT); // Set as OUTPUT
    gpio_init(this->pinVolumeDown);
    gpio_set_dir(this->pinVolumeDown, GPIO_OUT); // Set as OUTPUT
    gpio_init(this->pinMute);
    gpio_set_dir(this->pinMute, GPIO_OUT); // Set as OUTPUT    
    gpio_init(this->pinPowerState);
    gpio_set_dir(this->pinPowerState, GPIO_IN); // Set as INPUT
    gpio_pull_up(this->pinPowerState);          // Set as PULLUP
}

void Z680::process()
{
    bool temp = false;
}