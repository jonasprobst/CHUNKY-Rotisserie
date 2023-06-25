#include "WifiAPConfigServer.h"

WifiAPConfigServer::WifiAPConfigServer()
{
    m_ssid = "PhakIu";
    m_password = "1234";
    m_apIP = IPAddress(192, 168, 1, 1);
    m_dmxRootChannel = 1; // default DMX channel
    m_mode = 1;           // default mode
    m_lastActivityTime = 0;
}

void WifiAPConfigServer::begin()
{
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        Serial.println("An error has occurred while mounting SPIFFS");
        return;
    }

    // Set up access point
    WiFi.softAPConfig(m_apIP, m_apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(m_ssid.c_str(), m_password.c_str());

    // Serve preloaded HTML page stored in SPIFFS
    m_server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
                { handleRoot(request); });

    m_server.on("/post-config", HTTP_POST, [this](AsyncWebServerRequest *request)
                { handleConfigUpdate(request); });

    m_server.begin();
}

void WifiAPConfigServer::stop()
{
    m_server.end();
    WiFi.softAPdisconnect(true);
}

int WifiAPConfigServer::getDmxRootChannel() const
{
    return m_dmxRootChannel;
}

int WifiAPConfigServer::getMode() const
{
    return m_mode;
}

unsigned long WifiAPConfigServer::getIdleTime() const
{
    return millis() - m_lastActivityTime;
}

void WifiAPConfigServer::handleRoot(AsyncWebServerRequest *request)
{
    m_lastActivityTime = millis(); // update the last activity time
    String html = SPIFFS.open("/config.html", "r").readString();
    html.replace("{dmxRootChannel}", String(m_dmxRootChannel));

    // Replace the "selectedX" placeholder according to the current mode
    for (int i = 1; i <= 5; i++)
    {
        html.replace("{selected" + String(i) + "}", (i == m_mode) ? "selected" : "");
    }

    // Replace the "{message}" placeholder according to the query parameter
    if (request->hasParam("saveStatus"))
    {
        String saveStatus = request->getParam("saveStatus")->value();
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
    m_lastActivityTime = millis(); // update the last activity time

    if (request->hasParam("dmxRootChannel", true) && request->hasParam("mode", true))
    {
        m_dmxRootChannel = request->getParam("dmxRootChannel", true)->value().toInt();
        m_mode = request->getParam("mode", true)->value().toInt();
        request->redirect("/?saveStatus=success");
    }
    else
    {
        request->redirect("/?saveStatus=failure");
    }
}
