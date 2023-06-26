#ifndef SETTINGS_H
#define SETTINGS_H

#include <Preferences.h>

/**
 * @class Settings
 *
 * @brief Manages application settings, providing persistent storage in the ESP32's NVS.
 */
class NVSSettings {
  public:
    /**
     * @brief Construct a new Settings object.
     *
     * This constructor initializes the settings from stored NVS values, if they exist.
     */
    NVSSettings();

    /**
     * @brief Destroy the Settings object.
     */
    ~NVSSettings();

    /**
     * @brief Get the stored root channel value.
     * 
     * @return unsigned int The stored root channel value.
     */
    unsigned int getRootChannel() const;

    /**
     * @brief Get the stored mode value.
     * 
     * @return unsigned int The stored mode value.
     */
    unsigned int getMode() const;

    /**
     * @brief Save root channel and mode settings.
     * 
     * This method saves the given values both in memory and in NVS for persistent storage.
     * 
     * @param rootChannel The root channel value to save.
     * @param mode The mode value to save.
     */
    void save(unsigned int rootChannel, unsigned int mode);

  private:
    Preferences preferences;
    const uint32_t validKeyValue = 123456789; // A value to check for valid settings in EEPROM
    static const unsigned int DEFAULT_ROOT_CHANNEL = 1;
    static const unsigned int DEFAULT_MODE = 0;
    unsigned int rootChannel = DEFAULT_ROOT_CHANNEL;
    unsigned int mode = DEFAULT_MODE;
};

#endif  // SETTINGS_H
