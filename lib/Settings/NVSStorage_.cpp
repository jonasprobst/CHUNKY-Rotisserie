#include "settings.h"
#include "NVSStorage.h"
#include <esp_log.h>

static const char* TAG = "NVSStorage";

NVSStorage::NVSStorage(const std::string& namespace) : namespace_(namespace) {}

bool NVSStorage::saveSettings(const SettingsInterface& settings) {
    // Open NVS
    if (!openNVS()) {
        ESP_LOGE(TAG, "Failed to open NVS");
        return false;
    }

    // Save base channel
    int baseChannel = settings.getBaseChannel();
    if (!saveValue(BASE_CHANNEL_KEY, baseChannel)) {
        ESP_LOGE(TAG, "Failed to save base channel");
        closeNVS();
        return false;
    }

    // Save mode
    const Mode& mode = settings.getMode();
    if (!saveValue(MODE_NUMBER_KEY, mode.number) ||
        !saveValue(MODE_NAME_KEY, mode.name) ||
        !saveValue(MODE_DESCRIPTION_KEY, mode.description)) {
        ESP_LOGE(TAG, "Failed to save mode");
        closeNVS();
        return false;
    }

    // Close NVS
    closeNVS();
    return true;
}

bool NVSStorage::loadSettings(SettingsInterface& settings) {
    // Open NVS
    if (!openNVS()) {
        ESP_LOGE(TAG, "Failed to open NVS");
        return false;
    }

    // Load base channel
    int baseChannel;
    if (!loadValue(BASE_CHANNEL_KEY, baseChannel)) {
        ESP_LOGE(TAG, "Failed to load base channel");
        closeNVS();
        return false;
    }
    settings.setBaseChannel(baseChannel);

    // Load mode
    Mode mode;
    if (!loadValue(MODE_NUMBER_KEY, mode.number) ||
        !loadValue(MODE_NAME_KEY, mode.name) ||
        !loadValue(MODE_DESCRIPTION_KEY, mode.description)) {
        ESP_LOGE(TAG, "Failed to load mode");
        closeNVS();
        return false;
    }
    settings.setMode(mode);

    // Close NVS
    closeNVS();
    return true;
}

bool NVSStorage::openNVS() {
    esp_err_t err = nvs_open(namespace_.c_str(), NVS_READWRITE, &nvsHandle_);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(err));
        return false;
    }
    return true;
}

void NVSStorage::closeNVS() {
    nvs_close(nvsHandle_);
}

template <typename T>
bool NVSStorage::saveValue(const std::string& key, const T& value) {
    esp_err_t err = nvs_set_blob(nvsHandle_, key.c_str(), &value, sizeof(T));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to save value for key %s: %s", key.c_str(), esp_err_to_name(err));
        return false;
    }
    return true;
}

template <typename T>
bool NVSStorage::loadValue(const std::string& key, T& value) {
    size_t size = sizeof(T);
    esp_err_t err = nvs_get_blob(nvsHandle_, key.c_str(), &value, &size);
    if (err != ESP_OK || size != sizeof(T)) {
        ESP_LOGE(TAG, "Failed to load value for key %s: %s", key.c_str(), esp_err_to_name(err));
        return false
