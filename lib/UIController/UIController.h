#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include <Arduino.h>

/** 
 * @class UIController
 * @brief Class providing an interface to control a UI with a button and an OLED display.
 * 
 * The UIController allows the state of a button to be checked, 
 * and displays information on the OLED display including 
 * the base channel, mode, AP state, SSID, and IP address.
 */
class UIController {
public:
    /** 
     * @brief Constructor for UIController class
     * 
     * Initializes the button pin and sets up the OLED display.
     * @param buttonPin Arduino pin connected to the button
     */
    UIController(int buttonPin);

    /** 
     * @brief Check if the button state has changed since last check
     * 
     * @return True if button state has changed, false otherwise
     */
    bool buttonStateChanged();

    /** 
     * @brief Update the OLED display with the current status
     * 
     * Displays base channel and mode on the first line,
     * and if AP is on, displays SSID and IP address on the second line.
     * @param baseChannel Base channel value
     * @param mode Mode value
     * @param apState State of the AP (true if on, false if off)
     * @param ssid SSID of the AP (default: empty string)
     * @param ip IP address of the AP (default: empty string)
     */
    void displayStatus(int baseChannel, int mode, bool apState, String ssid = "", String ip = "");

    /** 
     * @brief Display an error message on the OLED display
     * 
     * @param errorMessage The error message to display
     */
    void displayError(String errorMessage);

private:
    const int _buttonPin; ///< Arduino pin connected to the button
    unsigned long _lastButtonPress; ///< Time when the button was last pressed
};

#endif // UI_CONTROLLER_H
