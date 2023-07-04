#include "WifiAPConfigServer.h"
#include <esp_log.h>
#include <Arduino.h>

static const char *TAG = "WifiAPConfigServer";

const char *WifiAPConfigServer::ssid_ = "phak_iu";
const char *WifiAPConfigServer::password_ = "phak1uT00";
const char *WifiAPConfigServer::ip_ = "192.168.4.1";

// Public functions
WifiAPConfigServer::WifiAPConfigServer(SettingsInterface &settings)
    : dmx_settings_(settings)
{
}

void WifiAPConfigServer::ToggleAP()
{
    if (IsRunning())
        Stop();
    else
        Start();
}

const char *WifiAPConfigServer::GetSSID()
{
    return ssid_;
}

const char *WifiAPConfigServer::GetPassword()
{
    return password_;
}

const char *WifiAPConfigServer::GetIP()
{
    return ip_;
}

SettingsInterface &WifiAPConfigServer::GetSettings()
{
    return dmx_settings_;
}

uint32_t WifiAPConfigServer::GetIdleTime() const
{
    return millis() - last_activity_;
}

// Private functions
void WifiAPConfigServer::Start()
{
    last_activity_ = millis(); // Update the last activity time

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
    WiFi.softAP(GetSSID(), GetPassword());
    delay(2000); // Give the AP some time to start
    if (!IsRunning())
    {
        ESP_LOGE(TAG, "An error has occurred while starting AP");
        return;
    }
    ESP_LOGI(TAG, "AP running. SSID: %s, pw: %s", ssid_, password_);

    // Serve preloaded HTML page stored in SPIFFS
    server_.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
               { HandleRoot(request); });

    server_.on("/post-config", HTTP_POST, [this](AsyncWebServerRequest *request)
               { HandleConfigUpdate(request); });

    server_.begin();
    ESP_LOGI(TAG, "Server running at %s", ip_);
}

void WifiAPConfigServer::Stop()
{
    last_activity_ = 0; // Reset idle time
    server_.end();
    WiFi.softAPdisconnect(true);
}

bool WifiAPConfigServer::IsRunning() const
{
    // Returns true if AP is running
    return (WiFi.getMode() == WIFI_MODE_AP);
}

void WifiAPConfigServer::HandleRoot(AsyncWebServerRequest *request)
{
    last_activity_ = millis(); // Update the last activity time

    // Load settings from NVS
    nvs_storage_.LoadSettings(dmx_settings_);
    uint8_t mode = dmx_settings_.GetMode();
    uint8_t base_channel = dmx_settings_.GetBaseChannel();
    ESP_LOGI(TAG, "Settings loaded. M: %d, BC: %d", mode, base_channel);

    // Load the HTML page from SPIFFS
    String html = SPIFFS.open("/index.html", "r").readString();
    html.replace("{dmxBaseChannel}", String(base_channel));

    // Replace the dropdown placeholder for the currently selected mode
    for (int i = 1; i <= 5; i++)
    {
        html.replace("{selected" + String(i) + "}", (i == mode) ? "selected" : "");
    }

    // Replace the save message placeholder with the appropriate message
    if (request->hasParam("save"))
    {
        String save_status = request->getParam("save")->value();
        if (save_status == "success")
        {
            html.replace("{messageClass}", "success");
            html.replace("{message}", "Values saved successfully!");
        }
        else
        {
            html.replace("{messageClass}", "error");
            html.replace("{message}", "Error saving values. Please try again.");
        }
    }
    else
    {
        // Default to no message
        html.replace("{message}", "");
    }

    // Send the assembled HTML page
    request->send(200, "text/html", html);
}

void WifiAPConfigServer::HandleConfigUpdate(AsyncWebServerRequest *request)
{
    last_activity_ = millis(); // update the last activity time

    if (request->hasParam("dmxBaseChannel", true) && request->hasParam("mode", true))
    {
        // Get the values from the POST request
        uint16_t base_channel = request->getParam("dmxBaseChannel", true)->value().toInt();
        uint8_t mode = request->getParam("mode", true)->value().toInt();

        // Save the values to the settings object and to NVS
        dmx_settings_.SetBaseChannel(base_channel);
        dmx_settings_.SetMode(mode);
        nvs_storage_.SaveSettings(dmx_settings_);

        // Redirect to the root page with a success message
        request->redirect("/?save=success");
        ESP_LOGI(TAG, "Values saved successfully. M: %d, BC: %d", mode, base_channel);
    }
    else
    {
        request->redirect("/?save=failure");
        ESP_LOGI(TAG, "Error saving values. POST Request missing param(s).");
    }
}
