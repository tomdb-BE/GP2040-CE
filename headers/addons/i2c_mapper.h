#ifndef _I2C_MAPPER_H
#define _I2C_MAPPER_H

#include <string>
#include <vector>

#include "i2cdevicebase.h"
#include "gpaddon.h"
#include "GamepadEnums.h"
#include "peripheralmanager.h"

#ifndef I2C_MAPPER_ENABLED
#define I2C_MAPPER_ENABLED 0
#endif

#ifndef I2C_MAP_COUNT
#define I2C_MAP_COUNT 12
#endif

// IO Module Name
#define I2CMapperAddonName "I2C Mapper"

class I2CMapper: public GPAddon {
	public:
	struct I2CDevice {
		uint8_t address;
		uint32_t dataSent;
	};

	struct I2CAction {
		I2CDevice* device;
		uint32_t buttonMap;
		uint32_t commandData;
	};
	virtual bool available();
	virtual void setup();
	virtual void preprocess() {}
	virtual void process();
    virtual std::string name() { return I2CMapperAddonName; }

	void setI2C(PeripheralI2C *i2cController) { this->i2c = i2cController; }

	bool addAddress(uint8_t address);
	I2CDevice* getDevice(uint8_t address);
		
	void send(I2CDevice* device, uint32_t data);
	    
private:
	const uint32_t initialValue = 0xFFFFFFFF;
	std::vector<I2CDevice> devices;
	std::vector<I2CAction> actions;
	uint8_t uc[128];
protected:
	PeripheralI2C* i2c = nullptr;
};

#endif  // _I2C_MAPPER_H
