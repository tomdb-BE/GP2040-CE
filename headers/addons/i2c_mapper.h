#ifndef _I2C_SEND_ADDON_H
#define _I2C_SEND_ADDON_H

#include <string>
#include <vector>

#include "i2cdevicebase.h"
#include "gpaddon.h"
#include "GamepadEnums.h"
#include "peripheralmanager.h"

#ifndef I2C_INPUT_ENABLED
#define I2C_INPUT_ENABLED 0
#endif

// IO Module Name
#define I2CMapperAddonName "I2C Mapper"

struct I2CDevice {
    uint8_t address;
    uint16_t dataReceived;	
    uint16_t dataSent;	
};

class I2CMapper: public GPAddon {
public:
	virtual bool available();
	virtual void setup();
	virtual void preprocess() {}
	virtual void process();
    virtual std::string name() { return I2CMapperAddonName; }

	void begin();
	void reset();

	void setI2C(PeripheralI2C *i2cController) { this->i2c = i2cController; }

	std::vector<uint8_t> getDeviceAddresses(uint8_t address) {		
		return {address};
	}	

	void send(uint16_t value);
	void receive();
	    
private:
	const uint16_t initialValue = 0xFFFF;    
	std::vector<I2CDevice> devices;
	uint8_t uc[128];
protected:
	PeripheralI2C* i2c = nullptr;
};

#endif  // _I2C_GENERIC_SEND_ADDON_H
