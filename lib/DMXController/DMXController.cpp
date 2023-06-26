#include <Arduino.h>
#include "DMXController.h"

#define NUM_CHANNELS 4 //Number of Channels used 

DMXController::DMXController(uint16_t baseChannel)
  : _baseChannel(baseChannel), 
    _lastPacketReceivedTimestamp(millis()) 
{
  _dmxInterface.initRead(_baseChannel, NUM_CHANNELS);  
}

uint16_t DMXController::getPosition() {
  byte lowerByte = _dmxInterface.read(_baseChannel + 1);
  byte upperByte = _dmxInterface.read(_baseChannel + 2);
  uint16_t position = ((upperByte << 8) | lowerByte);
  return position;
}

int DMXController::getDirection() {
  return _dmxInterface.read(_baseChannel + 3);
}

int DMXController::getSpeed() {
  return _dmxInterface.read(_baseChannel + 4);
}

bool DMXController::isConnected() {
  uint32_t currentTimestamp = millis();
  if(_dmxInterface.readPacket() > 0){
      _lastPacketReceivedTimestamp = currentTimestamp;
  }
  return (currentTimestamp - _lastPacketReceivedTimestamp) <= 1000;
}

void DMXController::refreshDMXData() {
  _dmxInterface.update();
}
