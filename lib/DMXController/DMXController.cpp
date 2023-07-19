#include "PinConfig.h"
#include "DMXController.h"
#include <esp_log.h>

static const char *TAG = "DMXController";

DMXController::DMXController(uint8_t base_channel)
    : base_channel_(base_channel), is_connected_(false)
{
  // Initialise DMX Connection
  ESP_LOGI(TAG, "Port enable: %s", dmx_set_pin(dmx_port_, DMX_TX, DMX_RX, DMX_ENABLE));
  ESP_LOGI(TAG, "Driver install: %s", dmx_driver_install(dmx_port_, DMX_DEFAULT_INTR_FLAGS));
}

DMXController::~DMXController()
{
  // nothing to do here
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
  return Read(MAX_SPEED_CHANNEL);
}

uint8_t DMXController::GetCWSpeed()
{
  return Read(CW_SPEED_CHANNEL);
}

uint8_t DMXController::GetCCWSpeed()
{
  return Read(CCW_SPEED_CHANNEL);
}

uint8_t DMXController::GetMotorMode()
{
  // If an error occurred receiving the new mode, the last mode will be returned.
  // this is to prevent the motor from moving unintentionally
  last_motor_mode_ = Read(MOTOR_MODE_CHANNEL);
  return last_motor_mode_;
}

uint8_t DMXController::Read(uint8_t channel, uint8_t default_value)
{
  // Check if DMX is connected and channel in range
  if (!IsConnected() || channel < 1 || channel > NUM_CHANNELS)
  {
    ESP_LOGE(TAG, "DMX Signal lost or invalid channel number: %d", channel);
    return default_value;
  }
  return data_[channel - 1];
}

bool DMXController::IsConnected()
{
  ESP_LOGE(TAG, "IsConnected: %d", is_connected_);
  return is_connected_;
}

bool DMXController::Update()
{
  dmx_packet_t packet;
  ESP_LOGI(TAG, "Update. DMX Port: %d", dmx_port_);
  // Try to receive a DMX package (this blocks until one is receive or it times out)
  if (dmx_receive(dmx_port_, &packet, DMX_TIMEOUT_TICK))
  {
    // Check if package was received successfully and without errors.
    if (packet.err == ESP_OK)
    {
      // store data in temporary variables so we can minimize the time we use mutex later
      dmx_read(dmx_port_, data_, NUM_CHANNELS);
      is_connected_ = true;
      return true;
    }
    else
    {
      is_connected_ = false;
      ESP_LOGE(TAG, "Error receiving DMX packet: %s", esp_err_to_name(packet.err));
    }
  }
  else
  {
    is_connected_ = false;
    ESP_LOGE(TAG, "Timed out waiting for DMX packet");
    ESP_LOGI(TAG, "Update. Packet:", packet);
  }
  return false;
}