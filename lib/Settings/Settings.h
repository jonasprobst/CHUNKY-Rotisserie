#ifndef SETTINGS_H
#define SETTINGS_H

#include "SettingsInterface.h"

/**
 * @brief The Settings class encapsulates the runtime settings of the application.
 *
 */
class Settings : public SettingsInterface {
public:
    /**
     * @brief Constructs a Settings object.
     */
    //Settings();

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

    /**
     * @brief Sets mode and base channel to default values.
     */
    void Reset();

private:
    uint16_t base_channel_ = DEFAULT_BASE_CHANNEL; // base_channel: DMX Start Address.
    uint8_t mode_ = DEFAULT_MODE; // mode: operation mode of this device.
};

#endif  // SETTINGS_H
