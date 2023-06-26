#include "WifiAPConfigServer.h"
#include "NVSSettings.h"

#define SSID "phak_iu"
#define PASSWORD "phak1uT00"

WifiAPConfigServer::WifiAPConfigServer(NVSSettings& settings) : _dmxSettings(settings){}

void WifiAPConfigServer::begin()
{
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        Serial.println("An error has occurred while mounting SPIFFS");
        return;
    }

    // Set up access point
    WiFi.softAPConfig(_apIP, _apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(SSID, PASSWORD);

    // Serve preloaded HTML page stored in SPIFFS
    _server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
                { handleRoot(request); });

    _server.on("/post-config", HTTP_POST, [this](AsyncWebServerRequest *request)
                { handleConfigUpdate(request); });

    _server.begin();
}

void WifiAPConfigServer::stop()
{
    _server.end();
    WiFi.softAPdisconnect(true);
}

int WifiAPConfigServer::getDmxBaseChannel() const
{
    return _dmxBaseChannel;
}

int WifiAPConfigServer::getMode() const
{
    return _mode;
}

uint32_t WifiAPConfigServer::getIdleTime() const
{
    return millis() - _lastActivityTime;
}

void WifiAPConfigServer::handleRoot(AsyncWebServerRequest *request)
{
    _lastActivityTime = millis(); // update the last activity time
    String html = SPIFFS.open("/config.html", "r").readString();
    html.replace("{dmxBaseCHannel}", String(_dmxBaseChannel));

    // Replace the "selectedX" placeholder according to the current mode
    for (int i = 1; i <= 5; i++)
    {
        html.replace("{selected" + String(i) + "}", (i == _mode) ? "selected" : "");
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
    _lastActivityTime = millis(); // update the last activity time

    if (request->hasParam("dmxBaseChannel", true) && request->hasParam("mode", true))
    {
        _dmxBaseChannel = request->getParam("dmxBaseChannel", true)->value().toInt();
        _mode = request->getParam("mode", true)->value().toInt();
        _dmxSettings.save(_dmxBaseChannel, _mode);
        request->redirect("/?save=success");
    }
    else
    {
        request->redirect("/?save=failure");
    }
}
