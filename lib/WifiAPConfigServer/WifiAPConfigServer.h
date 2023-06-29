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
    WifiAPConfigServer(NVSSettingsInterface& settings);

    /**
     * @brief Starts the WiFiAPConfigServer, acess point and webserver
     */
    void start();

    /**
     * @brief Stops the WiFiAPConfigServer, acces point and webserver
     */
    void stop();

    /**
     * @brief Gets the configured DMX root channel.
     * @return The DMX root channel.
     */
    int getBaseChannel() const;

    /**
     * @brief Gets the configured mode.
     * @return The mode.
     */
    int getMode() const;

    // TODO implement and document methods
    bool isAPRunning() const;

    String getSSID() const;

    String getIPAsString() const;

    /**
     * @brief Gets the idle time since the last activity.
     * @return The idle time in milliseconds.
     */
    uint32_t getIdleTime() const;

private:
    AsyncWebServer server_{80};
    IPAddress apIP_ = IPAddress(192, 168, 4, 1);
    uint16_t base_channel_ = 0;
    uint16_t mode_ = 0;
    uint32_t last_activity_ = 0;
    NVSSettingsInterface &dmx_settings_;

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
