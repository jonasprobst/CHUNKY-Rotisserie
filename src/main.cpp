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
    
    if (dmx_controller.IsConnected() == 1)
    {
        // TODO: DMX Connection lost!! (or not yet established) - do we need a grace period? (Timeout after 1s)
        // Emergency stop + Update UI ?
        // ui.displayError("no DMX signal");
        // motor_controller.Stop();
        // reset everything to 0?
        // see: .pio/libdeps/debug_thing/esp_dmx/examples/DMXRead/DMXRead.ino

        if (millis() >= last_update + 1000)
        {
            last_update = millis();

            // These log when called
            target_position = dmx_controller.GetTargetPosition();
            max_speed = dmx_controller.GetMaxSpeed();
            cw_speed = dmx_controller.GetCWSpeed();
            ccw_speed = dmx_controller.GetCCWSpeed();
            operation_mode = dmx_controller.GetOperationMode();

            //handle motor
            motor_controller.SetOperationMode(operation_mode);
            motor_controller.SetTargetPosition(target_position); 
            motor_controller.SetMaxSpeed(max_speed);

            // set direction and speed (relevant for continuous rotation mode only)
            if(cw_speed > 0 && ccw_speed > 0) {
                // what should we do here? stop the motor?
                ESP_LOGE(TAG, "Both CW and CCW speed are set. This is not supported.");
            } else if(cw_speed > 0) {
                motor_controller.SetSpeed(cw_speed);
                motor_controller.SetDirectionCW();
            } else if(ccw_speed > 0) {
                motor_controller.SetSpeed(ccw_speed);
                motor_controller.SetDirectionCCW();
            } else {
                // both speeds are 0, guess we're in a different mode
                // anything to do here?
            }
            
            ESP_LOGI(TAG, "----"); // make the log look nicer
        }
    }
}