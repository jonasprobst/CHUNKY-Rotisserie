#include "NVSStorage.h"
#include <esp_log.h>

// FIXME: Uses a lot of duplicate code. Should probably be refactored to use templates.

// For logging, we need a tag
static constexpr const char TAG[] = "NVSStorage";

NVSStorage::NVSStorage()
{
  nvs_namespace_ = "settings";
  InitNVS();
}

NVSStorage::NVSStorage(const String &nvs_namespace)
    : nvs_namespace_(nvs_namespace) {}

NVSStorage::~NVSStorage()
{
  CloseNVS();
}

void NVSStorage::InitNVS()
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
}

uint8_t NVSStorage::LoadMode()
{
  if (!OpenNVS())
  {
    return DEFAULT_MODE;
  }

  uint8_t mode = DEFAULT_MODE;
  bool load_success = LoadValue("mode", mode);

  if (load_success)
  {
    ESP_LOGI(TAG, "Successfully loaded mode: %d", mode);
  }
  else
  {
    // This happens if the settings have never been saved before, hence logged as info not error.
    ESP_LOGI(TAG, "Failed to load mode. Using default.");
  }

  CloseNVS();
  return mode;
}
uint16_t NVSStorage::LoadBaseChannel()
{
  if (!OpenNVS())
  {
    return DEFAULT_BASE_CHANNEL;
  }

  uint16_t base_channel = DEFAULT_BASE_CHANNEL;
  bool load_success = LoadValue("baseChannel", base_channel);

  if (load_success)
  {
    ESP_LOGI(TAG, "Successfully loaded base channel: %d", base_channel);
  }
  else
  {
    // This happens if the settings have never been saved before. Hence logged as info not error.
    ESP_LOGI(TAG, "Failed to load base channel. Using default.");
  }

  CloseNVS();
  return base_channel;
}

bool NVSStorage::SaveMode(uint8_t mode)
{
  if (!OpenNVS())
  {
    return false;
  }

  bool save_success = SaveValue("mode", mode);

  if (!save_success)
  {
    ESP_LOGE(TAG, "Failed to save mode");
  }

  CloseNVS();
  return save_success;
}
bool NVSStorage::SaveBaseChannel(uint16_t base_channel)
{
  if (!OpenNVS())
  {
    return false;
  }

  bool save_success = SaveValue("baseChannel", base_channel);

  if (!save_success)
  {
    ESP_LOGE(TAG, "Failed to save mode");
  }

  CloseNVS();
  return save_success;
}

bool NVSStorage::OpenNVS()
{
  esp_err_t err = nvs_open(nvs_namespace_.c_str(), NVS_READWRITE, &nvs_handle_);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to open NVS. Error code: %d", err);
    nvs_handle_valid_ = false;
  }
  else
  {
    nvs_handle_valid_ = true;
  }
  return (err == ESP_OK);
}

void NVSStorage::CloseNVS()
{
  nvs_close(nvs_handle_);
  nvs_handle_valid_ = false;
}

template <typename T>
bool NVSStorage::SaveValue(const String &key, const T &value)
{
  if (!nvs_handle_valid_)
  {
    ESP_LOGE(TAG, "Attempted to save value with invalid NVS handle");
    return false;
  }

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
  if (!nvs_handle_valid_)
  {
    ESP_LOGE(TAG, "Attempted to load value with invalid NVS handle");
    return false;
  }
  
  size_t required_size = sizeof(value);
  esp_err_t err = nvs_get_blob(nvs_handle_, key.c_str(), &value, &required_size);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to load value with key %s. Error code: %d", key.c_str(), err);
  }
  return (err == ESP_OK);
}
