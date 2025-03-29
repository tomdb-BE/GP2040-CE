// I2CGeneric Library
// category=Signal Input/Output

// Written by:
//   Tom De Backer <tomdebacker1@gmail.com>

#ifndef _I2CGENERIC_H_
#define _I2CGENERIC_H_

#include <vector>

#include "i2cdevicebase.h"

class I2CGeneric : public I2CDeviceBase {
    public:
        // Constructor
        I2CGeneric() {}
        I2CGeneric(PeripheralI2C *i2cController, uint8_t addr = 0x20) {
            this->i2c_ = i2cController;            
            this->addresses_[0] = addr;
        }

        std::vector<uint8_t> getDeviceAddresses() const override {
            return this->addresses_;
        }

        //// Methods
        void begin();
        void reset();
        //void start();

        void setI2C(PeripheralI2C *i2cController) { this->i2c_ = i2cController; }
        void setAddress(uint8_t addr) { this->addresses_[0] = addr; }
        void setPrefix(uint32_t prefix, uint8_t prefixSize);

        void send(uint32_t data);

    private:
        const uint32_t initialValue_{0};
        uint8_t prefixSize_{0};
        uint8_t sendBuffer_[128];

        uint32_t dataSent_;
        uint32_t dataReceived_ = initialValue_;
    protected:
        PeripheralI2C* i2c_{nullptr};
        std::vector<uint8_t> addresses_{0};
};

#endif // _I2CGENERIC_H_
