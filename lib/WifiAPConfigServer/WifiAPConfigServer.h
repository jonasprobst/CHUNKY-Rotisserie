#ifndef WIFIAPCONFIGSERVER_H
#define WIFIAPCONFIGSERVER_H

// #include "NVSSettingsInterface.h"
#include "SettingsInterface.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

/**
 * @brief WiFiAPConfigServer provides a WiFi access point to configure the DMX settings via a webserver. 
 */
class WifiAPConfigServer
{
public:
    static constexpr const char* SSID = "phak_iu";
    static constexpr const char* PASSWORD = "phak1uT00";
    static constexpr const char* IP = "192.168.4.1";

    /**
     * @brief Constructor of WiFiAPConfigServer
     * @param settings Reference to the settings object
     */
    WifiAPConfigServer(SettingsInterface &dmx_settings);

    /**
     * @brief Toggle (on/off) the access point and webserver
     */
    void ToggleAP();

      /**
     * @brief Checks if the access point is running.
     * @return True if the access point is running.
     */
    bool IsAPRunning();

    /**
     * @brief Gets the idle time since the last activity.
     * @return The idle time in milliseconds.
     */
    uint32_t GetIdleTime() const;

    /**
     * @brief Gets the settings object.
     * @return Reference to the settings object.
     */
    SettingsInterface &GetSettings();

    static const char *GetSSID();
    static const char *GetIP();
    static const char *GetPassword();

private:
    AsyncWebServer server_{80};       // use port 80 for the webserver
    SettingsInterface &dmx_settings_; // reference to the settings object
    uint32_t last_activity_ = 0; // time of the last activity

    /**
     * @brief Starts the access point and webserver.
     */
    void Start();

    /**
     * @brief Stops the access point and webserver.
     */
    void Stop();

    /**
     * @brief Handles the root request and serves the configuration HTML page.
     * @param request Pointer to the incoming request.
     */
    void HandleRoot(AsyncWebServerRequest *request);

    /**
     * @brief Handles the configuration update request.
     * @param request Pointer to the incoming request.
     */
    void HandleConfigUpdate(AsyncWebServerRequest *request);
};

#endif // WIFIAPCONFIGSERVER_H
