#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include "SettingsInterface.h"
#include "WifiAPConfigServer.h"
#include <Arduino.h>
// #include <Adafruit_SSD1306.h>
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
    static constexpr uint16_t DISPLAY_UPDATE_INTERVAL_MS = 1000;

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
    WifiAPConfigServer &config_server_;     // Reference to the config server object
    const SettingsInterface &dmx_settings_; // Reference to the settings object
    bool ap_running_ = false;               // Flag to indicate if the access point is running
    uint8_t current_mode_ = 0;              // The current mode
    uint16_t current_base_channel_ = 0;     // The current base channel
    uint32_t last_display_update_ = 0;      // Timestamp of the last display update

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
};
#endif // UI_CONTROLLER_H
