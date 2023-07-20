#ifndef DMXCONTROLLER_H
#define DMXCONTROLLER_H

#include <esp_dmx.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

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
   */
  DMXController();

  /**
   * @brief Construct a new DMX Controller object.
   *
   * @param base_channel Set a base channel aka offset address to retrieve the channels from.
   *                     eg. 200 to retriev channel 201, 202, etc.
   */
  DMXController(uint8_t base_channel);

  /**
   * @brief Destroy the DMX Controller object.
   */
  ~DMXController();

  /**
   * @brief Retrieve the motor mode from the DMX data.
   * @note 0-79% Position mode, 51-54% Position mode + 'soft limit' save enabled, 80-100% Angular mode
   *
   * @return The operation mode.
   */
  uint8_t GetMotorMode();

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
   * @brief Retrieve the absolute speed from the DMX data. The absolut speed
   *        ranges from zero to max speed. Zero is stopped and 255 is
   *        max speed. Zero is stopped.
   *
   * @return The absolute speed.
   */
  uint8_t GetSpeed();

  /**
   * @brief Retrieve the direction from the DMX data.
   *
   * @return The direction (0 = CW, 1 = CCW).
   */
  uint8_t GetDirection();

/**
 * @brief: Retrieve the save limits flag from the DMX data. When in
 *         "Save limits"-Mode (Channel 6 set to 51-54%), the current Position
 *         of the motor should be saved as limit when Channel 4 or 5 is set to 0.
 * 
 * @param direction: 0 = CW, 1 = CCW
 * @return true if the flag is set, false otherwise.
 */

  bool savePositionAsLimit(uint8_t direction);

  /**
   * @brief Retrieve the value of a single channel from the DMX data.
   *
   * @param channel The channel aka DMX address to read.
   * @return The channel value or if there was an error, the default value.
   */
  uint8_t Read(uint8_t channel, uint8_t default_value = 0);

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
  static constexpr uint8_t NUM_CHANNELS = 6;        // Number of consecutive channels to read.
  static constexpr uint8_t POSITION_CHANNEL_HB = 1; // Position rough (High Byte of 16bit DMX Channel)
  static constexpr uint8_t POSITION_CHANNEL_LB = 2; // Position fine (Low Byte of 16bit DMX Channel)
  static constexpr uint8_t MAX_SPEED_CHANNEL = 3;   // Set the maximum speed
  static constexpr uint8_t CW_SPEED_CHANNEL = 4;    // Rotate CW and set soft CW limit
  static constexpr uint8_t CCW_SPEED_CHANNEL = 5;   // Rotate CCW and set soft CCW limit
  static constexpr uint8_t MOTOR_MODE_CHANNEL = 6;  // Modus Operandi

  dmx_port_t dmx_port_ = DMX_NUM_1; // The UART port to use. WARNING: Use the correct pin setup for this!
  uint8_t base_channel_ = 0;        // The base channel (aka dmx address aka offset) to read from.
  uint8_t data_[NUM_CHANNELS];      // The data array to store the read data in.
  bool is_connected_;               // Flag to indicate if the DMX connection is active.

  uint8_t position_hb_; // The high byte of the last received position
  uint8_t position_lb_; // The low byte of the last received position
  uint8_t motor_mode_;  // The last received motor mode
  uint8_t max_speed_;   // The last received max speed
  uint8_t speed_;            // The absolute speed
  uint8_t direction_;        // THe direction the motor is turning in (CW/CCW)

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
};

#endif // DMXCONTROLLER_H
