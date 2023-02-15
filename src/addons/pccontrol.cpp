#include "addons/pccontrol.h"
#include "storagemanager.h"

bool PcControl::available() {
	AddonOptions options = Storage::getInstance().getAddonOptions();
    this->pinPower = options.pcControlPowerPin;
    this->pinSwitch = options.pcControlPowerSwitchPin;
    return (options.pcControlAddonEnabled &&
        this->pinPower != (uint8_t)-1);
}

void PcControl::setup()
{
    // Setup pins
    gpio_init(this->pinPower);
    gpio_set_dir(this->pinPower, GPIO_OUT); // Set as OUTPUT
    gpio_init(this->pinSwitch);
    gpio_set_dir(this->pinSwitch, GPIO_IN); // Set as INPUT
    gpio_pull_up(this->pinSwitch);          // Set as PULLUP
}

void PcControl::process()
{
    bool temp = false;
}