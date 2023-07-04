#include "settings.h"
#include <esp_log.h>

static const char* TAG = "Settings";

void Settings::SetBaseChannel(uint16_t base_channel) {
    // Ensure the base channel is within the valid range
    if (base_channel >= 1 && base_channel <= 512) {
        base_channel_ = base_channel;
    } else {
        // Log an error
        ESP_LOGE(TAG, "Invalid base channel value: %d. Not saved.", base_channel);
    }
}

uint16_t Settings::GetBaseChannel() const {
    return base_channel_;
}

void Settings::SetMode(uint8_t mode) {
    // Ensure mode is within the valid range
    if (mode >= 1 && mode <= 5) { //TODO: Set this to correct value!
        mode_ = mode;
    } else {
        // Log an error
        ESP_LOGE(TAG, "Invalid base channel: value %d. Not saved.", mode);
    }
    mode_ = mode;
}

uint8_t Settings::GetMode() const {
    return mode_;
}