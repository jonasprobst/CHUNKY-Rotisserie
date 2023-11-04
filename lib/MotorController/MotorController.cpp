#include "PinConfig.h"
#include "MotorController.h"
#include <Arduino.h>


static constexpr const char TAG[] = "MotorController";

MotorController::MotorController(uint8_t motor_mode)
{
    SetupStepper();
    DisableMotor();
    SetMotorMode(motor_mode);
    SetOperationMode(MODE_STOP);
    SetDirectionCW();
    SetSpeed(0);
}

MotorController::~MotorController()
{
    Stop();
    DisableMotor();
    delete stepper_;
}

void MotorController::SetupStepper()
{
    stepper_ = new AccelStepper(AccelStepper::DRIVER, STEPPER_STEP, STEPPER_DIRECTION);
    stepper_->setEnablePin(STEPPER_ENABLE);
    stepper_->disableOutputs();                    // This dsiables the motor - it will not move until enabled
    stepper_->setPinsInverted(false, false, true); // Invert the enable pin (common-cathode wiring)
    stepper_->setMaxSpeed(MOTOR_MAX_SPEED);        // Set the max speed
    stepper_->setAcceleration(RAMP_NORMAL);        // Set the acceleration
    stepper_->setCurrentPosition(0);               // Set the current position
}

void MotorController::SetMotorMode(uint8_t motor_mode)
{
    // IMPORTANT: this is not the same as the operation mode (see SetOperationMode for details)
    // Motor mode sets the ramp (up and down) of the stepper motor. It is set via Webserver.
    // - 0: Neutral function – motor stops
    // - 1: Slow
    // - 2: Normal
    // - 3: Fast


    switch (motor_mode)
    {
    case (0, 2): // Mode Stop or Normal Speed Change (Ramp)
        stepper_->setAcceleration(RAMP_NORMAL);
        ESP_LOGI(TAG, "Motor mode (ramp): normal");
        break;

    case (1): // Mode Slow Speed Change (Ramp)
        stepper_->setAcceleration(RAMP_SLOW);
        ESP_LOGI(TAG, "Motor mode (ramp): slow");
        break;

    case (3): // Mode Fast Speed Change (Ramp)
        stepper_->setAcceleration(RAMP_FAST);
        ESP_LOGI(TAG, "Motor mode (ramp): fast");
        break;

    default:
        ESP_LOGE(TAG, "Invalid motor mode %d. Ramp set to normal.", motor_mode);
        stepper_->setAcceleration(RAMP_SLOW);
        break;
    }
}

