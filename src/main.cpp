#include "WifiAPConfigServer.h"
#include "NVSSettings.h"
#include "DMXController.h"
#include "UIController.h"

// gpio config
#define BTN_AP 5

// Setup the User Interface Hardware
UIController uiController(BTN_AP);

// Load dmxSettings from NVS and pass them to the DMXController and ConfigWebserver
NVSSettings dmx_settings;
WifiAPConfigServer config_server(dmx_settings);
DMXController dmx_controller(dmx_settings.getBaseChannel());

void setup()
{
    config_server.start(); // TODO remove for production
}

void loop()
{
    if (dmx_controller.isConnected() && dmx_controller.recieveNewMessage())
    {
        uint16_t position = dmx_controller.getPosition();
        uint8_t direction = dmx_controller.getDirection(); // TODO: implement this with an enum
        uint8_t speed = dmx_controller.getSpeed();

        // Work the motor according to the mode
        uint8_t mode = dmx_settings.getMode();
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