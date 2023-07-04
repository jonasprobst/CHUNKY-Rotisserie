#ifndef SETTINGS_H
#define SETTINGS_H

#include <arduino.h>

/**
 * @brief The Settings class encapsulates the runtime settings of the application.
 *
 */
class Settings {
public:
    /**
     * @brief Sets the base channel.
     *
     * @param base_channel The base channel value to set.
     */
    void SetBaseChannel(uint16_t base_channel);

    /**
     * @brief Gets the current base channel.
     *
     * @return The current base channel value.
     */
    uint16_t GetBaseChannel() const;

    /**
     * @brief Sets the mode.
     *
     * @param mode The mode value to set.
     */
    void SetMode(uint8_t mode);

    /**
     * @brief Gets the current mode.
     *
     * @return The current mode value.
     */
    uint8_t GetMode() const;

private:
    uint16_t base_channel_ = 1; // base channel: DMX Start Address.
    uint8_t mode_ = 1; // mode: operation mode of this device.

};

#endif  // SETTINGS_H
