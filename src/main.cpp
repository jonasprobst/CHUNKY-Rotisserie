#include "WifiAPConfigServer.h"
#include "NVSSettings.h"
//#include "DMXController.h"
#include "UIController.h"

// gpio config
#define BTN_AP 5


// Load stored dmxSettings and pass them to DMXController and Webserver
NVSSettings dmx_settings;
//DMXController dmx_controller;
WifiAPConfigServer config_server(dmx_settings);

//UIController uiController(BTN_AP);

void setup()
{
    //dmxController.setBaseChannel(dmx_settings.getBaseChannel());
    config_server.start(); //TODO remove for production

}

void loop()
{
   /* if (dmxController.isConnected())
    {
        uint16_t position = dmxController.getPosition();
        int direction = dmxController.getDirection();
        int speed = dmxController.getSpeed();

        //Work the motor
    }
    else
    {
        // Emergency Stop!!
        // Display Error uiController.displayError("no DMX signal")
    }
*/

    // Update the UI based on the AP's status, which would be checked with a function in your WifiAPConfigServer class.
    // For this example, let's assume your WifiAPConfigServer class has the following functions:
    // getBaseChannel(), getMode(), isAPOn(), getSSID(), getIP()

    // Check if the button state has changed
    //if (uiController.buttonStateChanged()) {
        // Toggle the AP state and update the display
        // This would likely involve calling functions on your configServer object
        // e.g., configServer.toggleAP()
        // For this example, let's assume that your WifiAPConfigServer class has a toggleAP() function
        // configServer.toggleAP();
   // }

    // Display the current status on the OLED display
    // uiController.displayStatus(configServer.getBaseChannel(), configServer.getMode(), configServer.isAPRunning(), configServer.getSSID(), configServer.getIPAsString());

}