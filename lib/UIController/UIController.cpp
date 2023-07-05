#include "UIController.h"
#include "SettingsInterface.h"
#include <Wire.h>
#include <esp_log.h>

static const char *TAG = "UIController";

UIController::UIController(WifiAPConfigServer &config_server)
    : config_server_(config_server),
      display_(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
      last_button_press_(0)
{
    pinMode(AP_BUTTON_PIN, INPUT_PULLUP);
    SetupDisplay();
}

void UIController::Update()
{
    UpdateButton();
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

void DisplaySettings()
{
    // FIXME: some error about config_server_  and display_ not being initialized
    // FIXME: Error DisplayMessage() not declared in this scope

    // Get settings from config_server_
    SettingsInterface &settings = config_server_.GetSettings();
    uint8_t dmx_mode = settings.GetMode();
    uint16_t dmx_start_address = settings.GetBaseChannel();
    char* ap_ssid = config_server_.GetSSID();
    char* ap_ip = config_server_.GetIP();

    // Clear display
    display_.clearDisplay();

    // Assemble message
    String message = "DMX Addr: ";
    message += dmx_start_address;
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

void UIController::UpdateButton()
{
    // Debounce the button (non blocking)
    static constexpr uint8_t DEBOUNCE_TIME_MS = 50;
    static uint32_t last_debounce_time_ms = 0;
    static bool last_button_state = LOW;

    bool current_button_state = digitalRead(button_pin_);

    // If the button state has changed, reset the debouncing timer
    if (current_button_state != last_button_state)
    {
        last_debounce_time_ms = millis();
    }

    if ((millis() - last_debounce_time_ms) > DEBOUNCE_TIME_MS)
    {
        // If the button state has not changed for the debounce interval,
        // we assume that the button is truly in a new state.
        if (current_button_state != button_state_)
        {
            button_state_ = current_button_state;

            if (button_state_ == HIGH)
            {
                // Button was pressed. Toggle the access point.
                config_server_.ToggleAP();
            }
        }
    }

    last_button_state = current_button_state;
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