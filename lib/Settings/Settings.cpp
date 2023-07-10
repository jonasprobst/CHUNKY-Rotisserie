#include "Settings.h"
#include <esp_log.h>

static const char* TAG = "Settings";

Settings::Settings() {
    // Set default values
    nvs_storage_ = new NVSStorage(); // Use 'new' to allocate on the heap
    base_channel_ = nvs_storage_->LoadBaseChannel();
    mode_ = nvs_storage_->LoadMode();
}

Settings::~Settings() {
    delete nvs_storage_;
}

bool Settings::SetBaseChannel(uint16_t base_channel) {
    // Ensure the base channel is within the valid range
    if (base_channel >= 1 && base_channel <= 512) {
        // Save the base channel
        if (nvs_storage_->SaveBaseChannel(base_channel)){
            ESP_LOGI(TAG, "Successfully saved base channel: %d", base_channel);
            base_channel_ = base_channel;
            return true;
        } else {
            ESP_LOGE(TAG, "Failed to save base channel.");
            return false;
        }
    } else {
        ESP_LOGE(TAG, "Invalid base channel value: %d. Not saved.", base_channel);
        return false;
    }
}

uint16_t Settings::GetBaseChannel() const {
    return base_channel_;
}

bool Settings::SetMode(uint8_t mode) {
    // Ensure mode is within the valid range
    if (mode >= 1 && mode <= 5) { //TODO: Set this to correct value!
        // Save the mode
        if(nvs_storage_->SaveMode(mode)){
            ESP_LOGI(TAG, "Successfully saved mode: %d", mode);
            mode_ = mode;
            return true;
        } else {
            ESP_LOGE(TAG, "Failed to save mode.");
            return false;
        }
    } else {
        ESP_LOGE(TAG, "Invalid mode value: %d. Not saved.", mode);
        return false;
    }
}

uint8_t Settings::GetMode() const {
    return mode_;
}