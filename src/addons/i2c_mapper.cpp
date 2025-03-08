#include "addons/i2c_mapper.h"
#include "storagemanager.h"
#include "config.pb.h"

bool I2CMapper::available() {
    const I2CMapperOptions& options = Storage::getInstance().getAddonOptions().i2cMapperOptions;
    I2CDevice device = {0, initialValue, initialValue};
    uint32_t rawCommand = 0;
    uint8_t address = 0;
    uint32_t commands = 0;
    std::vector<uint8_t> scanAddress = {0};    
    PeripheralI2CScanResult result;

    if (!options.enabled) return false;

    for (uint8_t i = 0; i < I2C_MAP_COUNT; i++) {
        rawCommand = options.maps[i].command;
        address = (uint8_t) (rawCommand >> 24);
        commands = (uint32_t) (rawCommand & 0x00FFFFFF);
        if (addAddress(address)) {
            scanAddress.insert(scanAddress.begin(), address);
            result = PeripheralManager::getInstance().scanForI2CDevice(scanAddress);            
            if (result.address > -1) {            
                device.address = result.address;            
                devices.push_back(device);
            }
        }
    }
    if (!devices.empty()) {
        setI2C(PeripheralManager::getInstance().getI2C(result.block));
        return true;
    }

    return false;
}

void I2CMapper::setup() {
    const I2CMapperOptions& options = Storage::getInstance().getAddonOptions().i2cMapperOptions;

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

bool I2CMapper::addAddress(uint8_t address) {    
    for (I2CDevice& device : devices) {
        if (device.address == address)
            return false;
    }    
    return true;
}	

void I2CMapper::send(uint32_t value) {              
    uc[0] = (uint8_t) (value >> 16);
    uc[1] = (uint8_t) (value >> 8);
    uc[1] = (uint8_t) value;
    for (I2CDevice& device : devices) {        
        i2c->write(device.address, uc, 3);
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