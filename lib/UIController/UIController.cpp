#include "UIController.h"
#include <Wire.h>
#include <esp_log.h>

static const char *TAG = "UIController";

UIController::UIController(SettingsInterface &dmx_settings, WifiAPConfigServer &config_server)
    : dmx_settings_(dmx_settings),
      config_server_(config_server)
{
    // Set up the Access Point button
    ap_button_.attach(AP_BUTTON_PIN, INPUT_PULLUP);
    ap_button_.interval(BOUNCE_INTERVAL_MS);

    // Set up the Stop button
    stop_button_.attach(STOP_BUTTON_PIN, INPUT_PULLUP);
    stop_button_.interval(BOUNCE_INTERVAL_MS);

    SetupDisplay();
}

void UIController::Update()
{
    UpdateButtons();
    uint32_t current_time = millis();
    if (current_time - last_display_update_ > DISPLAY_UPDATE_INTERVAL_MS)
    {
        UpdateDisplay();
        last_display_update_ = current_time;
    }
    UpdateDisplay();
}

void UIController::DisplayMessage(const String &message)
{
    ESP_LOGI(TAG, "Displaying message: %s", message.c_str());
}

void UIController::DisplayError(const String &error_message)
{
    ESP_LOGE(TAG, "Displaying error: %s", error_message.c_str());
}

// -- Private methods --
void UIController::SetupDisplay()
{
    // Initialize the OLED display
    // TODO: implement
}

void UIController::ClearDisplay()
{
    // TODO: implement
}

void UIController::UpdateButtons()
{
    ap_button_.update();
    stop_button_.update();

    if (ap_button_.fell())
    {
        ESP_LOGI(TAG, "AP button pressed");
        config_server_.ToggleAP();
        ap_running_ = !ap_running_;
    }

    if (stop_button_.fell())
    {
        ESP_LOGI(TAG, "Stop button pressed");
        // TODO: Emergency stop the motor
    }
}

void UIController::UpdateDisplay()
{
    uint16_t base_channel = dmx_settings_.GetBaseChannel();
    uint8_t mode = dmx_settings_.GetMode();
    String ap_status = ap_running_ ? "on" : "off";

    // TODO: would it be better to only update if the values have changed?
    DisplayMessage(" B: " + String(base_channel) + "M: " + String(mode) + " AP: " + ap_status);
}