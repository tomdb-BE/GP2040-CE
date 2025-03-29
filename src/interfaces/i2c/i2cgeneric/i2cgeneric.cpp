#include "i2cgeneric.h"

void I2CGeneric::begin() {
    reset();
}

void I2CGeneric::reset(){
    if (initialValue_) send(initialValue_);
}

void I2CGeneric::setPrefix(uint32_t prefix, uint8_t prefixSize) {
    for (int i = prefixSize - 1 ; i >=0; i--) {
        sendBuffer_[prefixSize - 1 - i] = (uint8_t) ((prefix >> (8 * i)) & 0xFF);     
    }
    prefixSize_ = prefixSize;
}

void I2CGeneric::send(uint32_t data) {
    sendBuffer_[prefixSize_] = (uint8_t) ((data >> 16) & 0xFF);
    sendBuffer_[prefixSize_ + 1] = (uint8_t) ((data >> 8) & 0xFF);
    sendBuffer_[prefixSize_ + 2] = (uint8_t) ((data >> 0) & 0xFF);
    
    i2c_->write(addresses_[0], sendBuffer_, prefixSize_ + 3);
    dataSent_ = data;
}