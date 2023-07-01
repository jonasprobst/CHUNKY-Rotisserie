#ifndef WIFIAPCONFIGSERVER_H
#define WIFIAPCONFIGSERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "NVSSettingsInterface.h"

/**
 * @brief WiFiAPConfigServer class provides a configurable access point for web-based configuration.
 * Implements the NVSSettingsInterface
 */
class WifiAPConfigServer
{
public:
    /**
     * @brief Constructor of WiFiAPConfigServer
     * @param settings Reference to the settings object
     */
    WifiAPConfigServer(NVSSettingsInterface &settings);

    /**
     * @brief Toggle (on/off) the acess point and webserver
     */
    void toggleAP();

    /**
     * @brief Gets the idle time since the last activity.
     * @return The idle time in milliseconds.
     */
    uint32_t getIdleTime() const;


    static const char* getSSID();
    static const char* getIP();
    static const char* getPassword();

private:
    static const char* ssid_ ;       // access point name
    static const char* password_; // access point password
    static const char* ip_;     // webserver IP address
    AsyncWebServer server_{80};                     // use port 80 for the webserver
    NVSSettingsInterface &dmx_settings_;            // reference to the settings object
    uint32_t last_activity_ = 0;                    // time of the last activity

    /**
     * @brief Checks if the access point is running.
     * @return True if the access point is running.
     */
    bool isRunning() const;

    /**
     * @brief Starts the access point and webserver.
     */
    void start();

    /**
     * @brief Stops the access point and webserver.
     */
    void stop();

    /**
     * @brief Handles the root request and serves the configuration HTML page.
     * @param request Pointer to the incoming request.
     */
    void handleRoot(AsyncWebServerRequest *request);

    /**
     * @brief Handles the configuration update request.
     * @param request Pointer to the incoming request.
     */
    void handleConfigUpdate(AsyncWebServerRequest *request);
};

#endif // WIFIAPCONFIGSERVER_H
