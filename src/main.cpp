#include "WifiAPConfigServer.h"
#include "NVSSettings.h"


// If we've stored settings before, load and hand them to our Server.
NVSSettings dmxSettings; 
WifiAPConfigServer configServer(dmxSettings); 

void setup()
{
    
    configServer.begin();

    // Rest of your setup code
}

void loop()
{
    // Your main loop code
}