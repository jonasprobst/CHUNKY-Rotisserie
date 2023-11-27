#include "PinConfig.h"
#include "DMXController.h"
#include <esp_log.h>

static constexpr const char TAG[] = "DMXController";

DMXController::DMXController(uint16_t base_channel)
    : base_channel_(base_channel), is_connected_(false)
{
  // Initialise DMX Connection
  dmx_set_pin(dmx_port_, DMX_TX, DMX_RX, DMX_ENABLE);
  dmx_driver_install(dmx_port_, DMX_DEFAULT_INTR_FLAGS);
}

uint16_t DMXController::GetTargetPosition()
{
  // GetChannelValue returns the second argument if an error occured reading DMX data.
  // here this prevents the motor from moving unintentionally towards a new position
  position_hb_ = GetChannelValue(POSITION_CHANNEL_HB, position_hb_);
  position_lb_ = GetChannelValue(POSITION_CHANNEL_LB, position_lb_);
  target_position_ = position_lb_ + (position_hb_ << 8);
  ESP_LOGI(TAG, "Target Position value (16b): %d", target_position_);
  return target_position_;
}

uint8_t DMXController::GetMaxSpeed()
{
  // GetChannelValue retruns 0 on error to stop the motor.
  max_speed_ = GetChannelValue(MAX_SPEED_CHANNEL, 0);
  ESP_LOGI(TAG, "Max Speed value: %d", max_speed_);
  return max_speed_;
}

uint8_t DMXController::GetCWSpeed()
{
  // GetChannelValue returns 0 on error to stop the motor
  cw_speed_ = GetChannelValue(CW_SPEED_CHANNEL, 0);
  ESP_LOGI(TAG, "CW Speed value: %d", cw_speed_);
  return cw_speed_;
}

uint8_t DMXController::GetCCWSpeed()
{
  // GetChannelValue returns 0 on error to stop the motor
  ccw_speed_ = GetChannelValue(CCW_SPEED_CHANNEL);
  ESP_LOGI(TAG, "CCW Speed value: %d", ccw_speed_);
  return ccw_speed_;
}

uint8_t DMXController::GetOperationMode()
{
  // GetChannelValue returns the last known operation mode on error.
  // this is to prevent the motor from moving unintentionally
  operation_mode_ = GetChannelValue(OPERATION_MODE_CHANNEL, operation_mode_);
  ESP_LOGI(TAG, "Operation Mode value: %d", operation_mode_);
  return operation_mode_;
}

uint8_t DMXController::GetChannelValue(uint16_t channel, uint8_t default_value)
{
  // Check if DMX is connected and channel in range
  // On error, return default value to prevent motor from moving unintentionally
  // if default value is not set, 0 will be returned
  if (!IsConnected())
  {
    ESP_LOGE(TAG, "Error: DMX not connected.");
    return default_value;
  }
  else if (channel < 1 || channel > DMX_PACKET_SIZE)
  {
    ESP_LOGE(TAG, "Error: Invalid channel number: %d", channel);
    return default_value;
  }
  // Return the value of the requested channel offset by base channel
  return channel_values_[base_channel_ + channel -1];
}

bool DMXController::IsConnected()
{
  return is_connected_;
}

bool DMXController::Update()
{
  dmx_packet_t packet;

  // TODO: make this non-blocking if possible! 
  // Try to receive a DMX package (this blocks until one is receive or it times out)
  if (dmx_receive(dmx_port_, &packet, DMX_TIMEOUT_TICK) == 0)
  {
    is_connected_ = false;
    ESP_LOGE(TAG, "Timed out waiting for DMX packet. Error: %s", esp_err_to_name(packet.err));
    return false;
  }

  // Check if package was received successfully and without errors.
  if (packet.err != ESP_OK)
  {
    is_connected_ = false;
    ESP_LOGE(TAG, "Error receiving DMX packet: %s", esp_err_to_name(packet.err));
    return false;
  }

  // Package received succesfully. Read the channel values.
  is_connected_ = true;
  dmx_read(dmx_port_, channel_values_, packet.size);
  return true;
}