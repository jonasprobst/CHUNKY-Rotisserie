#include "UIController.h"
#include <Wire.h>
#include <esp_log.h>

static constexpr char *TAG = "UIController";

UIController::UIController(SettingsInterface &dmx_settings, WifiAPConfigServer &config_server)
    : dmx_settings_(dmx_settings),
      config_server_(config_server),
      display_(nullptr) // Need to initialise wire first in SetupDisplay()
{
    // Set up the Access Point button
    ap_button_.attach(BUTTON_AP, INPUT_PULLUP);
    ap_button_.interval(BOUNCE_INTERVAL_MS);

    // Set up the Stop button
    stop_button_.attach(BUTTON_STOP, INPUT_PULLUP);
    stop_button_.interval(BOUNCE_INTERVAL_MS);

    // SetupDisplay(); TODO: reanable once fixed :-)
}

UIController::~UIController()
{
    delete display_;
}

void UIController::Update()
{
    UpdateButtons();
    UpdateDisplay();
}

void UIController::DisplayMessage(const String &message)
{
    ESP_LOGI(TAG, "Displaying message: %s", message.c_str()); //TODO: move this down again
  
   if (!display_initialized_) {
        return;
    }

    // Check following links for formatting options:
    // https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
    // https://github.com/adafruit/Adafruit_SSD1306/blob/master/examples/ssd1306_128x32_i2c/ssd1306_128x32_i2c.ino
    // -> HERE
    ClearDisplay();
    display_->write(message.c_str());
    display_->display();
}

void UIController::DisplayError(const String &error_message)
{
    ESP_LOGE(TAG, "Displaying error: %s", error_message.c_str()); //TODO: move this down again
    
    if (!display_initialized_) {
        return;
    }

    // -> HERE
    ClearDisplay();
    display_->setTextColor(SSD1306_BLACK, SSD1306_WHITE); // 'inverted' text
    display_->println(F("ERROR:")); // store in flash to save RAM F()
    display_->println(error_message.c_str()); 
}

void UIController::ToggleAP()
{
    config_server_.ToggleAP();
}

// -- Private methods --
void UIController::SetupDisplay()
{   
    // TODO: Display probably needs 5V in and logic to work properly?
    // Or maybe it can be set to use 3.3V with #define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V
    ESP_LOGI(TAG, "Setting up display");

    // TODO: I2C is probably used for Thing connected to DMX Shield?
    //Initialize I2C (this is why we can't use the constructor for display_ above)
    if (!Wire.begin(THING_SDA, THING_SCL)) {
        ESP_LOGE(TAG, "Could not start I2C bus");
        display_initialized_ = false;
        display_= nullptr;
        return;
    }

    // FIXME: This is causing problems :-(

    // TODO: true? -> Initialize SSD1306 with the I2C addr 0x3C (factory default for the 128x32)
    display_ = new Adafruit_SSD1306(DISPLAY_WIDTH, DISPLAY_HEIGHT);
   if (display_->begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS)) {
        display_initialized_ = true;
        DisplayMessage("Starting up...");
        delay(1000);
    } else {
        ESP_LOGE(TAG, "SSD1306 allocation failed");
        display_initialized_ = false;
        display_= nullptr;
    }
}

void UIController::ClearDisplay()
{
    if (!display_initialized_) {
        return;
    }

    // Clear the display and reset the cursor
    display_->clearDisplay();
    display_->setTextSize(1);
    display_->setCursor(0, 0);
    display_->setTextColor(SSD1306_WHITE);
    display_->cp437(true); // Use full 256 char 'Code Page 437' font
}

void UIController::UpdateButtons()
{
    ap_button_.update();
    stop_button_.update();

    if (ap_button_.fell())
    {
        // Enable or disable the access point
        ESP_LOGI(TAG, "AP button pressed");
        ToggleAP();
    }

    if (stop_button_.fell())
    {
        // Stop the show :-O
        ESP_LOGI(TAG, "Stop button pressed");
        // TODO: Emergency stop the motor
    }
}

void UIController::UpdateDisplay()
{
    // Only update the display every DISPLAY_UPDATE_INTERVAL_MS milliseconds
    if (millis() - last_display_update_ < DISPLAY_UPDATE_INTERVAL_MS)
    {
        return;
    }

    // Check if something has changed. If not, don't update the display
    uint16_t base_channel = dmx_settings_.GetBaseChannel();
    uint8_t mode = dmx_settings_.GetMode();
    bool ap_status = config_server_.IsAPRunning();
    

    // TODO: Check if the AP status has changed and display a message if it has
    // TODO: Remove wait time from ConfigServer if possible when starting AP 
    //       so it can't block other code (dmx, etc.)
    if (current_mode_ != mode || current_base_channel_ != base_channel || current_ap_status_ != ap_status)
    {
        // Something has changed, update the display
        current_mode_ = mode;
        current_base_channel_ = base_channel;
        current_ap_status_ = ap_status;
        String base_channel_str = FormatWithLeadingZeros(base_channel);
        String ap_status_str = current_ap_status_ ? "ON" : "OFF";
        DisplayMessage("B:" + String(base_channel_str) + ", M:" + String(mode) + ", AP:" + ap_status_str);
    } 
}

String UIController::FormatWithLeadingZeros(uint16_t num)
{
    String num_str = String(num);
    while (num_str.length() < 3) {
        num_str = "0" + num_str;
    }
    return num_str;
}