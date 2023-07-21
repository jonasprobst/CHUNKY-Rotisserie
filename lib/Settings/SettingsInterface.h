#ifndef SETTINGS_INTERFACE_H
#define SETTINGS_INTERFACE_H

#include <Arduino.h>

/**
 * @brief The SettingsInterface class provides an 
 * interface for accessing and modifying application settings.
 * 
 * base_channel: DMX Start Address.
 * mode: operation mode of this device.
 */
class SettingsInterface
{
public:
    /**
     * @brief Sets the base channel.
     *
     * @param baseChannel The base channel value to set.
     */
    virtual bool SetBaseChannel(uint16_t baseChannel) = 0;

    /**
     * @brief Gets the current base channel.
     *
     * @return The current base channel value.
     */
    virtual uint16_t GetBaseChannel() const = 0;

    /**
     * @brief Sets the mode.
     *
     * @param mode The mode value to set.
     */
    virtual bool SetMode(uint8_t mode) = 0;

    /**
     * @brief Gets the current mode.
     *
     * @return The current mode value.
     */
    virtual uint8_t GetMode() const = 0;

    /**
     * @brief Gets the current ramp.
     * 
     * @return The current ramp value.
     */
    virtual uint8_t GetRamp() const = 0;
};

#endif // SETTINGS_INTERFACE_H