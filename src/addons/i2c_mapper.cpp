#include "addons/i2c_mapper.h"
#include "storagemanager.h"
#include "config.pb.h"

bool I2CMapper::available() {
    I2CDevice device = {0, initialValue, initialValue};
    uint8_t address = 0;
    const DisplayOptions& displayOptions = Storage::getInstance().getDisplayOptions();
    const PCF8575Options& options = Storage::getInstance().getAddonOptions().pcf8575Options;
    if (options.enabled) {        
        PeripheralI2CScanResult result = PeripheralManager::getInstance().scanForI2CDevice(getDeviceAddresses(address));
        if (result.address > -1) {            
            device.address = result.address;            
            devices.push_back(device);
        }
        if (!devices.empty()) {
            setI2C(PeripheralManager::getInstance().getI2C(result.block));
            return true;
        }
    }
    return false;
}

void I2CMapper::setup() {
    const PCF8575Options& options = Storage::getInstance().getAddonOptions().pcf8575Options;

}

void I2CMapper::process()
{
    Gamepad * gamepad = Storage::getInstance().GetGamepad();
}

void I2CMapper::begin() {
    reset();
}

void I2CMapper::reset(){
    send(initialValue);
}

void I2CMapper::send(uint16_t value) {              
    uc[0] = ((value >> 0) & 0x00FF);
    uc[1] = ((value >> 8) & 0x00FF);
    for (I2CDevice& device : devices) {        
        i2c->write(device.address, uc, 2);
        device.dataSent = value;
    }
}

void I2CMapper::receive() {
    uint8_t readBytes[2];    
    for (I2CDevice& device : devices) { 
        i2c->read(device.address, readBytes, 2);
        device.dataReceived = ((readBytes[0] << 0) | (readBytes[1] << 8));
    }    
}