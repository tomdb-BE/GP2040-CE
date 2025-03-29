#include "addons/i2c_mapper.h"
#include "gamepad.h"
#include "i2cgeneric.h"
#include "storagemanager.h"
#include "config.pb.h"
#include <vector>

bool I2CMapper::available() {
    const I2CMapperOptions& options = Storage::getInstance().getAddonOptions().i2cMapperOptions;    

    if (!options.enabled) return false;    
    uint8_t prefixSize = getPrefixSize(options.prefix);
    for (uint8_t i = 0; i < I2C_MAP_COUNT; i++) {        
        uint32_t rawCommand = options.maps[i].command;
        uint8_t address = (uint8_t) (rawCommand >> 24);
        I2CGeneric* device = getI2CDevice(address);   
        if (!device) {
            device = new I2CGeneric();
            device->setAddress(address);
            if (prefixSize > 0) device->setPrefix(options.prefix, prefixSize);
            PeripheralI2CScanResult result = PeripheralManager::getInstance().scanForI2CDevice(device->getDeviceAddresses());              
            if (result.address > -1) {
                device->setAddress(result.address);
                device->setI2C(PeripheralManager::getInstance().getI2C(result.block));                          
                i2cDevices_[deviceCount_] = device;                
                deviceCount_++;
            }
            else {
                delete device;
            }
        }
    }    
    return (deviceCount_ > 0);
}

void I2CMapper::setup() {
    const I2CMapperOptions& options = Storage::getInstance().getAddonOptions().i2cMapperOptions;    
    debounceTime_ = options.debounce;
    for (uint8_t i = 0; i < I2C_MAP_COUNT; i++) {
        uint32_t rawCommand = options.maps[i].command;
        uint8_t address = (uint8_t) (rawCommand >> 24);        
        I2CGeneric* device = getI2CDevice(address);
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
            actions_[actionCount_].i2cDevice = device;
            actions_[actionCount_].buttonsMask = buttonMask1 | buttonMask2;
            actions_[actionCount_].commandData = commands;
            actionCount_++;
        }
    }
}

void I2CMapper::process() {    
    Gamepad * gamepad = Storage::getInstance().GetProcessedGamepad();
    if (gamepad->state.buttons == previousButtonMask_ && getMillis() < debounceTimer_)
        return;    
    previousButtonMask_ = gamepad->state.buttons;
    for (uint8_t i = 0; i < actionCount_; i++) {
        if (actions_[i].buttonsMask & gamepad->state.buttons) {
            actions_[i].i2cDevice->send(actions_[i].commandData);
        }
    }    
    debounceTimer_ = getMillis() + debounceTime_;
}

I2CGeneric* I2CMapper::getI2CDevice(uint8_t address) {
    if (address) {
        for (uint8_t i = 0; i < deviceCount_; i++) {
            if (i2cDevices_[i]->getDeviceAddresses()[0] == address)
                return i2cDevices_[i];
        }
    }
    return nullptr;
}

uint8_t I2CMapper::getPrefixSize(uint32_t prefix) {
    for (int i = sizeof(uint32_t) - 1; i >= 0; i--) {
        if (((prefix >> (8 * i)) & 0xFF))
            return i + 1;                         
    }
    return 0;
}
