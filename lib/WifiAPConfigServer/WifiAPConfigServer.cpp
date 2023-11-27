#include "WifiAPConfigServer.h"
#include <esp_log.h>
#include <Arduino.h>

static constexpr const char TAG[] = "WifiAPConfigServer";

// Public functions
WifiAPConfigServer::WifiAPConfigServer(SettingsInterface &dmx_settings)
    : dmx_settings_(dmx_settings)
{
}

void WifiAPConfigServer::ToggleAP()
{
    if (IsAPRunning())
        Stop();
    else
        Start();
}

bool WifiAPConfigServer::IsAPRunning() const
{
    // Returns true if AP is running
    return (WiFi.getMode() == WIFI_MODE_AP);
}

const char *WifiAPConfigServer::GetSSID() const
{
    return SSID;
}

const char *WifiAPConfigServer::GetPassword() const
{
    return PASSWORD;
}

const char *WifiAPConfigServer::GetIP() const
{
    // Returns IP as String not IPADDRESS-Object.
    return IP;
}

const SettingsInterface &WifiAPConfigServer::GetSettings() const
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
    ip.fromString(IP);
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
    WiFi.softAP(GetSSID(), GetPassword());
    delay(100); // Give the AP some time to start. Needed?
    if (!IsAPRunning())
    {
        ESP_LOGE(TAG, "An error has occurred while starting AP");
        return;
    }
    ESP_LOGI(TAG, "AP running. SSID: %s, pw: %s", SSID, PASSWORD);

    // Serve preloaded HTML page stored in SPIFFS
    server_.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
               { HandleRoot(request); });

    server_.on("/post-config", HTTP_POST, [this](AsyncWebServerRequest *request)
               { HandleConfigUpdate(request); });

    server_.begin();
    ESP_LOGI(TAG, "Server running at %s", IP);
}

void WifiAPConfigServer::Stop()
{
    last_activity_ = 0; // Reset idle time
    server_.end();
    WiFi.softAPdisconnect(true);
}

void WifiAPConfigServer::HandleRoot(AsyncWebServerRequest *request)
{
    last_activity_ = millis(); // Update the last activity time

    // get the mode and base channel from settings
    uint8_t mode = dmx_settings_.GetMode();
    uint16_t base_channel = dmx_settings_.GetBaseChannel();
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

        // Try saving the values to the settings object
        if (dmx_settings_.SetBaseChannel(base_channel) &&
            dmx_settings_.SetMode(mode) == true)
        {
            // It worked! Redirect to the root page with a success message
            request->redirect("/?save=success");
            ESP_LOGI(TAG, "Values saved successfully. M: %d, BC: %d", mode, base_channel);
        }
        else
        {
            request->redirect("/?save=failure");
            ESP_LOGI(TAG, "Error saving values. Value(s) out of range.");
        }
    }
    else
    {
        request->redirect("/?save=failure");
        ESP_LOGI(TAG, "Error saving values. POST Request missing param(s).");
    }
}
