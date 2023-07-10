#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include "SettingsInterface.h"
#include "WifiAPConfigServer.h"
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <vector>
#include <Bounce2.h>

/**
 * @class UIController
 * @brief The User Interface that manages the access point and displays the current settings.
 *
 * @attention Update() must to be called regularly in the main loop.
 */
class UIController
{
public:
    // Button pin to toggle the access point
    static constexpr uint8_t AP_BUTTON_PIN = 12;       // TODO: change to correct pin
    static constexpr uint8_t STOP_BUTTON_PIN = 13;     // TODO: change to correct pin and implement it
    static constexpr uint16_t BOUNCE_INTERVAL_MS = 10; // Bounce interval in milliseconds

    // Configuration for SSD1306 OLED display (vis I2C)
    static constexpr int16_t SCREEN_WIDTH = 128; // OLED display width, in pixels
    static constexpr int16_t SCREEN_HEIGHT = 32; // OLED display height, in pixels
    static constexpr int8_t OLED_RESET = -1;     // Reset pin # (or -1 if sharing Arduino reset pin)
    static constexpr int8_t CHARACTER_WIDTH = 6; // Character width of used font in pixels
    static constexpr int8_t CHARACTERS_PER_LINE = SCREEN_WIDTH / CHARACTER_WIDTH;
    static constexpr int8_t NUM_DISPLAY_LINES = 2; // Number of lines to display at once
    static constexpr int16_t SCROLL_SPEED = 1000;  // Scroll speed in milliseconds

    /**
     * @brief Constructor for UIController class
     *
     * Initializes the button pin and sets up the OLED display.
     * @param config_server reference to the config server
     */
    UIController(SettingsInterface &settings, WifiAPConfigServer &config_server);

    /**
     * @brief Update the display and manage the button.
     * must be called regularly in the main loop.
     */
    void Update();

    /**
     * @brief Display an message on the OLED display
     *
     * @param message The message to display
     */
    void DisplayMessage(const String &message);

    /**
     * @brief Display an error message on the OLED display
     *
     * @param error_message The error message to display
     */
    void DisplayError(const String &error_message);

private:
    Bounce ap_button_;                      // Button to toggle the access point
    Bounce stop_button_;                    // Button to stop the show
    Adafruit_SSD1306 display_;              // OLED display object
    WifiAPConfigServer &config_server_;     // Reference to the config server object
    const SettingsInterface &dmx_settings_; // Reference to the settings object
    uint32_t last_scroll_ = 0;              // Time of the last scroll (ninja-go! :-D)
    std::vector<String> message_lines_;     // message split into lines to fit the display
    uint16_t line_offset_ = 0;              // offset for scrolling the message lines

    /**
     * @brief Update the OLED display with the current dmx and access point settings
     */
    void DisplaySettings();

    /**
     * @brief Setup Display (initialize, set color etc.)
     */
    void SetupDisplay();

    /**
     * @brief Clear the Display
     */
    void ClearDisplay();

    /**
     * @brief Read the buttons and execute the corresponding actions
     */
    void UpdateButtons();

    /**
     * @brief Update the display and scroll it if necessary
     */
    void UpdateDisplay();

    /**
     * @brief Break the message into lines to fit the display
     */
    std::vector<String> BreakMessageIntoLines(const String &message);
};
#endif // UI_CONTROLLER_H
