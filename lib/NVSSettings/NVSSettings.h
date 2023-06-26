#ifndef NVSSETTINGS_H
#define NVSSETTINGS_H

#include <Arduino.h>
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
     * @return uint16_t The stored root channel value.
     */
   uint16_t getBaseChannel() const;

    /**
     * @brief Get the stored mode value.
     * 
     * @return uint16_t The stored mode value.
     */
   uint16_t getMode() const;

    /**
     * @brief Save base channel and mode settings.
     * 
     * This method saves the given values both in memory and in NVS for persistent storage.
     * 
     * @param baseChannel The base channel value to save.
     * @param mode The mode value to save.
     */
    void save( uint16_t baseChannel, uint16_t mode);

  private:
    Preferences _preferences;
    const uint32_t _validKeyValue = 123456789; // A value to check for valid settings in EEPROM
    uint16_t _baseChannel = 1;
    uint16_t _mode = 1;
};

#endif  // NVSSETTINGS_H
