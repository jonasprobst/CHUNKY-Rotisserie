#ifndef DMXCONTROLLER_H
#define DMXCONTROLLER_H

#include <SparkFunDMX.h>

/**
 * @class DMXController
 * @brief DMX Controller for SparkFun DMX Arduino library.
 * 
 * This class serves as a wrapper for the SparkFun DMX Arduino library.
 * It extends the library's functionality, allowing you to set a base
 * channel and retrieve the values of the subsequent channels.
 */
class DMXController {
public:

  /**
   * @brief Construct a new DMX Controller object.
   *
   * @param baseChannel The base DMX channel (1-512).
   */
  void setBaseChannel(uint16_t baseChannel);

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

  /**
   * @brief Refresh the DMX data.
   */
  void refreshDMXData();

private:
  SparkFunDMX dmx_interface_; 
  uint16_t base_channel_ = 0; 
  uint32_t last_packet_recieved_timestamp_; 
};

#endif
