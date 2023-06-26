#include "WifiAPConfigServer.h"
#include "NVSSettings.h"
#include "DMXController.h"

// Load stored dmxSettings and populate our Server
NVSSettings dmxSettings;
DMXController dmxController(dmxSettings.getBaseChannel());
WifiAPConfigServer configServer(dmxSettings);

void setup()
{

    configServer.begin();

    // Rest of your setup code
}

void loop()
{
    if (dmxController.isConnected())
    {
        uint16_t position = dmxController.getPosition();
        int direction = dmxController.getDirection();
        int speed = dmxController.getSpeed();
    }
    else
    {
        // Emergency Stop!!
    }
}