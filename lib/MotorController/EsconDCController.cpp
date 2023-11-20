#include "PinConfig.h"
#include "EsconDCController.h"
#include <Arduino.h>

static constexpr const char TAG[] = "EsconDCController";

EsconDCController::EsconDCController(uint8_t motor_mode)
{
    // IMPORTANT: Parameter motor_mode is ignored since the ramp is set via potentiometer on the ESCON board.

    //TODO: Initialise ESCON connection (pins etc.)

    SetOperationMode(MODE_STOP);
    DisableMotor();
    SetDirectionCW();
    SetSpeed(0);
}

EsconDCController::~EsconDCController()
{
    Stop();
    DisableMotor();
}

void EsconDCController::SetMotorMode(uint8_t motor_mode)
{
    // IMPORTANT: this is not the same as the operation mode (see SetOperationMode for details)
    // Motor mode sets the ramp (up and down) of the stepper motor. It is set via Webserver.
    // - 0: Neutral function – motor stops
    // - 1: Slow
    // - 2: Normal
    // - 3: Fast

    // WARNING: In the current setup the ramp speed is set via the potentiometer on the ESCON board.
    // Changes to the parameter motor_mode have absolutley no effect.
    // A future feature would be to set the ramp via an analog output (PWM). The ESCON board supports this, 
    // but would need to be reconfigured via the ESCON Studio software.

    ESP_LOGW(TAG, "Motor mode is set with the potentiometer on the ESCON board.");
    /*
    switch (motor_mode)
    {
    case (0, 2): // Mode Stop or Normal Speed Change (Ramp)
        ESP_LOGI(TAG, "Motor mode (ramp): normal");
        // Implementation goes here :-)
        break;

    case (1): // Mode Slow Speed Change (Ramp)
        ESP_LOGI(TAG, "Motor mode (ramp): slow");
        // Implementation goes here :-)
        break;

    case (3): // Mode Fast Speed Change (Ramp)
        ESP_LOGI(TAG, "Motor mode (ramp): fast.");
        // Implementation goes here :-)
        break;

    default:
        ESP_LOGE(TAG, "Invalid motor mode %d.", motor_mode);
        break;
    }
    */
}

void EsconDCController::SetOperationMode(uint8_t operation_mode)
{
    // IMPORTANT: this is not the same as motor mode (see SetMotorMode for details)
    // Operation mode is set as a percentage of the DMX Channel 6 value.

    // WARNING: The Escon 36/2 DC only supports continuous rotation (operation_mode 1 - 50%)
    // THerefore values passed by operation_mode are ignored in this implementation.
    
    if (operation_mode == MODE_STOP){
        operation_mode_ = MODE_STOP;
        ESP_LOGI(TAG, "Operation mode set to STOP.");
    } else if (operation_mode == MODE_ROTATION){
        operation_mode_ = MODE_ROTATION;
        ESP_LOGI(TAG, "Operation mode set to ROTATION.");
    } else {
        operation_mode_ = MODE_ROTATION;
        ESP_LOGW(TAG, "Operation mode set to ROTATION. Other modes are not supported.");
    }
}

void EsconDCController::SetMaxSpeed(uint8_t speed)
{
    // Map the speed percentage to the range 0 to MOTOR_MAX_SPEED
    max_speed_ = map(speed, 0, 255, 0, MOTOR_MAX_SPEED);
    ESP_LOGI(TAG, "Max speed set to %f", max_speed_);
}

void EsconDCController::SetSpeed(uint8_t speed)
{
    // Map the speed percentage to the range 0 to current max_speed_
    speed_ = map(speed, 0, 255, 0, max_speed_);
    ESP_LOGI(TAG, "Speed set to %f", speed_);
}

void EsconDCController::SetCWLimitPosition()
{
    ESP_LOGW(TAG, "SetCWLimitPosition is not supported by this implementation.");
}

void EsconDCController::SetCCWLimitPosition()
{
    ESP_LOGW(TAG, "SetCCWLimitPosition is not supported by this implementation.");
}

void EsconDCController::SetTargetPosition(uint16_t position)
{
    ESP_LOGW(TAG, "SetTargetPosition is not supported by this implementation.");
}

void EsconDCController::Run()
{
    switch (operation_mode_)
    {
    case OperationMode::MODE_STOP:
        // The motor is stopped.
        if (IsRunning()){
            Stop();
        } else if (!IsRunning && IsEnabled()){
            DisableMotor();
        } else {
            // Do nothing
        }
        break;
    case OperationMode::MODE_ROTATION:
        // The motor rotates in the direction and speed set by channel 4/ 5.
        // The speed is a percentage of max speed set by channel 3.
        // - Channel 3: Set max speed
        // - Channel 4: Rotate CW relative to max speed. Channel 5 must be 0.
        // - Channel 5: Rotate CCW relative to max speed. Channel 4 must be 0.
        if (!IsEnabled()){
             EnableMotor();
        } 
        ContinuousRotation();
        break;
    default:
        // Technically this can't and shouldn't be called... I think
        ESP_LOGE(TAG, "Invalid operation mode");
    }
}

void EsconDCController::ContinuousRotation()
{
    // TODO: implement
    // if not enabled, enable to motor?
    // set direction
    // set speed
    // go

}

void EsconDCController::Stop()
{
    SetSpeed(0);
    ESP_LOGI(TAG, "Motor is stopping.");
}

void EsconDCController::EnableMotor()
{
        // TODO: Set MOTOR_ENABLE pin to High
        is_enabled_ = true;
        ESP_LOGI(TAG, "Motor enabled.");
}

void EsconDCController::DisableMotor()
{
        // TODO: Set MOTOR_ENABLE pin to Low
        is_enabled_ = false;
        ESP_LOGI(TAG, "Motor disabled.");
}

bool EsconDCController::IsEnabled()
{
    return is_enabled_;
}

void EsconDCController::SetDirectionCW()
{
    is_direction_cw_ = true;
    // TODO: set MOTOR_DIRECTION pin to low
    ESP_LOGI(TAG, "Direction set to CW");
}

void EsconDCController::SetDirectionCCW()
{
    is_direction_cw_ = false;
    // TODO: set MOTOR_DIRECTION pin to high
    ESP_LOGI(TAG, "Direction set to CCW");
}

bool EsconDCController::IsRunning()
{
    // We can read the speed comparator of the escon board to get an indication if the motor is running.
    // According to the manual "the digital output is set as soon as the *actual speed average* reaches
    // the preset speed for comparsion." This functionality has to be configured in the ESCON Studio software.
    // My configuration (see hardware.md) is set to once the speed is above 0 D4 out of escon board is set to high.

    // TODO: read D4 pin and return true if high
    bool is_running = false;
    ESP_LOGI(TAG, "Motor is running: %d", is_running);
    return is_running;
}
