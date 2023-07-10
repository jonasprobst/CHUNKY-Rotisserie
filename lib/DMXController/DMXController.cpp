#include "DMXController.h"
#include <Arduino.h>
#include <esp_log.h>

static const char *TAG = "DMXController";

DMXController::DMXController(uint16_t base_channel) : base_channel_(base_channel)
{
  // Initialize the DMX driver.
  dmx_set_pin(dmx_port_, TRANSMIT_PIN, RECEIVE_PIN, ENABLE_PIN);
  dmx_driver_install(dmx_port_, &config_, DMX_INTR_FLAGS_DEFAULT);
}

DMXController::~DMXController()
{
  dmx_driver_delete(dmx_port_);
}

bool DMXController::ReceiveNewMessage()
{
  // wait for a new packet to be received.
  // If it takes longer than DMX_TIMEOUT_TICK (1250ms) return false.
  dmx_packet_t packet;
  if (dmx_receive(dmx_port_, &packet, DMX_TIMEOUT_TICK))
  {
    unsigned long now = millis();

    // check for DMX errors
    if (!packet.err)
    {
      // first connection
      if (!dmx_is_connected_)
      {
        dmx_is_connected_ = true;
        ESP_LOGI(TAG, "DMX connected");
      }

      // Read the DMX data.
      position_ = dmx_read_slot(dmx_port_, base_channel_ + 1);
      direction_ = dmx_read_slot(dmx_port_, base_channel_ + 2);
      speed_ = dmx_read_slot(dmx_port_, base_channel_ + 3);
      ESP_LOGI(TAG, "DMX data received.");
      ESP_LOGI(TAG, "Position: %d, Direction: %d, Speed: %d",
               position_, direction_, speed_);

      last_update_ = now;
      return true;
    }
    else
    {
      // A DMX error occurred. If it keeps happening repeatedly,
      // something is wrong with the code or the DMX transmitter.
      // For now, just log the error and keep going.
      ESP_LOGE(TAG, "DMX error: %d", packet.err);
      return false;
    }
  }
  else if (dmx_is_connected_)
  {
    // It's been a while since the last update (DMX_TIMEOUT_TICK),
    // assume the DMX cable has been unplugged.
    // TODO: Emergency stop in case the motor is still running!
    // TODO: maybe wait longer? check for a few more packets? personnel safety?
    ESP_LOGE(TAG, "DMX disconnected");
    dmx_is_connected_ = false;
    return false;
  }
  else
  {
    // No DMX data received, but we're not connected yet. Just keep going.
    return false;
  }
}

uint16_t DMXController::GetPosition()
{
  return position_;
}

uint8_t DMXController::GetDirection()
{
  return direction_;
}

uint8_t DMXController::GetSpeed()
{
  return speed_;
}

bool DMXController::IsConnected()
{
  return dmx_is_connected_;
}
