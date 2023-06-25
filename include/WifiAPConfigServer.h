#ifndef WIFIAPCONFIGSERVER_H
#define WIFIAPCONFIGSERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

/**
 * @brief Class for configuring and managing a WiFi access point with a configuration server.
 */
class WiFiAPConfigServer {
public:
    /**
     * @brief Constructs a WiFiAPConfigServer object.
     *
     * @param ssid The SSID of the access point.
     * @param password The password of the access point.
     * @param ip The IP address of the access point (default: 192.168.1.1).
     */
    WiFiAPConfigServer(const String& ssid, const String& password, const IPAddress& ip = IPAddress(192, 168, 1, 1));

    /**
     * @brief Begins the WiFi access point and starts the configuration server.
     */
    void begin();

    /**
     * @brief Stops the configuration server and disconnects the WiFi access point.
     */
    void stop();

    /**
     * @brief Gets the root DMX channel.
     *
     * @return The root DMX channel.
     */
    int getDmxRootChannel() const;

    /**
     * @brief Gets the current mode.
     *
     * @return The current mode.
     */
    int getMode() const;

    /**
     * @brief Gets the idle time since the last activity.
     *
     * @return The idle time in milliseconds.
     */
    unsigned long getIdleTime() const;

private:
    AsyncWebServer m_server{80}; /**< The configuration server. */
    String m_ssid; /**< The SSID of the access point. */
    String m_password; /**< The password of the access point. */
    IPAddress m_apIP; /**< The IP address of the access point. */
    int m_dmxRootChannel; /**< The root DMX channel. */
    int m_mode; /**< The current mode. */
    unsigned long m_lastActivityTime; /**< The timestamp of the last activity. */

    /**
     * @brief Handles the root request.
     *
     * @param request The AsyncWebServerRequest object.
     */
    void handleRoot(AsyncWebServerRequest* request);

    /**
     * @brief Handles the configuration update request.
     *
     * @param request The AsyncWebServerRequest object.
     */
    void handleConfigUpdate(AsyncWebServerRequest* request);
};

#endif // WIFIAPCONFIGSERVER_H
