#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

//TODO: Add method to pass a config object for default dmxroot and mode

class WiFiAPConfigServer
{
public:
    WiFiAPConfigServer(const String &ssid,
                       const String &password,
                       const IPAddress &ip = IPAddress(192, 168, 1, 1))
    {
        m_ssid = ssid;
        m_password = password;
        m_apIP = ip;
        m_dmxRootChannel = 1; // defualt dmx channel
        m_mode = 1;           // default mode
        m_lastActivityTime = 0;
    }

    void begin()
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

    void stop()
    {
        m_server.end();
        WiFi.softAPdisconnect(true);
    }

    int getDmxRootChannel() const
    {
        return m_dmxRootChannel;
    }

    int getMode() const
    {
        return m_mode;
    }

    unsigned long getIdleTime() const
    {
        return millis() - m_lastActivityTime;
    }

private:
    AsyncWebServer m_server{80};
    String m_ssid;
    String m_password;
    IPAddress m_apIP;
    int m_dmxRootChannel;
    int m_mode;
    unsigned long m_lastActivityTime; // Keeps track of the last activity time

    void handleRoot(AsyncWebServerRequest *request)
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

    void handleConfigUpdate(AsyncWebServerRequest *request)
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
};