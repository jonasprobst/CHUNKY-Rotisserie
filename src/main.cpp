#include "WifiAPConfigServer.h"
#include "Settings.h"
#include "DMXController.h"
#include "UIController.h"
#include "MotorController.h"
#include <esp_log.h>

constexpr const char TAG[] = "Main";

Settings dmx_settings;
WifiAPConfigServer config_server(dmx_settings);
DMXController dmx_controller(dmx_settings.GetBaseChannel());
UIController ui(dmx_settings, config_server);
MotorController motor_controller(dmx_settings.GetMode());

uint16_t target_position_dmx = 0;
uint8_t user_max_speed_dmx = 0;
uint8_t cw_speed_dmx = 0;
uint8_t ccw_speed_dmx = 0;
uint8_t operation_mode_dmx = 0;

unsigned long last_update = 0;

void setup()
{
    ui.DisplayMessage("starting AP...");
    ui.ToggleAP(); // TODO: remove for production once buttons are implemented
}

void loop()
{
    ui.Update();
    dmx_controller.Update();

    if (dmx_controller.IsConnected() == 1)
    {
        // TODO: DMX Connection lost!! (or not yet established) - do we need a grace period? (Timeout after 1s)
        // Emergency stop + Update UI ?
        // ui.displayError("no DMX signal");
        // motor_controller.Stop();
        // reset everything to 0?
        // see: .pio/libdeps/debug_thing/esp_dmx/examples/DMXRead/DMXRead.ino
        // Je nach mode würde es wohl Sinn machen den Motor zu stoppen.

        if (millis() >= last_update + 1000) // warum nur jede sekunde updaten? Scheint mir etwas selten. DMX wird ca mit 30-45 FPS rausgesendet. -> grob alle 25 ms
        {
            last_update = millis();

            // These log when called
            target_position_dmx = dmx_controller.GetTargetPosition();
            user_max_speed_dmx = dmx_controller.GetMaxSpeed();
            cw_speed_dmx = dmx_controller.GetCWSpeed();
            ccw_speed_dmx = dmx_controller.GetCCWSpeed();
            operation_mode_dmx = dmx_controller.GetOperationMode();

            // handle motor
            motor_controller.SetOperationMode(operation_mode_dmx);
            motor_controller.SetTargetPosition(target_position_dmx);
            motor_controller.SetUserMaxSpeed(user_max_speed_dmx);

            // set direction and speed (relevant for continuous rotation mode only)
            // wäre allenfalls zu Lösen/Vereinfachen mit nur einer Speed Addresse -> 128 entspricht 0 darüber CW darunter CCW
            // ich schätze 128 speed abstufungen in einer richtung reichen?
            if (cw_speed_dmx > 0 && ccw_speed_dmx > 0)
            {
                // FIXME: what should we do here? stop the motor? @demi
                // or ignore and just work with what ever value is bigger?
                ESP_LOGE(TAG, "Both CW and CCW speed are set. This is not supported.");
                motor_controller.SetSpeed(0);
            }
            else if (cw_speed_dmx >= ccw_speed_dmx)
            {
                ESP_LOGE(TAG, "turning cw.");
                motor_controller.SetSpeed(cw_speed_dmx);
                motor_controller.SetDirectionCW();
            }
            else
            {
                ESP_LOGE(TAG, "turning ccw.");
                motor_controller.SetSpeed(ccw_speed_dmx);
                motor_controller.SetDirectionCCW();
            }

            ESP_LOGI(TAG, "----"); // make the log look nicer
        }
    }
}