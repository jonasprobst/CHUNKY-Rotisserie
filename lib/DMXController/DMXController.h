#ifndef DMXCONTROLLER_H
#define DMXCONTROLLER_H

#include <esp_dmx.h>
#include <Arduino.h>

/**
 * @class DMXController
 * @brief DMX Controller for ESP_DMX library.
 *
 * This class serves as a wrapper for the ESP_DMX library.
 * It extends the library to set a base channel and retrieve
 * the values of the subsequent channels.
 */

#define SIZE 3  // The size of this device's DMX footprint.
#define TRANSMIT_PIN 17 // UART2 TX Pin.
#define RECEIVE_PIN 16  // UART2 RX Pin.
#define ENABLE_PIN 21  // UART2 RTS Pin.

class DMXController
{
public:
  /**
   * @brief Construct a new DMX Controller object.
   */
  DMXController(uint8_t offset);

  /**
   * @brief Destroy the DMX Controller object.
   */ 
  ~DMXController();

  /**
   * @brief Recieve the DMX data.
   * 
   *  @return true if data was recieved, false otherwise.
  */
  bool recieveNewMessage();

  /**
   * @brief Retrieve the position value from the DMX data.
   *
   * @return The position value.
   */
  uint16_t getPosition();

  /**
   * @brief Retrieve the direction value from the DMX data.
   *
   * @return The direction value.
   */
  int getDirection();

  /**
   * @brief Retrieve the speed value from the DMX data.
   *
   * @return The speed value.
   */
  int getSpeed();

  /**
   * @brief Check if the DMX connection is active.
   *
   * @return true if the connection is active, false otherwise.
   */
  bool isConnected();

private:
  const uint8_t offset_ = 0;  // The start address of this device (0...(512-SIZE))
  uint16_t position_ =0; // The position of the motor (0...65535)
  uint8_t direction_ = 0; // 0 = forward, 1 = reverse
  uint8_t speed_ = 0; // 0 = slow, 512 = fast
  dmx_port_t dmx_port_ = 1; // The UART port to use (1 or 2). 0 is used by the console.
  uint8_t data_[SIZE];  // The DMX data for this device.
  //esp_dmx dmx_interface_; // The DMX interface.
  dmx_config_t config_ = DMX_CONFIG_DEFAULT;
  bool dmx_is_connected_ = false;
  unsigned long last_update_ = millis();
};

#endif
