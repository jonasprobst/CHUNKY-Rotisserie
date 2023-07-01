#include "UIController.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "esp_log.h"



// Configuration for SSD1306 OLED display (I2C)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

// Initialize the OLED display using Wire library
// TODO: is this library non-blocking?!
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

UIController::UIController(int buttonPin) : _buttonPin(buttonPin)
{
    pinMode(_buttonPin, INPUT_PULLUP);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Starting up...");
    display.display();
}

bool UIController::buttonStateChanged()
{
    int buttonState = digitalRead(_buttonPin);
    // Check if button is pressed with simple debounce
    if (buttonState == LOW && millis() - _lastButtonPress > 200)
    {
        _lastButtonPress = millis();
        return true;
    }
    return false;
}

void UIController::displayStatus(int baseChannel, int mode, bool apState, String ssid, String ip)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("C: ");
    display.print(baseChannel);
    display.print(" | M: ");
    display.println(mode);
    // If AP is on, display SSID and IP address
    if (apState)
    {
        display.print("AP: ");
        display.print(ssid);
        display.print(" | ");
        display.println(ip);
    }
    display.display();
}

void UIController::displayError(String errorMessage)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("ERROR:");
    display.println(errorMessage);
    display.display();
}
