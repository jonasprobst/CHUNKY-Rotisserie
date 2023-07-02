#include "settings.h"
#include <esp_log.h>

static const char* TAG = "Settings";

void Settings::setBaseChannel(int base_channel) {
    // Ensure the base channel is within the valid range
    if (base_channel >= 1 && base_channel <= 513) {
        base_channel_ = base_channel;
    } else {
        // Log an error
        ESP_LOGE(TAG, "Invalid base channel value: %d. (set to default)", base_channel);
        base_channel_ = 1; // Set to default value
    }
}

int Settings::getBaseChannel() const {
    return base_channel_;
}

void Settings::setMode(int mode) {
    // Perform any necessary validation or logic
    mode_ = mode;
}

int Settings::getMode() const {
    return mode_;
}