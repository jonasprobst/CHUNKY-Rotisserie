#ifndef SETTINGS_H
#define SETTINGS_H

#include "SettingsInterface.h"
#include "NVSStorage.h"

/**
 * @brief The Settings class encapsulates the runtime settings of the application.
 *
 */
class Settings : public SettingsInterface {
public:

    /**
     * @brief Constructs a Settings object.
     */
    Settings();

    /**
     * @brief Destroys a Settings object.
     */
    ~Settings();

    /**
     * @brief Sets the base channel.
     *
     * @param base_channel The base channel value to set.
     */
    bool SetBaseChannel(uint16_t base_channel);

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
    bool SetMode(uint8_t mode);

    /**
     * @brief Gets the current mode.
     *
     * @return The current mode value.
     */
    uint8_t GetMode() const;

private:
    uint16_t base_channel_; // base_channel: DMX Start Address.
    uint8_t mode_; // mode: operation mode of this device.
    NVSStorage* nvs_storage_; // NVS storage pointer
};

#endif  // SETTINGS_H
