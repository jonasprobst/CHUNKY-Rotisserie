#ifndef DMXCONTROLLER_H
#define DMXCONTROLLER_H

#include <esp_dmx.h>

/**
 * @class DMXController
 * @brief DMX Controller for ESP_DMX library.
 *
 * This class serves as a wrapper for the ESP-DMX library. It extends the library to
 * set a base channel and retrieve the values of the subsequent channels.
 *
 * @warning the update() function must be called regularly in the main loop.
 */

class DMXController
{
public:
  /**
   * @brief Construct a new DMX Controller object.
   * @note The base channel (aka offset adress) is set to 1.
   */
  DMXController();

  /**
   * @brief Construct a new DMX Controller object.
   *
   * @param base_channel Set a base channel (aka offset address) to retrieve the channels from.
   *                     eg. 200 to retriev channel 201, 202, etc.
   */
  DMXController(uint16_t base_channel);

  /**
   * @brief Retrieve the motor mode from the DMX data.
   * @note 0-79% Position mode, 51-54% Position mode + 'soft limit' save enabled, 80-100% Angular mode
   *
   * @return The operation mode.
   */
  uint8_t GetOperationMode();

  /**
   * @brief Retrieve the 16bit position value from the DMX data. (Channel 1+2 combined)
   *
   * @return The 16bit position value.
   */
  uint16_t GetTargetPosition();

  /**
   * @brief Retrieve the max speed (speed limit) from the DMX data.
   *        Can't be higer than the max speed of the motor. Zero is stopped.
   *
   * @return The max speed (speed limit).
   */
  uint8_t GetMaxSpeed();

  /**
   * @brief Retrieve the limit clock wise speed value from the DMX data.
   * @note This is a percentage of the max speed. When in
   *       "Save soft limits"-Mode (Channel 6 set to 51-54%), the  Position
   *       (Channel 1+2) is saved as CW limit when this channel is set to 0.
   *
   * @return The clock wise speed value (% of max speed).
   */
  uint8_t GetCWSpeed();

  /**
   * @brief Retrieve the limit counter clock wise speed value from the DMX data.
   * @note This is a percentage of the max speed. When in
   *       "Save soft limits"-Mode (Channel 6 set to 51-54%), the Position
   *       (Channel 1+2) is saved as CCW limit when this channel is set to 0.
   *
   * @return The counter clock wise speed value (% of max speed).
   */
  uint8_t GetCCWSpeed();

  /**
   * @brief Retrieve the value of a single channel from the DMX data.
   *
   * @param channel The channel aka DMX address to read.
   * @return The channel value or if there was an error, the default value.
   */
  uint8_t GetChannelValue(uint16_t channel, uint8_t default_value = 0);

  /**
   * @brief Check if the DMX connection is active.
   *
   * @return true if the connection is active, false otherwise.
   */
  bool IsConnected();

  /**
   * @brief Update the DMX connection.
   * @warning This function must be called regularly in the main loop.
   */
  bool Update();

private:
  // Channel settings
  static constexpr uint8_t POSITION_CHANNEL_HB = 1;    // Position rough (High Byte of 16bit DMX Channel)
  static constexpr uint8_t POSITION_CHANNEL_LB = 2;    // Position fine (Low Byte of 16bit DMX Channel)
  static constexpr uint8_t MAX_SPEED_CHANNEL = 3;      // Set the maximum speed
  static constexpr uint8_t CW_SPEED_CHANNEL = 4;       // Rotate CW and set soft CW limit
  static constexpr uint8_t CCW_SPEED_CHANNEL = 5;      // Rotate CCW and set soft CCW limit
  static constexpr uint8_t OPERATION_MODE_CHANNEL = 6; // Modus Operandi

  dmx_port_t dmx_port_ = DMX_NUM_1; // The UART port to use. WARNING: Use the correct pin setup for this!
  uint16_t base_channel_ = 0;        // The base channel (aka dmx address aka offset) to read from.
  byte channel_values_[DMX_PACKET_SIZE];      // The array to store the read values in.
  bool is_connected_ = false;               // Flag to indicate if the DMX connection is active.

  uint8_t position_hb_;      // The high byte of the last received position
  uint8_t position_lb_;      // The low byte of the last received position
  uint16_t target_position_; // The target position
  uint8_t max_speed_;        // The last received max speed
  uint8_t cw_speed_;         // The last received clock wise speed
  uint8_t ccw_speed_;        // The last received counter clock wise speed
  uint8_t operation_mode_;   // The operation mode
};
#endif // DMXCONTROLLER_H
