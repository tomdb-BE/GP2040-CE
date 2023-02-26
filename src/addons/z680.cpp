#include "addons/z680.h"
#include "storagemanager.h"

bool Z680Addon::available() {
    AddonOptions options = Storage::getInstance().getAddonOptions();
    return (options.z680AddonEnabled);
}

void Z680Addon::setup()
{
    AddonOptions options = Storage::getInstance().getAddonOptions();
    this->_pinPower = options.z680PowerPin;    
    this->_pinVolumeA = options.z680VolumeUpPin;
    this->_pinVolumeB = options.z680VolumeDownPin;
    this->_pinMute = options.z680MutePin;
    this->_pinPowerState = options.z680PowerStatePin;
    if (this->_pinPower != 0xFF) {
        gpio_init(this->_pinPower);
        gpio_set_dir(this->_pinPower, GPIO_OUT);
        gpio_put(this->_pinPower, 0);
    }
    if (this->_pinVolumeA != 0xFF) {
        gpio_init(this->_pinVolumeA);
        gpio_set_dir(this->_pinVolumeA, GPIO_OUT);
        gpio_put(this->_pinVolumeA, 0);
    }
    if (this->_pinVolumeB != 0xFF) {
        gpio_init(this->_pinVolumeB);
        gpio_set_dir(this->_pinVolumeB, GPIO_OUT);
        gpio_put(this->_pinVolumeB, 0);
    }
    if (this->_pinMute != 0xFF) {
        gpio_init(this->_pinMute);
        gpio_set_dir(this->_pinMute, GPIO_OUT);
        gpio_put(this->_pinMute, 0);
    }
    if (this->_pinPowerState != 0xFF) {
        gpio_init(this->_pinPowerState);
        gpio_set_dir(this->_pinPowerState, GPIO_IN);
        gpio_pull_up(this->_pinPowerState);
    }
    this->_ready = true;
}

void Z680Addon::process()
{
    if (!this->_ready)
        return;

    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();	
    
	bool buttonsPressed = gamepad->state.buttons & Z680_CONTROL_TRIGGER;
	uint16_t dpadPressed = gamepad->state.dpad & GAMEPAD_MASK_DPAD;

    if (!buttonsPressed)
        return;

	if (dpadPressed & (Z680_CONTROL_VOLUME_UP | Z680_CONTROL_VOLUME_DOWN))
	{
		if (this->debounce())
			return;
		(dpadPressed & Z680_CONTROL_VOLUME_UP) ? this->volumeUp() : this->volumeDown();
		return;
	}

    if (this->_lastDpadPressed == dpadPressed)
        return;

    this->_lastDpadPressed = dpadPressed;

	if ((dpadPressed & Z680_CONTROL_POWER))
        this->togglePower();
	else if ((dpadPressed & Z680_CONTROL_MUTE))
        this->mute();
  		   
}

bool Z680Addon::isOn(bool setOff)
{
    if (this->_pinPowerState == 0xFF)
        return setOff;
    return(!gpio_get(this->_pinPowerState));
}

bool Z680Addon::debounce()
{
	if (Z680_DEBOUNCE_MILLIS <= 0)
		return false;
				
    uint32_t nowTime = getMillis();

    if ((nowTime - this->_debounceVolume) > Z680_DEBOUNCE_MILLIS)
	{
        this->_debounceVolume = nowTime;
		return false;
    }
    return true;
}

bool Z680Addon::togglePower(bool setOff)
{
    if (this->_pinPower == 0xFF)
        return false;     
    gpio_put(this->_pinPower, 1);
    sleep_ms(200);
    gpio_put(this->_pinPower, 0);
    return this->isOn(!setOff);
}

void Z680Addon::mute()
{
    gpio_put(this->_pinMute, 1);
    sleep_ms(200);
    gpio_put(this->_pinMute, 0);
}

void Z680Addon::handeVolume(uint8_t amount, bool up)
{
    if (this->_pinVolumeA == 0xFF || this->_pinVolumeB == 0xFF)
        return;

    bool pinValueA = true;
    bool pinValueB = up;

    for (uint8_t i = 0; i < amount; i++) {
		gpio_put(this->_pinVolumeA, pinValueA);        
		gpio_put(this->_pinVolumeB, pinValueB);
        pinValueA ^= true;
        pinValueB ^= true;
        sleep_ms(10);
    }
}