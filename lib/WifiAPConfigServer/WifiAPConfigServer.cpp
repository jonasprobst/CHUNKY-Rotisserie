#include "WifiAPConfigServer.h"
#include "NVSSettings.h"

#define SSID "phak_iu"
#define PASSWORD "phak1uT00"

WifiAPConfigServer::WifiAPConfigServer(NVSSettings& settings) : dmxSettings(settings){}

void WifiAPConfigServer::begin()
{
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        Serial.println("An error has occurred while mounting SPIFFS");
        return;
    }

    // Set up access point
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(SSID, PASSWORD);

    // Serve preloaded HTML page stored in SPIFFS
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
                { handleRoot(request); });

    server.on("/post-config", HTTP_POST, [this](AsyncWebServerRequest *request)
                { handleConfigUpdate(request); });

    server.begin();
}

void WifiAPConfigServer::stop()
{
    server.end();
    WiFi.softAPdisconnect(true);
}

int WifiAPConfigServer::getDmxRootChannel() const
{
    return dmxRootChannel;
}

int WifiAPConfigServer::getMode() const
{
    return mode;
}

unsigned long WifiAPConfigServer::getIdleTime() const
{
    return millis() - lastActivityTime;
}

void WifiAPConfigServer::handleRoot(AsyncWebServerRequest *request)
{
    lastActivityTime = millis(); // update the last activity time
    String html = SPIFFS.open("/config.html", "r").readString();
    html.replace("{dmxRootChannel}", String(dmxRootChannel));

    // Replace the "selectedX" placeholder according to the current mode
    for (int i = 1; i <= 5; i++)
    {
        html.replace("{selected" + String(i) + "}", (i == mode) ? "selected" : "");
    }

    // Replace the "{message}" placeholder according to the query parameter
    if (request->hasParam("save"))
    {
        String saveStatus = request->getParam("save")->value();
        html.replace("{message}", (saveStatus == "success") ? "Values saved successfully!" : "Error saving values. Please try again.");
    }
    else
    {
        html.replace("{message}", "");
    }

    request->send(200, "text/html", html);
}

void WifiAPConfigServer::handleConfigUpdate(AsyncWebServerRequest *request)
{
    lastActivityTime = millis(); // update the last activity time

    if (request->hasParam("dmxRootChannel", true) && request->hasParam("mode", true))
    {
        this->dmxRootChannel = request->getParam("dmxRootChannel", true)->value().toInt();
        this->mode = request->getParam("mode", true)->value().toInt();
        dmxSettings.save(dmxRootChannel, mode);
        request->redirect("/?save=success");
    }
    else
    {
        request->redirect("/?save=failure");
    }
}
