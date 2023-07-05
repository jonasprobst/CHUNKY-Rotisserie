#include "WifiAPConfigServer.h"
#include "Settings.h"
#include "DMXController.h"
#include "UIController.h"

// gpio config TODO: move to motor controller
#define UI_AP_BTN_PIN 5 
#define MOTOR_ENABLE_PIN 4
#define MOTOR_DIRECTION_PIN 2
#define MOTOR_STEP_PIN 0


// Setup the User Interface Hardware
// UIController UIController(UI_AP_BTN_PIN);

Settings dmx_settings;
WifiAPConfigServer config_server(dmx_settings);
DMXController dmx_controller(dmx_settings.GetBaseChannel());
//TODO: StepperMotorController motor_controller( MOTOR_ENABLE_PIN, MOTOR_DIRECTION_PIN, MOTOR_STEP_PIN);    

void setup()
{
    config_server.ToggleAP(); // TODO remove for production
}

void loop()
{
    // TODO: Mock a DMX connection and signal, setup Hardware (shield, USB to DMX, etc).
    if (dmx_controller.IsConnected() && dmx_controller.ReceiveNewMessage())
    {
        uint16_t position = dmx_controller.GetPosition();
        uint8_t direction = dmx_controller.GetDirection(); // TODO: implement this with an enum
        uint8_t speed = dmx_controller.GetSpeed();

        // Work the motor according to the mode
        uint8_t mode = dmx_settings.GetMode();
        switch (mode)
        { // TODO: Implement this with an enum
        case 0:
            // Manual Mode
            ESP_LOGI("main", "Mode: %d - Manual", mode);
            break;
        case 1:
            // Other mode
            ESP_LOGI("main", "Mode: %d - Auto", mode);
            break;
        case 2:
            // Yet another mode
            ESP_LOGI("main", "Mode: %d - Custom", mode);
            break;
        default:
            // Invalid mode -  this should never happen
            // TODO: Emergency Stop??
            // TODO: Display Error
            ESP_LOGE("main", "Invalid mode: %d", mode);
            break;
        }
    }
    else
    {
        // Emergency Stop!!
        // Display Error uiController.displayError("no DMX signal")
    }

    // TODO: start AP and Update Display
    /*
    if (uiController.buttonStateChanged())
    {
        // start AP
        config_server.start();

        // update display with new Settings
        uiController.displayStatus(dmx_settings.getBaseChannel(), 
                                    dmx_settings.getMode(), 
                                    true, 
                                    config_server.getSSID(),
                                    config_server.getIPAsString());
    }*/
}