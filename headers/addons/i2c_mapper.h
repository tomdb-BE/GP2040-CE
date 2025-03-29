#ifndef _I2C_MAPPER_H
#define _I2C_MAPPER_H

#include "i2cgeneric.h"

#include "gpaddon.h"
#include "peripheralmanager.h"

#include <string>

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
	struct I2CAction {
		I2CGeneric* i2cDevice = nullptr;
		uint32_t buttonsMask = 0;
		uint32_t commandData = 0;
	};
	virtual bool available();
	virtual void setup();
	virtual void preprocess() {}
	virtual void process();
    virtual std::string name() { return I2CMapperAddonName; }

	I2CGeneric* getI2CDevice(uint8_t address);
	uint8_t getPrefixSize(uint32_t prefix);
	    
private:
	I2CGeneric* i2cDevices_[I2C_MAP_COUNT];
	I2CAction actions_[I2C_MAP_COUNT];	
	uint32_t debounceTime_ = 0;
	uint32_t debounceTimer_ = 0;
	uint32_t previousButtonMask_ = 0;
	uint8_t prefixSize_ = 0;
	uint8_t actionCount_ = 0;
	uint8_t deviceCount_ = 0;	
};

#endif  // _I2C_MAPPER_H
