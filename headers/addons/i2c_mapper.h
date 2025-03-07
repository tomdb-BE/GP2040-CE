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

#ifndef I2C_MAPPER_COUNT
#define I2C_MAPPER_COUNT 12
#endif

// IO Module Name
#define I2CMapperAddonName "I2C Mapper"

struct I2CDevice {
    uint8_t address;
    uint16_t dataReceived;	
    uint16_t dataSent;	
};

struct I2CAction {
    I2CDevice device;
    uint32_t mapping;
    uint16_t command;	
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

	bool addAddress(uint8_t address);
		
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
