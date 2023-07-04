#include "NVSStorage.h"
#include <esp_log.h>

// For logging, we need a tag
static const char *TAG = "NVSStorage";

NVSStorage::NVSStorage()
{
  nvs_namespace_ = "settings";
}

NVSStorage::NVSStorage(const String &nvs_namespace)
    : nvs_namespace_(nvs_namespace) {}

NVSStorage::~NVSStorage()
{
  CloseNVS();
}

bool NVSStorage::SaveSettings(const SettingsInterface &settings)
{
  if (!OpenNVS())
  {
    return false;
  }

  bool save_success = SaveValue("baseChannel", settings.GetBaseChannel()) &&
                      SaveValue("mode", settings.GetMode());

  if (!save_success)
  {
    ESP_LOGE(TAG, "Failed to save settings");
  }

  CloseNVS();
  return save_success;
}

bool NVSStorage::LoadSettings(SettingsInterface &settings)
{
  if (!OpenNVS())
  {
    return false;
  }

  // q: what's the value range of uint8_t?
  uint16_t base_channel = 0;
  uint8_t mode = 0;

  bool load_success = LoadValue("baseChannel", base_channel) &&
                      LoadValue("mode", mode);

  if (load_success)
  {
    settings.SetBaseChannel(base_channel);
    settings.SetMode(mode);
  }
  else
  {
    //This will happen if the settings have never been saved before
    ESP_LOGE(TAG, "Failed to load settings. Using default.");
    settings.Reset();
  }

  CloseNVS();
  return load_success;
}

bool NVSStorage::OpenNVS()
{
  esp_err_t err = nvs_open(nvs_namespace_.c_str(), NVS_READWRITE, &nvs_handle_);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to open NVS. Error code: %d", err);
  }
  return (err == ESP_OK);
}

void NVSStorage::CloseNVS()
{
  nvs_close(nvs_handle_);
}

template <typename T>
bool NVSStorage::SaveValue(const String &key, const T &value)
{
  esp_err_t err = nvs_set_blob(nvs_handle_, key.c_str(), &value, sizeof(value));
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to save value with key %s. Error code: %d", key.c_str(), err);
    return false;
  }

  err = nvs_commit(nvs_handle_);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to commit changes. Error code: %d", err);
  }
  return true;
}

template <typename U>
bool NVSStorage::LoadValue(const String &key, U &value)
{
  size_t required_size = sizeof(value);
  esp_err_t err = nvs_get_blob(nvs_handle_, key.c_str(), &value, &required_size);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to load value with key %s. Error code: %d", key.c_str(), err);
  }
  return (err == ESP_OK);
}
