#include "DMXController.h"
#include "PinConfig.h"
#include <esp_log.h>

static const char *TAG = "DMXController";

DMXController::DMXController(uint8_t base_channel)
    : base_channel_(base_channel), is_connected_(false)
{
  // Initialise DMX Connection
  dmx_set_pin(dmx_port_, DMX_TX, DMX_RX, DMX_ENABLE);
  dmx_driver_install(dmx_port_, DMX_DEFAULT_INTR_FLAGS);
  // Creat Task and Mutex for async DMX reading
  xTaskCreate(DMXTask, "dmx_task", 2048, this, 10, &dmx_task_handle_);
  dmx_mutex_ = xSemaphoreCreateMutex();
}

DMXController::~DMXController()
{
  if (dmx_task_handle_ != NULL)
  {
    vTaskDelete(dmx_task_handle_);
  }
  if (dmx_mutex_ != NULL)
  {
    vSemaphoreDelete(dmx_mutex_);
  }
}

uint16_t DMXController::GetPosition()
{
  // If an error occurred receiving the new position, the last position will be returned.
  // this is to prevent the motor from moving unintentionally towards a new position
  last_position_hb_ = Read(POSITION_CHANNEL_HB, last_position_hb_);
  last_position_lb_ = Read(POSITION_CHANNEL_LB, last_position_lb_);
  return last_position_hb_ + (last_position_lb_ << 8);
}

uint8_t DMXController::GetMaxSpeed()
{
  return Read(MAX_SPEED_CHANNEL, 0);
}

uint8_t DMXController::GetSpeedCW()
{
  return Read(CW_SPEED_CHANNEL, 0);
}

uint8_t DMXController::GetSpeedCCW()
{
  return Read(CCW_SPEED_CHANNEL, 0);
}

uint8_t DMXController::GetMotorMode()
{
  // If an error occurred receiving the new mode, the last mode will be returned.
  // this is to prevent the motor from moving unintentionally
  last_motor_mode_ = Read(MOTOR_MODE_CHANNEL);
  return last_motor_mode_;
}

uint8_t DMXController::Read(uint8_t channel, uint8_t default_value = 0)
{
  // Check if DMX is connected and channel in range
  if (!IsConnected() || channel < 1 || channel > NUM_CHANNELS)
  {
    ESP_LOGE(TAG, "DMX Signal lost or invalid channel number: %d", channel);
    return default_value;
  }

  // Read DMX value from data array safely
  uint8_t value;
  xSemaphoreTake(dmx_mutex_, portMAX_DELAY);
  value = data_[channel - 1];
  xSemaphoreGive(dmx_mutex_);
  return value;
}

bool DMXController::IsConnected()
{
  // Check if DMX is connected safely
  bool result;
  xSemaphoreTake(dmx_mutex_, portMAX_DELAY);
  result = is_connected_;
  xSemaphoreGive(dmx_mutex_);
  ESP_LOGE(TAG, "IsConnected: %d", result);
  return result;
}

// Function to receive DMX Data asynchronously (FreeRTOS Task)
static void DMXTask(void *parameter)
{
  DMXController *instance = static_cast<DMXController *>(parameter);
  dmx_packet_t packet;
  uint8_t temp_data[NUM_CHANNELS];
  bool temp_is_connected;

  // Continuously receive DMX data.
  while (true)
  {
    // Try to receive a DMX package (blocks until one is receive or it times out)
    if (dmx_receive(instance->dmx_port_, &packet, DMX_TIMEOUT_TICK))
    {
      // Check if package was received successfully and without errors.
      if (packet.err == ESP_OK)
      {
        // store data in temporary variables so we can minimize the time we use mutex later
        dmx_read(instance->dmx_port_, temp_data, NUM_CHANNELS);
        temp_is_connected = true;
      }
      else
      {
        ESP_LOGE(TAG, "Error receiving DMX packet: %s", esp_err_to_name(packet.err));
        temp_is_connected = false;
      }
    }
    else
    {
      ESP_LOGE(TAG, "Timed out waiting for DMX packet");
      temp_is_connected = false;
    }

    // if it takes longer than 1s to take the mutex, try again later
    if (xSemaphoreTake(instance->dmx_mutex, pdMS_TO_TICKS(1000)) == pdTRUE)
    {
      memcpy(instance->data_, temp_data, NUM_CHANNELS);
      instance->is_connected_ = temp_is_connected;
      xSemaphoreGive(instance->dmx_mutex_);
    }
    else
    {
      ESP_LOGE(TAG, "Failed to acquire mutex");
    }
  }
}