void MotorController::SetOperationMode(uint8_t operation_mode)
{
    // IMPORTANT: this is not the same as motor mode (see SetMotorMode for details)
    // Operation mode is set as a percentage of the DMX Channel 6 value.
    // - 0-50% Continuous rotation mode
    // - 51-54% Position Mode (set limits enabled)
    // - 55-79% Position mode
    // - 80-100% Angular mode

    // Map the operation mode to the range 0 to 100
    operation_mode = map(operation_mode, 0, 255, 0, 100);

    // Set operation mode
    if (operation_mode >= 1 && operation_mode <= 50)
    { // TODO: should this start at 0?
        operation_mode_ = MODE_ROTATION;
        ESP_LOGI(TAG, "Operation mode set to ROTATION ");
    }
    else if (operation_mode >= 51 && operation_mode <= 54)
    {
        operation_mode_ = MODE_POSITION_SAVE;
        ESP_LOGI(TAG, "Operation mode set to POSITION SAVE ");
    }
    else if (operation_mode >= 55 && operation_mode <= 79)
    {
        operation_mode_ = MODE_POSITION;
        ESP_LOGI(TAG, "Operation mode set to POSITION ");
    }
    else if (operation_mode >= 80 && operation_mode <= 100)
    {
        operation_mode_ = MODE_ANGULAR;
        ESP_LOGI(TAG, "Operation mode set to ANGULAR ");
    }
    else
    {
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
    // setting max speed insted of speed so the stepper can handle acceleration itself
    stepper_->setMaxSpeed(speed_);
    ESP_LOGI(TAG, "Speed set to %d", speed_);
}

void MotorController::SetCWLimitPosition()
{
    // set the current absolute position as the limit for the given direction
    cw_limit_position_ = absolute_position_;
    ESP_LOGI(TAG, "CW limit position %d", cw_limit_position_);
}

void MotorController::SetCCWLimitPosition()
{
    // set the current absolute position as the limit for the given direction
    ccw_limit_position_ = absolute_position_;
    ESP_LOGI(TAG, "Saved CCW position %d", ccw_limit_position_);
}

void MotorController::SetTargetPosition(uint16_t position)
{
    target_position_ = position;
    stepper_->moveTo(target_position_);
    ESP_LOGI(TAG, "Target position set to %d", position);
}

void MotorController::Run()
{
    EnableMotor();
    switch (operation_mode_)
    {
    case OperationMode::MODE_STOP:
        // The motor is stopped.
        Stop();
        stepper_->run(); // needed to ramp down to full stop
        break;

    case OperationMode::MODE_ROTATION:
        // The motor rotates in the direction and speed set by channel 4/ 5.
        // The speed is a percentage of max speed set by channel 3.
        // - Channel 3: Set max speed
        // - Channel 4: Rotate CW relative to max speed. Channel 5 must be 0.
        // - Channel 5: Rotate CCW relative to max speed. Channel 4 must be 0.
        ContinuousRotation();
        break;

    case OperationMode::MODE_POSITION_SAVE:
        // The motor rotates like continuous rotation mode, but:
        // Once the motor stops, current absolute position is saved as a limit position.

        ContinuousRotation();

        if (!IsRunning() && is_direction_cw_)
        {
            SetCWLimitPosition();
        }
        else
        {
            SetCCWLimitPosition();
        }
        break;

    case OperationMode::MODE_POSITION:
        // The motor moves between the two set limit positions

        // Ensure both limites were set before starting position mode
        if (cw_limit_position_ == 0 && ccw_limit_position_ == 0)
        {
            ESP_LOGE(TAG, "No limits set. Can't move to position");
            Stop();
            break;
        }

        MoveBetweenLimitPositions();
        break;

    case OperationMode::MODE_ANGULAR:
        // TODO: Implement
        // Make sure motor is homed
        // Move to the chosen angle relative to home
        break;

    default:
        ESP_LOGE(TAG, "Invalid operation mode");
        Stop(); // FIXME: is this the right action?
    }
}

void MotorController::ContinuousRotation()
{
    // In order to let AccelStepper handel the ramping for us we can't use the
    // speed parameter to set the direction. Instead we reset the current position
    // and set a target position (twice the speed to make sure the motor doesn't stop unintentionally)
    // in the direction we want to go. Since we are messing with the current position we need an absolute position
    // to be able to go back our homed (0) position for position mode and angular mode. The reliabilty of these positions
    // depends on the accuracy of the stepper motor. So this is just best effort here ;-)
    stepper_->setCurrentPosition(0);
    uint16_t target_position = 2 * max_speed_; // carrot to chase, keeps motor running continuously

    // set direction
    target_position = is_direction_cw_ ? target_position : -target_position;

    // move motor by speed and direction, handling acceleration and deceleration automatically
    stepper_->moveTo(target_position);
    stepper_->run();

    // keep track of the absolute position (actual steps moved)
    uint16_t current_position = stepper_->currentPosition();
    if (current_position != previous_position_)
    {
        absolute_position_ += (current_position - previous_position_);
        // ensure absolute position stays in range 0 to STEPS_PER_ROTATION
        // can't use % operator because it doesn't handle negative numbers correctly
        if (absolute_position_ < 0)
        {
            absolute_position_ += STEPS_PER_ROTATION;
        }
        else if (absolute_position_ >= STEPS_PER_ROTATION)
        {
            absolute_position_ -= STEPS_PER_ROTATION;
        }
        previous_position_ = current_position;
    }
}

void MotorController::MoveBetweenLimitPositions()
{
    // FIXME: This is a servo implementation! Instead move to set target by target_position_ within the limits!!!

    long current_target = stepper_->targetPosition(); // could be negative due to continuous running implementation

    if (stepper_->distanceToGo() == 0)
    {
        // Motor has reached a target position, change direction and move to other limit
        is_direction_cw_ = !is_direction_cw_;
        uint16_t new_target = is_direction_cw_ ? ccw_limit_position_ : cw_limit_position_;
        stepper_->moveTo(new_target);
    }
    else if (current_target != cw_limit_position_ &&
             current_target != ccw_limit_position_)
    {
        // current target is not within the limits (eg. when changin into position mode)
        // continue to the limit in the current direction
        uint16_t correct_target = is_direction_cw_ ? cw_limit_position_ : ccw_limit_position_;
        stepper_->moveTo(correct_target);
    }
    // Run motor at speed between target positions, ramping automatically
    stepper_->run();
}

void MotorController::MoveToAngle()
{
    // Convert 16bit DMX512 value (2 Channels)
    long target_position = (target_position_ * STEPS_PER_ROTATION) / 65535;

    // Calculate the difference between the current position and the target
    long current_position = stepper_->currentPosition();
    long steps_to_target = target_position - current_position;

    // If motor is stationarry: Adjust for shortest path, considering wrap-around at STEPS_PER_ROTATION.
    // Ele continue in current direction to target angle. This based on the whalberg implementation.
    if (IsRunning())
    {
        if (steps_to_target >= (STEPS_PER_ROTATION / 2))
        {
            // steps to target are more than half a rotation.
            // moving backward is shorter.
            steps_to_target -= STEPS_PER_ROTATION;
        }
        else if (steps_to_target <= (-STEPS_PER_ROTATION / 2))
        {
            // steps to target are less than negative half of STEPS_PER_ROTATION
            // moving backward takes longer. It's quicker to move forward.
            steps_to_target += STEPS_PER_ROTATION;
        }
    }

    // Move to the desired position
    stepper_->moveTo(current_position + steps_to_target);
    stepper_->run();
}

void MotorController::Stop()
{
    // Initiates a controlled deceleration to stop the motor, rather than stopping it abruptly.
    // this method only initiates the stopping process. stepper->run() still has to be called
    // repeatedly in the loop to actually execute the steps.
    stepper_->stop(); // sets speed, target and position to 0.
    ESP_LOGI(TAG, "Motor stopped");
}

void MotorController::EnableMotor()
{
    if (!is_enabled_)
    {
        stepper_->enableOutputs();
        is_enabled_ = true;
        ESP_LOGI(TAG, "Motor enabled.");
    }
}

void MotorController::DisableMotor()
{
    if (is_enabled_)
    {
        stepper_->disableOutputs();
        is_enabled_ = false;
        ESP_LOGI(TAG, "Motor disabled.");
    }
}

void MotorController::SetDirectionCW()
{
    is_direction_cw_ = true;
    ESP_LOGI(TAG, "Direction set to CW");
}

void MotorController::SetDirectionCCW()
{
    is_direction_cw_ = false;
    ESP_LOGI(TAG, "Direction set to CCW");
}

bool MotorController::IsRunning()
{
    bool is_running = stepper_->isRunning();
    return is_running;
}
