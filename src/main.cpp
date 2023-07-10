#include "WifiAPConfigServer.h"
#include "Settings.h"
#include "DMXController.h"
#include "UIController.h"

Settings dmx_settings;
WifiAPConfigServer config_server(dmx_settings);
DMXController dmx_controller(dmx_settings.GetBaseChannel());
UIController ui(dmx_settings, config_server);
// TODO: StepperMotorController motor_controller( MOTOR_ENABLE_PIN, MOTOR_DIRECTION_PIN, MOTOR_STEP_PIN);

void setup()
{
    ui.ToggleAP(); // TODO: remove for production
}

void loop()
{
    ui.Update();

    if (dmx_controller.IsConnected())
    {
        if (dmx_controller.ReceiveNewMessage())
        {
            uint16_t position = dmx_controller.GetPosition();
            uint8_t direction = dmx_controller.GetDirection();
            uint8_t speed = dmx_controller.GetSpeed();

            // Work the motor according to the mode
            uint8_t mode = dmx_settings.GetMode();
            switch (mode)
            { // TODO: Implement this with an enum?
            case 0:
                // Manual Mode
                ESP_LOGI("main", "Mode: %d - Manual", mode);
                break;
            case 1:
                // Other mode
                ESP_LOGI("main", "Mode: %d - Auto", mode);
                break;
            case 2:
                // Yet another mode
                ESP_LOGI("main", "Mode: %d - Custom", mode);
                break;
            default:
                // Invalid mode -  this should never happen
                // TODO: Emergency Stop??
                // TODO: Display Error
                ESP_LOGE("main", "Invalid mode: %d", mode);
                break;
            }
        }
    }
    else
    {
        // Emergency Stop!!
        // Display Error uiController.displayError("no DMX signal")
    }
}