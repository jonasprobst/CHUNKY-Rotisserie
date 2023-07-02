#include "WifiAPConfigServer.h"
#include "esp_log.h"
#include <Arduino.h>

static const char *TAG = "WifiAPConfigServer";

const char *WifiAPConfigServer::ssid_ = "phak_iu";
const char *WifiAPConfigServer::password_ = "phak1uT00";
const char *WifiAPConfigServer::ip_ = "192.168.4.1";

// Public functions
WifiAPConfigServer::WifiAPConfigServer(NVSSettingsInterface &dmx_settings)
    : dmx_settings_(dmx_settings)
{
}

void WifiAPConfigServer::toggleAP()
{
    if (isRunning()) stop(); else start();
}

const char *WifiAPConfigServer::getSSID()
{
    return ssid_;
}

const char *WifiAPConfigServer::getPassword()
{
    return password_;
}

const char *WifiAPConfigServer::getIP()
{
    return ip_;
}

uint32_t WifiAPConfigServer::getIdleTime() const
{
    return millis() - last_activity_;
}
// Private functions
void WifiAPConfigServer::start()
{
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        ESP_LOGE(TAG, "An error has occurred while mounting SPIFFS");
        return;
    }

    // Set up access point
    IPAddress ip;
    ip.fromString(ip_);
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
    WiFi.softAP(getSSID(), getPassword());
    delay(2000); // give the AP some time to start
    if (!isRunning())
    {
        ESP_LOGE(TAG, "An error has occurred while starting AP");
        return;
    }
    ESP_LOGI(TAG, "AP running. SSID: %s, pw: %s", ssid_, password_);

    // Serve preloaded HTML page stored in SPIFFS
    server_.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
               { handleRoot(request); });

    server_.on("/post-config", HTTP_POST, [this](AsyncWebServerRequest *request)
               { handleConfigUpdate(request); });

    server_.begin();
    ESP_LOGI(TAG, "Server running at %s", ip_);
}

void WifiAPConfigServer::stop()
{
    server_.end();
    WiFi.softAPdisconnect(true);
}

bool WifiAPConfigServer::isRunning() const
{
    // Returns true if AP is running
    return (WiFi.getMode() == WIFI_MODE_AP);
}

void WifiAPConfigServer::handleRoot(AsyncWebServerRequest *request)
{
    last_activity_ = millis(); // update the last activity time
    uint8_t mode = dmx_settings_.getMode();
    uint8_t base_channel = dmx_settings_.getBaseChannel();
    ESP_LOGI(TAG, "Settings loaded. M: %d, BC: %d", mode, base_channel);

    String html = SPIFFS.open("/config.html", "r").readString();
    html.replace("{dmxBaseChannel}", String(base_channel));

    // Replace the "selectedX" placeholder according to the current mode
    for (int i = 1; i <= 5; i++)
    {
        html.replace("{selected" + String(i) + "}", (i == mode) ? "selected" : "");
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
        uint8_t base_channel = request->getParam("dmxBaseChannel", true)->value().toInt();
        uint8_t mode = request->getParam("mode", true)->value().toInt();
        dmx_settings_.save(base_channel, mode);
        request->redirect("/?save=success");
        ESP_LOGI(TAG, "Values saved successfully. M: %d, BC: %d", mode, base_channel);
    }
    else
    {
        request->redirect("/?save=failure");
        ESP_LOGI(TAG, "Error saving values. POST Request missing param(s).");
    }
}
