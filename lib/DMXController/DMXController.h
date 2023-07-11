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
 * the values of the subsequent channels. It's made for the
 * Sparkfun DMX-to-LED Shield.
 */

class DMXController
{
public:
  // Setup UART for communication via MAX485.
  static constexpr uint8_t UART_PORT = 2;      // The UART port to use. 0 is used by the console.
  static constexpr uint8_t RECEIVE_PIN = 16;  // UART2 RX Pin.
  static constexpr uint8_t TRANSMIT_PIN = 17; // UART2 TX Pin.
  static constexpr uint8_t ENABLE_PIN = 21;   // Pin to enable the MAX485.

  /**
   * @brief Construct a new DMX Controller object.
   *
   * @param base_channel Start DMX Address of this Device (1-512)
   */
  DMXController(uint16_t base_channel);

  /**
   * @brief Destroy the DMX Controller object.
   */
  ~DMXController();

  /**
   * @brief Receive the DMX data.
   *
   *  @return true if data was received, false otherwise.
   */
  bool ReceiveNewMessage();

  /**
   * @brief Retrieve the position value from the DMX data.
   *
   * @return The position value.
   */
  uint16_t GetPosition();

  /**
   * @brief Retrieve the direction value from the DMX data.
   *
   * @return The direction value.
   */
  uint8_t GetDirection();

  /**
   * @brief Retrieve the speed value from the DMX data.
   *
   * @return The speed value.
   */
  uint8_t GetSpeed();

  /**
   * @brief Check if the DMX connection is active.
   *
   * @return true if the connection is active, false otherwise.
   */
  bool IsConnected();

private:
  dmx_port_t dmx_port_ = UART_PORT; // The UART port to use. 0 is used by the console, 1 for flash
  const uint8_t base_channel_ = 0; // The start dmx address of this device (0...(512-SIZE))
  uint16_t position_ = 0;          // ch: offset +1. The position of the motor (0...65535)
  uint8_t direction_ = 0;          // ch: offset +2. 0 = forward, 1 = reverse
  uint8_t speed_ = 0;              // ch: offset +3. 0 = slow, 255 = fast
  uint8_t num_channels_ = 3;       // Number of (consecutive) channels used, see above. // TODO: should this bne in the settings?
  dmx_config_t config_ = DMX_CONFIG_DEFAULT;
  // TODO: check if a man config is needed. Personalities = modes? start address static(!)?
  /* dmx_config_t config_man_ = {
      .model_id = 0x0500,
      .product_category = 0x0700,
      .software_version_id = 0x0001,
      .current_personality = 1,
      .personalities = {{1, "Default Personality"}},
      .personality_count = 1,
      .dmx_start_address = 1,
  };*/
  bool dmx_is_connected_ = false;
  unsigned long last_update_ = millis();
};
#endif
