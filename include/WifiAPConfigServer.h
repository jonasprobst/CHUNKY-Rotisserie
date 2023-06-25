#ifndef WIFIAPCONFIGSERVER_H
#define WIFIAPCONFIGSERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

/**
 * @brief WiFiAPConfigServer class provides a configurable access point for web-based configuration.
 */
class WifiAPConfigServer {
public:
    /**
     * @brief Constructs a WiFiAPConfigServer object with default configuration.
     */
    WifiAPConfigServer();

    /**
     * @brief Initializes the WiFiAPConfigServer and starts the access point.
     */
    void begin();

    /**
     * @brief Stops the WiFiAPConfigServer and disconnects the access point.
     */
    void stop();

    /**
     * @brief Gets the configured DMX root channel.
     * @return The DMX root channel.
     */
    int getDmxRootChannel() const;

    /**
     * @brief Gets the configured mode.
     * @return The mode.
     */
    int getMode() const;

    /**
     * @brief Gets the idle time since the last activity.
     * @return The idle time in milliseconds.
     */
    unsigned long getIdleTime() const;

private:
    AsyncWebServer m_server{80};
    String m_ssid;
    String m_password;
    IPAddress m_apIP;
    int m_dmxRootChannel;
    int m_mode;
    unsigned long m_lastActivityTime;

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

#endif  // WIFIAPCONFIGSERVER_H
