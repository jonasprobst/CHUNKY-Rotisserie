#ifndef NVS_STORAGE_H
#define NVS_STORAGE_H

#include "SettingsInterface.h"
#include <Arduino.h>
#include <esp_err.h>
#include <nvs_flash.h>
#include <nvs.h>

/**
 * @brief The NVSStorage class provides functionality to store and retrieve settings in NVS.
 *
 * The NVSStorage class uses the ESP-IDF NVS library to interact with the Non-Volatile Storage (NVS)
 * and provides methods to save and load settings from NVS.
 *
 * @note This class assumes that the NVS subsystem has been properly initialized before use.
 */
class NVSStorage {
public:
    /**
     * @brief Constructs an NVSStorage object with the specified namespace.
     *
     * @param nvs_namespace The namespace to use for NVS storage.
     */
    NVSStorage(const String& nvs_namespace);

    /**
     * @brief Destroys the NVSStorage object.
     */
    ~NVSStorage();

    /**
     * @brief Saves the settings to NVS.
     *
     * @param settings The settings object implementing the SettingsInterface.
     * @return True if the settings were successfully saved, false otherwise.
     */
    bool SaveSettings(const SettingsInterface& settings);

    /**
     * @brief Loads the settings from NVS.
     *
     * @param settings The settings object implementing the SettingsInterface.
     * @return True if the settings were successfully loaded, false otherwise.
     */
    bool LoadSettings(SettingsInterface& settings);

private:
    String nvs_namespace_;       ///< The namespace for NVS storage.
    nvs_handle_t nvs_handle_;    ///< The NVS handle.

    /**
     * @brief Opens the NVS storage.
     *
     * @return True if the NVS storage was successfully opened, false otherwise.
     */
    bool OpenNVS();

    /**
     * @brief Closes the NVS storage.
     */
    void CloseNVS();

    /**
     * @brief Saves a value to NVS with the specified key.
     *
     * @param T The type of the value to save.
     * @param key The key for the value.
     * @param value The value to save.
     * @return True if the value was successfully saved, false otherwise.
     */
    template <typename T>
    bool SaveValue(const String& key, const T& value);

    /**
     * @brief Loads a value from NVS with the specified key.
     *
     * @param U The type of the value to load.
     * @param key The key for the value.
     * @param value The variable to store the loaded value.
     * @return True if the value was successfully loaded, false otherwise.
     */
    template <typename U>
    bool LoadValue(const String& key, U& value);
};

#endif  // NVS_STORAGE_H
