#include "WifiAPConfigServer.h"
#include "Settings.h"
#include "DMXController.h"
#include "UIController.h"
//#include "MotorController.h"
#include <esp_log.h>

String TAG = "Main";

Settings dmx_settings;
WifiAPConfigServer config_server(dmx_settings);
DMXController dmx_controller(dmx_settings.GetBaseChannel());
UIController ui(dmx_settings, config_server);
//MotorController motor_controller(dmx_settings.GetMode());
//MotorController::OperationMode operation_mode = motor_controller.GetOperationMode();


uint16_t target_position = 0;
uint8_t max_speed = 0;
uint8_t cw_speed = 0;
uint8_t ccw_speed = 0;
uint8_t operation_mode = 0;

void setup()
{
    ui.DisplayMessage("starting AP...");
    ui.ToggleAP(); // TODO: remove for production
}

void loop()
{
    ui.Update();
    dmx_controller.Update();

    if (!dmx_controller.IsConnected())
    {
        // TODO: Emergency stop + Update UI
        // ui.displayError("no DMX signal");
        // motor_controller.Stop();
        // TODO: reset everything to 0;

    } 

    uint16_t new_target_position = dmx_controller.GetTargetPosition();
    uint8_t new_max_speed = dmx_controller.GetMaxSpeed();
    uint8_t new_cw_speed = dmx_controller.GetCWSpeed();
    uint8_t new_ccw_speed = dmx_controller.GetCCWSpeed();
    uint8_t new_operation_mode = dmx_controller.GetOperationMode();

    if (new_target_position != target_position)
    {
        target_position = new_target_position;
        ESP_LOGI(TAG, "New Target Position: %d", target_position);
    }
    if (new_max_speed != max_speed)
    {
        max_speed = new_max_speed;
        ESP_LOGI(TAG, "New Max Speed: %d", max_speed);
    }
    if (new_cw_speed != cw_speed)
    {
        cw_speed = new_cw_speed;
        ESP_LOGI(TAG, "New CW Speed: %d", cw_speed);
    }
    if (new_ccw_speed != ccw_speed)
    {
        ccw_speed = new_ccw_speed;
        ESP_LOGI(TAG, "New CCW Speed: %d", ccw_speed);
    }
    if (new_operation_mode != operation_mode)
    {
        operation_mode = new_operation_mode;
        ESP_LOGI(TAG, "New Operation Mode: %d", operation_mode);
    }
}