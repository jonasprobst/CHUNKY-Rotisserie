#include "NVSSettings.h"
#include <esp_log.h>

#define DEFAULT_BASE_CHANNEL 1
#define DEFAULT_MODE 1

NVSSettings::NVSSettings()
{
  _preferences.begin("my-app", false);
  // If we've stored settings before, load them.
  if (_preferences.getUInt("validKey", 0) == _validKeyValue)
  {
    _baseChannel = _preferences.getUInt("baseChannel", DEFAULT_BASE_CHANNEL);
    _mode = _preferences.getUInt("mode", DEFAULT_MODE);
    ESP_LOGD("NVSSettings:", "loaded: M: %d, BC: %d", _mode, _baseChannel);
  }
}

NVSSettings::~NVSSettings()
{
  _preferences.end();
}

uint16_t NVSSettings::getBaseChannel() const
{
  return _baseChannel;
}

uint16_t NVSSettings::getMode() const
{
  return _mode;
}

void NVSSettings::save( uint16_t baseChannel, uint16_t mode)
{
  _baseChannel = baseChannel;
  _mode = mode;
  _preferences.putUInt("baseChannel", baseChannel);
  _preferences.putUInt("mode", mode);
  _preferences.putUInt("validKey", _validKeyValue);
  ESP_LOGD("NVSSettings:", "Saved M: %d, BC: %d", _mode, _baseChannel);
}
