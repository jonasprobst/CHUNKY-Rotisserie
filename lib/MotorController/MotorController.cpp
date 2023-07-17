#include "PinConfig.h"
#include "MotorController.h"
#include <Arduino.h>

String TAG = "MotorController";


//TODO: change Modes: Rotation, Position, Angular
//TODO: Only change mode if it's different form current mode
//TODO: Update the ramp from settings object (mode) -> add settings object to update in real time
//      Maybe it makes more sense to do this in the main loop?

// Constructor implementation
MotorController::MotorController()
    : stepper_(AccelStepper::DRIVER, STEPPER_STEP, STEPPER_DIRECTION)
{
    stepper_.setPinsInverted(false, false, true);
    stepper_.setMaxSpeed(1000);
    pinMode(STEPPER_ENABLE, OUTPUT);
    SetDirection(CLOCKWISE);
    SetSpeed(0);
    Disable();
}

MotorController::~MotorController()
{
    Stop();
    Disable();
}

// Set motor speed
void MotorController::SetSpeed(uint8_t speed)
{
    if (speed != stepper_.speed())
    {
        stepper_.setSpeed(static_cast<float>(speed));
        ESP_LOGI(TAG, "Speed set to %d", speed);
    }
}

// Set motor direction
void MotorController::SetDirection(Direction direction)
{
    if (current_direction_ != direction)
    {
        stepper_.setSpeed(stepper_.speed() * -1);
        current_direction_ = direction;
        ESP_LOGI(TAG, "Direction set to %d", direction);
    }
}

// Set mode
void MotorController::SetMode(MotorMode mode)
{
    current_mode_ = mode;
    ESP_LOGI(TAG, "Mode set to %d", mode);
}

// Set target position for stepper motor to move to
void MotorController::SetTargetPosition(uint16_t position)
{
    uint16_t current_position = stepper_.currentPosition();
    if (current_position != position)
    {
        target_position_ = position;
        stepper_.moveTo(position);
        ESP_LOGI(TAG, "Target position set to %d", position);
    }
}

void MotorController::SetServoPositions(uint16_t position1, uint16_t position2)
{
    //TODO: the positions should probably be calculated from an angle and current position, right?
    // if position1 or position2 have changed, update them
    if(servo_position1_ != position1 || servo_position2_ != position2){
        ESP_LOGI(TAG, "Servo positions set to %d and %d", position1, position2);
        // if the motor is currently moving, stop it
        if(stepper_.isRunning()){
            stepper_.stop();
        }
        // save positions and set target position to position1
        servo_position1_ = position1;
        servo_position2_ = position2;
        SetTargetPosition(position1);
    }     
}

// Perform a step
void MotorController::Step()
{
    if (!enabled_)
    {
        return;
    }

    switch (current_mode_)
    {
    case MANUAL:
       // Run the motor at the current speed and direction

        /*if(!stepper_.runSpeed()){
            ESP_LOGE(TAG, "Motor couldn't move");
        }*/
        stepper_.runSpeed();
        break;

    case STEPPER:
        // Stepper mode: move to target position. Accelerate and decelerate to reach target position.
        if(!stepper_.run()){
            ESP_LOGE(TAG, "Motor couldn't move.");
        }
        break;

    case SERVO:
        // Servo mode: move back and forth between two positions
        if (stepper_.distanceToGo() == 0)
        {
            // If we've reached the target, switch to the other position
            target_position_ = (target_position_ == servo_position1_) ? servo_position2_ : servo_position1_;
            stepper_.moveTo(target_position_);
            ESP_LOGI(TAG, "We hit our target position. Do a u-turn.");
        }
        if(!stepper_.run()){
            ESP_LOGE(TAG, "Motor couldn't move.");
        }
        break;
    }
}

// Stop the motor
void MotorController::Stop()
{
    if(!stepper_.isRunning()){
        ESP_LOGI(TAG, "Motor already stopped");
        return;
    }
    
    stepper_.stop(); // resets speed and target position to 0.
    ESP_LOGI(TAG, "Motor stopped");

    // get things back to the way they were before stopping so we can resume if needed
    SetSpeed(current_speed_);
    if (current_mode_ == STEPPER || current_mode_ == SERVO)
    {
        SetTargetPosition(target_position_);
    }
}

// Enable the motor
void MotorController::Enable()
{
    // active low, due to the common-cathode wiring of TB6600
    digitalWrite(STEPPER_ENABLE, LOW); 
    enabled_ = true;
    ESP_LOGI(TAG, "Motor enabled.");
}

// Disable the motor
void MotorController::Disable()
{
    // active low, due to the common-cathode wiring of TB6600
    digitalWrite(STEPPER_ENABLE, HIGH); 
    enabled_ = false;
    ESP_LOGI(TAG, "Motor disabled.");
}