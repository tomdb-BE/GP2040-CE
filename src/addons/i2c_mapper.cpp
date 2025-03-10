#include "addons/i2c_mapper.h"
#include "storagemanager.h"
#include "config.pb.h"

#include <algorithm>

bool I2CMapper::available() {
    const I2CMapperOptions& options = Storage::getInstance().getAddonOptions().i2cMapperOptions;
    I2CDevice device = {0, initialValue};
    PeripheralI2CScanResult result;

    if (!options.enabled) return false;

    for (uint8_t i = 0; i < I2C_MAP_COUNT; i++) {
        uint32_t rawCommand = options.maps[i].command;
        uint8_t address = (uint8_t) (rawCommand >> 24);    
        if (addAddress(address)) {
            std::vector<uint8_t> scanAddress = {address};
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
    
    for (uint8_t i = 0; i < I2C_MAP_COUNT; i++) {
        uint32_t rawCommand = options.maps[i].command;
        uint8_t address = (uint8_t) (rawCommand >> 24);
        I2CDevice* device = getDevice(address);
        if (device) {
            uint32_t commands = (uint32_t) (rawCommand & 0x00FFFFFF);  
            uint32_t buttonsMask = (uint32_t)  options.maps[i].buttonsMask;
            uint32_t buttonMask1 = buttonsMask >> 16;
            uint32_t buttonMask2 = buttonsMask & 0x0000FFFF;
            uint16_t buttonMask1Temp = (uint16_t) buttonMask1;
            uint16_t buttonMask2Temp = (uint16_t) buttonMask2;
            if (buttonMask1Temp > 0x8000) {
                buttonMask1Temp = ~buttonMask1Temp;
                buttonMask1 = buttonMask1Temp << 16;            
            }
            if (buttonMask2Temp > 0x8000) {
                buttonMask2Temp = ~buttonMask2Temp;
                buttonMask2 = buttonMask2Temp << 16;            
            }
            I2CAction action = {device, buttonMask1 | buttonMask2, commands};
            actions.push_back(action);            
        }
    }
}

void I2CMapper::process() {    
    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();
    for (I2CAction& action : actions) {
        if (action.buttonMap & gamepad->state.buttons) {
            send(action.device, action.commandData);
        }
    }
}

bool I2CMapper::addAddress(uint8_t address) {    
    for (I2CDevice& device : devices) {
        if (device.address == address)
            return false;
    }    
    return true;
}

I2CMapper::I2CDevice* I2CMapper::getDevice(uint8_t address) {    
    for (I2CMapper::I2CDevice& device : devices) {
        if (device.address == address)
            return &device;
    }    
    return nullptr;
}

void I2CMapper::send(I2CDevice* device, uint32_t data) {
    uc[0] = (uint8_t) (data >> 16);
    uc[1] = (uint8_t) (data >> 8);
    uc[2] = (uint8_t) data;         
    i2c->write(device->address, uc, 3);
    device->dataSent = data;
}
