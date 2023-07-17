#include "PinConfig.h"
#include "WifiAPConfigServer.h"
#include "Settings.h"
#include "DMXController.h"
#include "UIController.h"
#include "MotorController.h"

String TAG = TAG; // ESP_LOGx Tag
Settings dmx_settings;
WifiAPConfigServer config_server(dmx_settings);
DMXController dmx_controller(dmx_settings.GetBaseChannel());
UIController ui(dmx_settings, config_server);
MotorController motor_controller;
MotorController::Direction motor_direction = MotorController::CLOCKWISE;

uint8_t position = 0;
uint8_t max_speed = 0;
uint8_t cw_speed = 0;
uint8_t ccw_speed = 0;
uint8_t motor_mode = 1; // Continuous rotation mode

void setup()
{
    ui.DisplayMessage("starting AP...");
    ui.ToggleAP(); // TODO: remove for production

    motor_controller.SetMode(MotorController::MANUAL);
    motor_controller.SetSpeed(200);
    motor_controller.Enable();
}

void loop()
{
    // TODO: remove following DEBUG LOOP
    while (true)
    {
        motor_controller.SetDirection(MotorController::CLOCKWISE);
        for (int i = 0; i < 100; i++)
        {
            motor_controller.Step();
            delay(100);
        }
        motor_controller.Stop();
        delay(1000);
        motor_controller.SetDirection(MotorController::COUNTERCLOCKWISE);
        for (int i = 0; i < 100; i++)
        {
            motor_controller.Step();
            delay(100);
        }
        motor_controller.Stop();
        delay(1000);

        dmx_controller.Update();
        if(dmx_controller.IsConnected()){
            ESP_LOGI(TAG,"DMX Position: %d", dmx_controller.GetPosition());
            ESP_LOGI(TAG,"DMX Max Speed: %d", dmx_controller.GetMaxSpeed());
        }
    }

    ui.Update();
    dmx_controller.Update();

    if (!dmx_controller.IsConnected())
    {
        // TODO: Emergency stop + Update UI
        // ui.displayError("no DMX signal");
        motor_controller.Stop();
    } 

    // All is good. Let's get the latest DMX messages
    position = dmx_controller.GetPosition();
    max_speed = dmx_controller.GetMaxSpeed();
    cw_speed = dmx_controller.GetSpeedCW();
    ccw_speed = dmx_controller.GetSpeedCCW();
    motor_mode = dmx_controller.GetMotorMode();

    // work the motor according to DMX instructions
    switch (motor_mode)
    {
    case 1:
        // Continuous Rotation Mode
        // TODO: complete this
        //       work the modes in functions and only pass the relevant parameters
        //       e.g. executeContinuousRotation(max_speed, cw_speed, ccw_speed)
        motor_controller.SetMode(MotorController::MANUAL);
        motor_controller.SetSpeed(max_speed);
        motor_controller.Step(); // continue stepping
        break;
    case 2:
        // Mode 2 - blablabla
        // TODO: implement other modes
        break;
    case 3:
        // Mode 3 - blablabla
        // TODO: implement other modes
        break;
    default:
        // Invalid mode -  this should never happen
        // TODO: Emergency Stop??
        // TODO: Display Error
        ESP_LOGE(TAG, "Invalid motor mode: %d", motor_mode);
        ui.DisplayError("Invalid motor mode");
        break;
    }
}