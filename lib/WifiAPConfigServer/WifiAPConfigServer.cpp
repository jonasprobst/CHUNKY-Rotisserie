#include "WifiAPConfigServer.h"
#include "esp_log.h"
#include <Arduino.h>

// INFO: See this library for more advance wifi management and captive portal: https://github.com/tzapu/WiFiManager

// Default SSID and Password for AP
#define AP_SSID "phak_iu"
#define AP_PASSWORD "phak1uT00"

WifiAPConfigServer::WifiAPConfigServer(NVSSettingsInterface &settings) : dmx_settings_(settings) {}

void WifiAPConfigServer::begin()
{
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        ESP_LOGE("WifiAPConfigServer:", "An error has occurred while mounting SPIFFS");
        return;
    }

    // Set up access point
    WiFi.softAPConfig(apIP_, apIP_, IPAddress(255, 255, 255, 0));
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    delay(2000); // give AP some time to start
    if (!isAPRunning())
    {
        ESP_LOGE("WifiAPConfigServer:", "An error has occurred while starting AP");
        return;
    }
    ESP_LOGI("WifiAPConfigServer:", "AP running. SSID: %s, pw: %s", AP_SSID, AP_PASSWORD);

    // Serve preloaded HTML page stored in SPIFFS
    server_.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
               { handleRoot(request); });

    server_.on("/post-config", HTTP_POST, [this](AsyncWebServerRequest *request)
               { handleConfigUpdate(request); });

    server_.begin();
    ESP_LOGI("WifiAPConfigServer:", "Server running at %s", getIPAsString());
}

void WifiAPConfigServer::stop()
{
    server_.end();
    WiFi.softAPdisconnect(true);
}

int WifiAPConfigServer::getBaseChannel() const
{
    return base_channel_;
}

int WifiAPConfigServer::getMode() const
{
    return mode_;
}

bool WifiAPConfigServer::isAPRunning() const
{
    // Returns true if AP is running
    return (WiFi.getMode() == WIFI_MODE_AP);
}

String WifiAPConfigServer::getSSID() const
{
    return String(AP_SSID);
}

String WifiAPConfigServer::getIPAsString() const
{
    return apIP_.toString();
}

uint32_t WifiAPConfigServer::getIdleTime() const
{
    return millis() - last_activity_;
}

void WifiAPConfigServer::handleRoot(AsyncWebServerRequest *request)
{
    last_activity_ = millis(); // update the last activity time
    mode_ = dmx_settings_.getMode();
    base_channel_ = dmx_settings_.getBaseChannel();
    ESP_LOGI("WifiAPConfigServer:", "Settings loaded. M: %d, BC: %d", mode_, base_channel_);

    String html = SPIFFS.open("/config.html", "r").readString();
    html.replace("{dmxBaseChannel}", String(base_channel_));

    // Replace the "selectedX" placeholder according to the current mode
    for (int i = 1; i <= 5; i++)
    {
        html.replace("{selected" + String(i) + "}", (i == mode_) ? "selected" : "");
    }

    // Replace the "{message}" placeholder according to the query parameter
    if (request->hasParam("save"))
    {
        String save_status = request->getParam("save")->value();
        html.replace("{message}", (save_status == "success") ? "Values saved successfully!" : "Error saving values. Please try again.");
    }
    else
    {
        html.replace("{message}", "");
    }

    request->send(200, "text/html", html);
}

void WifiAPConfigServer::handleConfigUpdate(AsyncWebServerRequest *request)
{
    last_activity_ = millis(); // update the last activity time

    if (request->hasParam("dmxBaseChannel", true) && request->hasParam("mode", true))
    {
        base_channel_ = request->getParam("dmxBaseChannel", true)->value().toInt();
        mode_ = request->getParam("mode", true)->value().toInt();
        dmx_settings_.save(base_channel_, mode_);
        request->redirect("/?save=success");
        ESP_LOGI("WifiAPConfigServer:", "Values saved succesfully. M: %d, BC: %d", mode_, base_channel_);
    }
    else
    {
        request->redirect("/?save=failure");
        ESP_LOGI("WifiAPConfigServer:", "Error saving values. POST Request missing param(s).");
    }
}
