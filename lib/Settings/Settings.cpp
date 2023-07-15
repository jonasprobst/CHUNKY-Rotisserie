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
    if (base_channel >= 0 && base_channel <= 505) {
        // Save the base channel
        if (nvs_storage_->SaveBaseChannel(base_channel)){
            base_channel_ = base_channel;
            ESP_LOGI(TAG, "Successfully saved base channel: %d", base_channel);
            return true;
        } else {
            ESP_LOGE(TAG, "Failed to save base channel. (NVS Error)");
            return false;
        }
    } else {
        ESP_LOGE(TAG, "Base channel value out of Range (1-505): %d. Not saved.", base_channel);
        return false;
    }
}

uint16_t Settings::GetBaseChannel() const {
    return base_channel_;
}

bool Settings::SetMode(uint8_t mode) {
    // Ensure mode is within the valid range
    if (valid_modes_.count(mode) > 0) {
        // Save the mode
        if(nvs_storage_->SaveMode(mode)){
            mode_ = mode;
            ESP_LOGI(TAG, "Successfully saved mode: %d", mode);
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

uint8_t Settings::GetRamp() const {
    switch (mode_) {
        case 1:
            return SLOW_RAMP;
        case 2:
            return NORMAL_RAMP;
        case 3:
            return FAST_RAMP;
        default:
            return DEFAULT_RAMP;
    }
}