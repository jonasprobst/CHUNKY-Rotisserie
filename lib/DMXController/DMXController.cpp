#include <Arduino.h>
#include "DMXController.h"
#include "esp_log.h"

//TODO: Use a diffrent dmx library (make sure its compatible with the shield tho) that uses interrupts to revieve dmx data.

#define NUM_CHANNELS 4 //Number of Channels used 

void DMXController::setBaseChannel(uint16_t base_channel)
{
  dmx_interface_.initRead(NUM_CHANNELS);
  base_channel_ = base_channel;
  last_packet_recieved_timestamp_ = millis();
}

uint16_t DMXController::getPosition() {
  byte lowerByte = dmx_interface_.read(base_channel_ + 1);
  byte upperByte = dmx_interface_.read(base_channel_ + 2);
  uint16_t position = ((upperByte << 8) | lowerByte);
  return position;
}

int DMXController::getDirection() {
  return dmx_interface_.read(base_channel_ + 3);
}

int DMXController::getSpeed() {
  return dmx_interface_.read(base_channel_ + 4);
}

bool DMXController::isConnected() {
  uint32_t currentTimestamp = millis();
  //FIXME: this won't work as SparFunkDMX return the stored values and is not interrupt triggered!
  if(dmx_interface_.read(NUM_CHANNELS) > 0){
      last_packet_recieved_timestamp_ = currentTimestamp;
  }
  return (currentTimestamp - last_packet_recieved_timestamp_) <= 1000;
}

void DMXController::refreshDMXData() {
  dmx_interface_.update();
}
