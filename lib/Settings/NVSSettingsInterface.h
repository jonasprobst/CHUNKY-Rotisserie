#ifndef NVSSETTINGS_INTERFACE_H
#define NVSSETTINGS_INTERFACE_H

#include <arduino.h>

class NVSSettingsInterface {
public:
    virtual void initialize() = 0;
    virtual uint16_t getBaseChannel() const = 0;
    virtual uint16_t getMode() const = 0;
    virtual void save(uint16_t baseChannel, uint16_t mode) = 0;
};

#endif  // SETTINGS_INTERFACE_H