#include "WifiAPConfigServer.h"
#include "Settings.h"
#include "DMXController.h"
#include "UIController.h"
#include "EsconDCController.h"
#include <esp_log.h>

constexpr const char TAG[] = "Main";

Settings dmx_settings;
WifiAPConfigServer config_server(dmx_settings);
DMXController dmx_controller(dmx_settings.GetBaseChannel());
UIController ui(dmx_settings, config_server);
EsconDCController motor_controller(dmx_settings.GetMode());

uint16_t target_position = 0;
uint8_t max_speed = 0;
uint8_t cw_speed = 0;
uint8_t ccw_speed = 0;
uint8_t operation_mode = 0;

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
    motor_controller.Run();

    if (dmx_controller.IsConnected() == 1)
    {
        if (millis() >= last_update + 1000)
        {
            last_update = millis();

            // These log when called
            target_position = dmx_controller.GetTargetPosition();
            max_speed = dmx_controller.GetMaxSpeed();
            cw_speed = dmx_controller.GetCWSpeed();
            ccw_speed = dmx_controller.GetCCWSpeed();
            operation_mode = dmx_controller.GetOperationMode();

            // handle motor
            motor_controller.SetOperationMode(operation_mode);   // 0 = Stop, >1 = continuous rotation with escon controller
            motor_controller.SetTargetPosition(target_position); // has no effect with escon controller
            motor_controller.SetMaxSpeed(max_speed);

            // set direction and speed (relevant for continuous rotation mode only)
            if (cw_speed > 0 && ccw_speed > 0)
            {
                // FIXME: what should we do here? stop the motor? @demi
                // or ignore and just work with what ever value is bigger?
                ESP_LOGE(TAG, "Both CW && CCW Speed > 0. Stop.");
                motor_controller.Stop();
            }
            else if (cw_speed >= ccw_speed)
            {
                ESP_LOGI(TAG, "cw_speed %d >= ccw_speed %d", cw_speed, ccw_speed);
                motor_controller.SetDirectionCW();
                motor_controller.SetSpeed(cw_speed);
            }
            else
            {
                ESP_LOGI(TAG, "cw_speed %d < ccw_speed %d", cw_speed, ccw_speed);
                motor_controller.SetDirectionCCW();
                motor_controller.SetSpeed(ccw_speed);
            }

            ESP_LOGI(TAG, "----"); // make the log look nicer
        }
    }
    else
    {
        // TODO: DMX Connection lost!! (or not yet established) - do we need a grace period? (Timeout after 1s)
        // Emergency stop + Update UI ?
        // ui.displayError("no DMX signal");
        // motor_controller.Stop();
        // see: .pio/libdeps/debug_thing/esp_dmx/examples/DMXRead/DMXRead.ino
    }
}