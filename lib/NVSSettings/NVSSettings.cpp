#include "NVSSettings.h"


NVSSettings::NVSSettings() {
  preferences.begin("my-app", false);
  // If we've stored settings before, load them.
  if (preferences.getUInt("validKey", 0) == validKeyValue) {
      rootChannel = preferences.getUInt("rootChannel", DEFAULT_ROOT_CHANNEL);
      mode = preferences.getUInt("mode", DEFAULT_MODE);
  }
}

NVSSettings::~NVSSettings() {
  preferences.end();
}

unsigned int NVSSettings::getRootChannel() const {
  return rootChannel;
}

unsigned int NVSSettings::getMode() const {
  return mode;
}

void NVSSettings::save(unsigned int rootChannel, unsigned int mode) {
  this->rootChannel = rootChannel;
  this->mode = mode;
  preferences.putUInt("rootChannel", rootChannel);
  preferences.putUInt("mode", mode);
  preferences.putUInt("validKey", validKeyValue);
}
