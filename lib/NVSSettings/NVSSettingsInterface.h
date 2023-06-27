#include <Arduino.h>

#ifndef NVSSETTINGS_INTERFACE_H
#define NVSSETTINGS_INTERFACE_H

class NVSSettingsInterface {
public:
    virtual uint16_t getBaseChannel() const = 0;
    virtual uint16_t getMode() const = 0;
    virtual void save(uint16_t baseChannel, uint16_t mode) = 0;
};

#endif  // SETTINGS_INTERFACE_H