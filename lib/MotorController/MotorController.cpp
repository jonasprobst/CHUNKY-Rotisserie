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
    stepper_->disableOutputs();                    // This disables the motor - it will not move until enabled
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
        ESP_LOGI(TAG, "Motor mode (ramp): fast.");
        break;

    default:
        ESP_LOGE(TAG, "Invalid motor mode %d. Ramp set to normal.", motor_mode);
        stepper_->setAcceleration(RAMP_SLOW);
        break;
    }
}

void MotorController::SetOperationMode(uint8_t operation_mode_dmx)
{
    // IMPORTANT: this is not the same as motor mode (see SetMotorMode for details)
    // Operation mode is set as a percentage of the DMX Channel 6 value.
    // Operation mode is set as a value between 0-255 in the DMX Channel 6 value.
    //  - 0 MotorStop
    //  - 1-128 (1-50%) Continuous rotation mode
    //  - 129-139 (51-54%) Position Mode (set limits enabled)
    //  - 140-203 (55-79%) Position mode
    //  - 204-255 (80-100%) Angular mode

    // Map the operation mode to the range 0 to 100
    // !!! AS PERCENTAGE IS LESS PRECISE THAN THE 255 STEPS OF DMX, I SUGGEST NOT TO CONVERT AND HARDCODE DMX VALUES. THE PERCENTAGE IS JUST A NICE FEATURE WHEN PROGRAMMING ON THE LIGHTDESK.
    // I ADDED SOME DMX VALUES TO THE COMMENTS I HOPE THEY CORRELATE WITH THOSE FROM WAHLBERG
    uint8_t operation_mode_pct = map(operation_mode_dmx, 0, 255, 0, 100); // macht es einfacher zu lesen, compiler optimiert es ohnehin, dass es nicht mehr programspeicher braucht.

    // Set operation mode
    if (operation_mode_dmx == 0)
    {
        operation_mode_ = MODE_STOP;
        ESP_LOGI(TAG, "Operation mode set to STOP ");
    }
    else if (operation_mode_dmx < 129)
    { // FIXME: should this start at 0? @demi -> würde sagen es kommt auf die fixture drauf an. Aber ist sicher gut, wenn es default nicht im Rotation Mode ist.
        operation_mode_ = MODE_ROTATION;
        ESP_LOGI(TAG, "Operation mode set to ROTATION ");
    }
    else if (operation_mode_dmx < 140)
    {
        operation_mode_ = MODE_POSITION_SAVE;
        ESP_LOGI(TAG, "Operation mode set to POSITION SAVE ");
    }
    else if (operation_mode_dmx < 204)
    {
        operation_mode_ = MODE_POSITION;
        ESP_LOGI(TAG, "Operation mode set to POSITION ");
    }
    else if (operation_mode_dmx <= 255)
    {
        operation_mode_ = MODE_ANGULAR;
        ESP_LOGI(TAG, "Operation mode set to ANGULAR (not implemented yet) ");
    }
    else
    {
        operation_mode_ = MODE_STOP;
        ESP_LOGI(TAG, "Operation mode set to STOP !! undefined state ");
    }
}

void MotorController::SetUserMaxSpeed(uint8_t max_speed_dmx)
{
    // Map the speed percentage to the range 0 to MOTOR_MAX_SPEED
    user_max_speed_ = map(max_speed_dmx, 0, 255, 0, MOTOR_MAX_SPEED);
    stepper_->setMaxSpeed(user_max_speed_);
    ESP_LOGI(TAG, "Max speed set to %f", user_max_speed_);
}

void MotorController::SetSpeed(uint8_t speed_dmx)
{
    // Map the speed percentage to the range 0 to current max_speed_
    speed_ = map(speed_dmx, 0, 255, 0, user_max_speed_);
    // using SetMaxSpeed instead of SetSpeed so stepper handles ramp up/down itself
    stepper_->setMaxSpeed(speed_);
    ESP_LOGI(TAG, "Speed set to %f", speed_);
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

void MotorController::SetTargetPosition(uint16_t position_dmx16)
{
    target_position_ = position_dmx16;
    stepper_->moveTo(target_position_);
    ESP_LOGI(TAG, "Target position set to %d", target_position_);
}

void MotorController::Run()
{
    EnableMotor(); // FIXME: Probably the wrong place for this? => Case stop!
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

        // Ensure both limits were set before starting position mode
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
        // Technically this can't be called... I think
        ESP_LOGE(TAG, "Invalid operation mode");
        Stop(); // is this the right action?
    }
}

void MotorController::ContinuousRotation()
{
    // In order to let AccelStepper handel the ramping for us we can't use the
    // speed parameter to set the direction. Instead we reset the current position
    // and set a target position (twice the speed to make sure the motor doesn't stop unintentionally)
    // in the direction we want to go. Since we are messing with the current position we need an absolute position
    // to be able to go back our homed (0) position for position mode and angular mode. The reliability of these positions
    // depends on the accuracy of the stepper motor. So this is just best effort here ;-)
    stepper_->setCurrentPosition(0);
    uint16_t position_to_chase = 2 * user_max_speed_; // carrot to chase, keeps motor running continuously

    // set direction
    position_to_chase = is_direction_cw_ ? position_to_chase : -position_to_chase;

    // move motor by speed and direction, handling acceleration and deceleration automatically
    stepper_->moveTo(position_to_chase);
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

    if (stepper_->distanceToGo() == 0) // my experience with servos is that they often don't hit the target exactly. at least when using a sensor and electro motor. therefore a target range might be adequate. This is possibly not the case when counting steps on a steppermotor.
    {
        // Motor has reached a target position, change direction and move to other limit
        is_direction_cw_ = !is_direction_cw_;
        /*
        uint16_t new_target = is_direction_cw_ ? ccw_limit_position_ : cw_limit_position_; // Not everyone can read this way of writing if/else, if you want to make it better readable for everyone, better write it out.
        stepper_->moveTo(new_target);
        */

        if(is_direction_cw_)
        {
            stepper_->moveTo(ccw_limit_position_); // as we already have switched direction it seems contradictory the new target?
        }else
        {
            stepper_->moveTo(cw_limit_position_);
        }
    
    }
    else if (current_target != cw_limit_position_ &&
             current_target != ccw_limit_position_)
    {
        // current target is not within the limits (eg. when changing into position mode)
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
    long target_position = (target_position_ * STEPS_PER_ROTATION) / 65535; // TAKE CARE OF THE VARIABLE target_position_ HERE!!! I THINK IT'S NOT HOW IT'S INTENDED

    // Calculate the difference between the current position and the target
    long current_position = stepper_->currentPosition();
    long steps_to_target = target_position - current_position;

    // If the motor is stationary: Adjust for shortest path, considering wrap-around at STEPS_PER_ROTATION.
    // Else continue in current direction to target angle. This is based on the implementation by Wahlberg.
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
