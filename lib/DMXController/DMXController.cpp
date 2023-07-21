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

uint16_t DMXController::GetTargetPosition()
{
  // If an error occurred receiving the new position, the last position will be returned.
  // this is to prevent the motor from moving unintentionally towards a new position
  position_hb_ = Read(POSITION_CHANNEL_HB, position_hb_);
  position_lb_ = Read(POSITION_CHANNEL_LB, position_lb_);
  return position_hb_ + (position_lb_ << 8);
}

uint8_t DMXController::GetMaxSpeed()
{
  // If an error occurred receiving the new max speed, 0 will be returned to stop the motor.
  max_speed_ = Read(MAX_SPEED_CHANNEL);
  return max_speed_;
}

uint8_t DMXController::GetMotorMode()
{
  // If an error occurred receiving the new mode, the last mode will be returned.
  // this is to prevent the motor from moving unintentionally
  motor_mode_ = Read(MOTOR_MODE_CHANNEL, motor_mode_);
  return motor_mode_;
}

uint8_t DMXController::GetSpeed()
{
  uint8_t cw_speed = Read(CW_SPEED_CHANNEL);
  uint8_t ccw_speed = Read(CCW_SPEED_CHANNEL);
  if (cw_speed > 0 && ccw_speed > 0)
  {
    // Unallowed state. Stop motor.
    return 0;
  }
  // set absolute value of speed
  (ccw_speed > 0) ? speed_ = ccw_speed : speed_ = cw_speed;
  ESP_LOGI(TAG, "Speed: %d", speed_);
  return speed_;
}

uint8_t DMXController::GetDirection()
{
  uint8_t cw_speed = GetCWSpeed();
  uint8_t ccw_speed = GetCCWSpeed();
  if (cw_speed > 0 && ccw_speed > 0)
  {
    // Unallowed state. Stop motor.
    return 0;
  }
  //TODO: return 1 or -1? or enum?
  (ccw_speed > 0) ? direction_ = 1 : direction_ = 0; // 0 = CW, 1 = CCW
  ESP_LOGI(TAG, "Direction: %d", direction_);
  return direction_;
}

uint8_t DMXController::Read(uint8_t channel, uint8_t default_value)
{
  // Check if DMX is connected and channel in range
  if (!IsConnected() || channel < 1 || channel > NUM_CHANNELS)
  {
    ESP_LOGE(TAG, "DMX Signal lost or invalid channel number: %d", channel);
    // Return default value to prevent motor from moving unintentionally
    // if default value is not set, 0 will be returned
    return default_value;
  }
  // Return the value of the requested channel
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

  // TODO: make this non-blocking if possible!
  // Try to receive a DMX package (this blocks until one is receive or it times out)
  if (dmx_receive(dmx_port_, &packet, DMX_TIMEOUT_TICK) > 0) // TODO: does this work?
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
  }

  // Package received succesfully. Read the data.
  dmx_read(dmx_port_, data_, NUM_CHANNELS);
  is_connected_ = true;
  ESP_LOGI(TAG, "DMX packet received successfully.");
  return true;
}

uint8_t DMXController::GetCWSpeed()
{
  // returns 0 on error to stop the motor
  uint8_t cw_speed = Read(CW_SPEED_CHANNEL);
  ESP_LOGI(TAG, "CW Speed: %d", cw_speed);
  return cw_speed;
}

uint8_t DMXController::GetCCWSpeed()
{
  // returns 0 on error to stop the motor
  uint8_t ccw_speed = Read(CCW_SPEED_CHANNEL);
  ESP_LOGI(TAG, "CCW Speed: %d", ccw_speed);
  return ccw_speed;
}