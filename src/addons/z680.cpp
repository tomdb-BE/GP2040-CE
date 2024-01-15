#include "addons/z680.h"
#include "storagemanager.h"

bool Z680Addon::available() {
    AddonOptions options = Storage::getInstance().getAddonOptions();
    return (options.z680AddonEnabled);
}

void Z680Addon::setup()
{
    AddonOptions options = Storage::getInstance().getAddonOptions();
    this->pinPower = options.z680PowerPin;    
    this->pinVolumeA = options.z680VolumeUpPin;
    this->pinVolumeB = options.z680VolumeDownPin;
    this->pinMute = options.z680MutePin;
    this->pinPowerState = options.z680PowerStatePin;
    if (this->pinPower != 0xFF) {
        gpio_init(this->pinPower);
        gpio_set_dir(this->pinPower, GPIO_OUT);
    }
    if (this->pinVolumeA != 0xFF) {
        gpio_init(this->pinVolumeA);
        gpio_set_dir(this->pinVolumeA, GPIO_OUT);
    }
    if (this->pinVolumeB != 0xFF) {
        gpio_init(this->pinVolumeB);
        gpio_set_dir(this->pinVolumeB, GPIO_OUT);
    }
    if (this->pinMute != 0xFF) {
        gpio_init(this->pinMute);
        gpio_set_dir(this->pinMute, GPIO_OUT);
    }
    if (this->pinPowerState != 0xFF) {
        gpio_init(this->pinPowerState);
        gpio_set_dir(this->pinPowerState, GPIO_IN);
        gpio_pull_up(this->pinPowerState);
    }
    this->ready = true;
}

void Z680Addon::process()
{
    if (!this->ready)
        return;

    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	uint16_t buttonsPressed = gamepad->state.buttons & (Z680_CONTROL_TRIGGER);
	uint16_t dpadPressed = gamepad->state.dpad & GAMEPAD_MASK_DPAD;

    if (!(buttonsPressed && dpadPressed))
        return;

	if ((dpadPressed & Z680_CONTROL_VOLUME))
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
        (this->isOn()) ? this->powerOff(): this->powerOn();
	else if ((dpadPressed & Z680_CONTROL_MUTE))
        this->mute();
  		   
}

bool Z680Addon::isOn()
{
    if (this->pinPowerState == 0xFF)
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
    if (this->pinPower == 0xFF)
        return false;
    if (this->isOn()^setOff)
        return true;       
    gpio_put(this->pinPower, 1);
    sleep_ms(10);
    gpio_put(this->pinPower, 0);
    return true;
}

void Z680Addon::mute()
{
    if (!this->isOn())
        return;
    gpio_put(this->pinMute, 1);
    sleep_ms(10);
    gpio_put(this->pinMute, 0);
}

void Z680Addon::handeVolume(uint8_t amount, bool up)
{
    if (this->pinVolumeA == 0xFF || this->pinVolumeB == 0xFF)
        return;

    if (!this->powerOn())
        return;

    bool pinValue = true; 

    for (uint8_t i = 0; i < amount; i++) {
		gpio_put(this->pinVolumeA, pinValue);	        
		gpio_put(this->pinVolumeB, pinValue^up);
        pinValue ^= true;
        sleep_ms(10);
    }
}