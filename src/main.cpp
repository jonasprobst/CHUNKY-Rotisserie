#include "WifiAPConfigServer.h"
#include "Settings.h"
#include "DMXController.h"
#include "UIController.h"
#include "MotorController.h"

Settings dmx_settings;
WifiAPConfigServer config_server(dmx_settings);
DMXController dmx_controller(dmx_settings.GetBaseChannel());
UIController ui(dmx_settings, config_server);
MotorController motor_controller;
MotorController::Direction motor_direction = MotorController::CLOCKWISE;

void setup()
{
    ui.DisplayMessage("starting AP...");
    ui.ToggleAP(); // TODO: remove for production

    motor_controller.Enable();
    motor_controller.SetSpeed(1000);
    motor_controller.SetDirection(motor_direction); //true for clockwise, false for counterclockwise
}

void loop()
{
    ui.Update();

    if (dmx_controller.IsConnected())
    {
        ESP_LOGI("main", "DMX connected");
        if (dmx_controller.ReceiveNewMessage())
        {
            ESP_LOGI("main", "Received new DMX message");
            uint16_t position = dmx_controller.GetPosition();
            uint32_t speed = dmx_controller.GetSpeed();
            uint8_t direction = dmx_controller.GetDirection();
            motor_direction = direction == 1 ? MotorController::CLOCKWISE : MotorController::COUNTERCLOCKWISE;

            // Work the motor according to the mode
            uint8_t mode = dmx_settings.GetMode();
            switch (mode)
            {
            case 1:
                // Mode 1 - Manually control the motor by speed and direction
                motor_controller.SetMode(MotorController::MANUAL);
                motor_controller.SetDirection(motor_direction);
                motor_controller.SetSpeed(speed);
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
                ESP_LOGE("main", "Invalid mode: %d", mode);
                ui.DisplayError("Invalid mode");
                break;
            }
        }
    }
    else
    {
        // TODO: Emergency Stop?! give it a grace period in case the DMX signal is lost momentarily
        //ui.displayError("no DMX signal");
    }
}