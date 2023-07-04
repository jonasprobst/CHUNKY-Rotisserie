#ifndef WIFIAPCONFIGSERVER_H
#define WIFIAPCONFIGSERVER_H

// #include "NVSSettingsInterface.h"
#include "SettingsInterface.h"
#include "NVSStorage.h"
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
    /**
     * @brief Constructor of WiFiAPConfigServer
     * @param settings Reference to the settings object
     */
    WifiAPConfigServer(SettingsInterface &settings);

    /**
     * @brief Toggle (on/off) the access point and webserver
     */
    void ToggleAP();

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
    static const char *ssid_;         // access point name
    static const char *password_;     // access point password
    static const char *ip_;           // webserver IP address
    AsyncWebServer server_{80};       // use port 80 for the webserver
    SettingsInterface &dmx_settings_; // reference to the settings object
    NVSStorage nvs_storage_;          // storage for the settings
    uint32_t last_activity_ = 0; // time of the last activity

    /**
     * @brief Checks if the access point is running.
     * @return True if the access point is running.
     */
    bool IsRunning() const;

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
