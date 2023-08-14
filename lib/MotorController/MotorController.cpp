#include "PinConfig.h"
#include "MotorController.h"
#include <Arduino.h>

String TAG = "MotorController";

MotorController::MotorController(uint8_t motor_mode)
    : operation_mode_(MODE_STOP),
      direction_(DIRECTION_CW)
{
    SetMotorMode(motor_mode);
    SetupStepper();
}

MotorController::~MotorController()
{
    Stop();
    Disable();
    delete stepper_;
}

void MotorController::SetupStepper()
{
    stepper_ = new AccelStepper(AccelStepper::DRIVER, STEPPER_STEP, STEPPER_DIRECTION);
    stepper_->setEnablePin(STEPPER_ENABLE);
    stepper_->setPinsInverted(false, false, true); // Invert the enable pin (common-cathode wiring)
    stepper_->disableOutputs();
}

void MotorController::SetMotorMode(uint8_t motor_mode)
{
    // The motor mode is based on the Wahloberg Rotator (https://wahlberg.dk/products/dmx-rotators/dmx-rotator)
    // The following modes are supported:
    // 0: Stop
    // 1: Slow Speed Change (Ramp)
    // 2: Normal Speed Change (Ramp)
    // 3: Fast Speed Change (Ramp)
    // Modes 5 - 9 (manual modes) are not supported.
    // (TODO: Feature Idea: Add support for modes 5 - 9?)

    ESP_LOGI(TAG, "Motor mode set to %d", motor_mode);

    switch (motor_mode)
    {
    case (0, 2): // Mode Stop
        SetRamp(RAMP_NORMAL);
        break;

    case (1): // Mode Slow Speed Change (Ramp)
        SetRamp(RAMP_SLOW);
        break;

    case (3): // Mode Fast Speed Change (Ramp)
        SetRamp(RAMP_FAST);
        break;

    default:
        ESP_LOGE(TAG, "Invalid motor mode %d", motor_mode);
        SetRamp(RAMP_NORMAL);
        break;
    }
}

void MotorController::SetRamp(uint8_t ramp)
{
    ramp_ = ramp;
    ESP_LOGI(TAG, "Motor Mode (Ramp) set to %d", ramp_);
}

void SetOperationMode(uint8_t operation_mode)
{
    // Operation mode is determind by channel 6
    // Map it to percent for readability
    operation_mode = map(operation_mode,0, 255, 0, 100);

    // Set operation mode
    if(operation_mode > 1 && operation_mode < 49){
        operation_mode_ = MODE_ROTATION;
        ESP_LOGI(TAG, "Operation mode set to ROTATION ");
    } else if (operation_mode > 1 && operation_mode < 49){
        operation_mode_ = MODE_POSITION_SAVE;
        ESP_LOGI(TAG, "Operation mode set to POSITION SAVE ");
    } else if (operation_mode > 1 && operation_mode < 49){
        operation_mode_ = MODE_POSITION;
        ESP_LOGI(TAG, "Operation mode set to POSITION ");
    } else if (operation_mode > 1 && operation_mode < 49){
        operation_mode_ = MODE_ANGULAR;
        ESP_LOGI(TAG, "Operation mode set to ANGULAR ");
    } else {
        operation_mode_ = MODE_STOP;
        ESP_LOGI(TAG, "Operation mode set to STOP ");
    }
}

void MotorController::SetMaxSpeed(uint8_t speed)
{
    // Map the speed percentage to the range 0 to MOTOR_MAX_SPEED
    max_speed_ = map(speed, 0, 255, 0, MOTOR_MAX_SPEED);
    stepper_->setMaxSpeed(max_speed_);
    ESP_LOGI(TAG, "Max speed set to %d", max_speed_);
}

void MotorController::SetSpeed(uint8_t speed)
{
    // Map the speed percentage to the range 0 to current max_speed_
    speed_ = map(speed, 0, 255, 0, max_speed_);
    stepper_->setMaxSpeed(speed_);
    ESP_LOGI(TAG, "Speed set to %d", speed_);
}

// FIXME: Split this into two methods, one for each direction??
void MotorController::SaveLimitPosition(Direction direction)
{
    // set the current position as the limit for the given direction
    if (direction == DIRECTION_CW)
    {
        saved_cw_position_ = stepper_->currentPosition();
        ESP_LOGI(TAG, "Saved CW position %d", saved_cw_position_);
    }
    else
    {
        saved_ccw_position_ = stepper_->currentPosition();
        ESP_LOGI(TAG, "Saved CCW position %d", saved_ccw_position_);
    }
}

void MotorController::SetTargetPosition(uint16_t position)
{
    uint16_t current_position = stepper_->currentPosition();
    if (current_position != position)
    {
        target_position_ = position;
        ESP_LOGI(TAG, "Target position set to %d", position);
    }
}

void MotorController::Move()
{
    switch (operation_mode_)
    {
    case OperationMode::MODE_STOP:
        Stop();
        break;

    case OperationMode::MODE_ROTAION:
        // The motor rotates in the direction and speed set by channel 4/ 5. 
        // The speed is a percentage of max speed set by channel 3.
        // - Channel 3: Set max speed
        // - Channel 4: Rotate CW relative to max speed. Channel 5 must be 0.
        // - Channel 5: Rotate CCW relative to max speed. Channel 4 must be 0. */

        // move motor by speed and direction

        break;

    case OperationMode::MODE_POSITION_SAVE:
        // Save current position as a limit position
        // The motor rotates continuously in the direction set by channel 4/ 5 at the speed set by channel 3.
        // - Channel 3: Set max speed
        // - Channel 4: Rotate CW relative to max speed. Channel 5 must be 0.
        // - Channel 5: Rotate CCW relative to max speed. Channel 4 must be 0. */
        // The moment Channel 4 or 5 reach 0, the current position is saved as a limit position.
        // TODO: Implement
        saved_ccw_position_ = 0;
        saved_cw_position_ = 0;
        is_limits_saved_ = false;
        break;

    case OperationMode::MODE_POSITION:
        //
        // Ensure both limites were saved before moving to a position
        if (is_limits_saved_)
        {
            // Check if target is set
            // if it's not set, set it
            // Check if we are at pivot point (current position = target position)
            // if we are, swap target position
            // if not, keep moving
        }
        ESP_LOGI(TAG, "Motor mode set to POSITION SAVE");
        break;

    case OperationMode::MODE_ANGULAR:
        // Set target angle
        // MoveToAngle(target_angle_); // Create a safe method to move to a target angle

        break;

    default:
        ESP_LOGE(TAG, "Invalid operation mode");
        Stop(); // FIXME: is this the right action?
    }
}

// Stop the motor
void MotorController::Stop()
{
    stepper_->stop(); // sets speed, target and position to 0.
    ESP_LOGI(TAG, "Motor stopped");

    // TODO: needed? Check Axel stepper docu
    // get things back to the way they were before stopping so we can resume if needed
    SetSpeed(speed_);
    SetDirection(direction_);
}

// Enable the motor
void MotorController::Enable()
{
    // TODO: impplement this with a timer to prevent the motor from being enabled and disabled too quickly -> call stop?
    if (!is_enabled_)
    {
        stepper_->enableOutputs();
        is_enabled_ = true;
        ESP_LOGI(TAG, "Motor enabled.");
    }
}

// Disable the motor
void MotorController::Disable()
{
    // active low, due to the common-cathode wiring of TB6600
    if (is_enabled_)
    {
        stepper_->disableOutputs();
        is_enabled_ = false;
        ESP_LOGI(TAG, "Motor disabled.");
    }
}