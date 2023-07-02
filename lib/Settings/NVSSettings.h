#ifndef NVS_SETTINGS_H
#define NVS_SETTINGS_H

#include "NVSSettingsInterface.h"
#include <Arduino.h>
#include <freertos/semphr.h>

/**
 * @brief Non-Volatile Storage (NVS) settings class for ESP32.
 *
 * This class handles saving and loading of settings into the NVS.
 */
class NVSSettings : public NVSSettingsInterface
{
public:
    /**
     * @brief Constructor for the NVSSettings class.
     *
     * This also initializes the NVS flash storage.
     */
    NVSSettings();

    /**
     * @brief Deconstructor for the NVSSettings class.
     * 
     * Cleans up semaphore that gets created in the constructor
     */
    ~NVSSettings();

    /**
     * @brief Initialize the NVS.
     *
     * This function is called in the constructor.
     */
    void initialize() override;

    /**
     * @brief Get the base channel setting from NVS.
     *
     * @return The saved base channel as a uint16_t.
     */
    uint16_t getBaseChannel() const override;

    /**
     * @brief Get the mode setting from NVS.
     *
     * @return The saved mode as a uint16_t.
     */
    uint16_t getMode() const override;

    /**
     * @brief Save the base channel and mode settings into NVS.
     *
     * @param base_channel The base channel to be saved.
     * @param mode The mode to be saved.
     */
    void save(uint16_t base_channel, uint16_t mode) override;

private:
    mutable SemaphoreHandle_t xSemaphore_;
};

#endif // NVS_SETTINGS_H

/**class NVSSettings {
    public:
        void saveSettings(Settings& settings);
        Settings loadSettings();
};*/