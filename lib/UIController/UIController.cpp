#include "UIController.h"
#include <Wire.h>
#include <esp_log.h>

static const char *TAG = "UIController";

UIController::UIController(SettingsInterface &dmx_settings, WifiAPConfigServer &config_server)
    : dmx_settings_(dmx_settings),
      config_server_(config_server),
      display_(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
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
    UpdateDisplay();
    DisplaySettings();
}

void UIController::DisplayMessage(const String &message)
{
    // Break the message into lines
    message_lines_ = BreakMessageIntoLines(message);

    // Display the first few lines of the message
    ClearDisplay();
    for (int i = 0; i < min((int)message_lines_.size(), 4); i++)
    {                                 // Assume display can show 4 lines at a time
        display_.setCursor(0, i * 8); // Assume 8 pixels per line
        display_.println(message_lines_[i]);
    }
    display_.display();

    // Remember when we started displaying this message
    last_scroll_ = millis();
}

void UIController::DisplayError(const String &error_message)
{
    // TODO: Make this more fancy by adding some indicator, pretext or icon.
    DisplayMessage(error_message);
}

// -- Private methods --

void UIController::DisplaySettings()
{
    // Get settings 
    uint8_t dmx_mode = dmx_settings_.GetMode();
    uint16_t dmx_base_channel = dmx_settings_.GetBaseChannel();
    const char* ap_ssid = config_server_.GetSSID();
    const char* ap_ip = config_server_.GetIP();

    // Clear display
    ClearDisplay();

    // Assemble message
    String message = "DMX Addr: ";
    message += dmx_base_channel;
    message += " Mode: ";
    message += dmx_mode;
    message += "\nAP: ";
    message += ap_ssid;
    message += " @ ";
    message += ap_ip;

    // display message using DisplayMessage()
    DisplayMessage(message);
}

void UIController::SetupDisplay()
{
    display_.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display_.clearDisplay();
    display_.setTextColor(SSD1306_WHITE);
    display_.setTextSize(1);
}

void UIController::ClearDisplay()
{
    display_.clearDisplay();
    // display_.setTextColor(SSD1306_WHITE); TODO: needed?
    display_.display(); //TODO: needed?
}

void UIController::UpdateButtons()
{
    ap_button_.update();
    stop_button_.update();

    if (ap_button_.fell())
    {
        ESP_LOGI(TAG, "AP button pressed");
        config_server_.ToggleAP();
    }

    if (stop_button_.fell())
    {
        ESP_LOGI(TAG, "Stop button pressed");
        // TODO: Emergency stop the motor
    }
}

void UIController::UpdateDisplay()
{
    // If it's been more than a SCROLL_SPEED (eg. 1s) since we last scrolled the message
    if (message_lines_.size() > NUM_DISPLAY_LINES &&
        millis() - last_scroll_ >= SCROLL_SPEED)
    {
        // Scroll the message
        line_offset_++;
        if (line_offset_ >= message_lines_.size())
        {
            line_offset_ = 0;
        }

        // Redisplay the message
        ClearDisplay();
        for (int i = 0; i < NUM_DISPLAY_LINES; i++)
        {
            display_.setCursor(0, i * 8);
            display_.println(message_lines_[(i + line_offset_) % message_lines_.size()]);
        }
        display_.display();

        // Remember when we last scrolled the message
        last_scroll_ = millis();
    }
}

std::vector<String> UIController::BreakMessageIntoLines(const String &message)
{
    std::vector<String> lines;
    String line;
    int line_length = 0;

    int start = 0;
    while (start < message.length())
    {
        int end = message.indexOf(' ', start);
        if (end == -1)
        { // No more spaces
            end = message.length();
        }

        String word = message.substring(start, end + 1);
        if (line_length + word.length() > CHARACTERS_PER_LINE)
        { // Word doesn't fit on line
            lines.push_back(line);
            line = word;
            line_length = word.length();
        }
        else
        { // Word fits on line
            line += word;
            line_length += word.length();
        }

        start = end + 1;
    }

    // Don't forget to add the last line
    if (line.length() > 0)
    {
        lines.push_back(line);
    }

    return lines;
}