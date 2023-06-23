#include "StepperMotor.h"

StepperMotor::StepperMotor(int enable_pin, int forward_pin, int backward_pin)
    : enable_pin_(enable_pin), forward_pin_(forward_pin), backward_pin_(backward_pin), is_moving_(false), speed_(0), max_speed_(100), position_(0), target_position_(0)
{
    // Set pin modes
    pinMode(enable_pin_, OUTPUT);
    pinMode(forward_pin_, OUTPUT);
    pinMode(backward_pin_, OUTPUT);

    // Stop motor
    digitalWrite(forward_pin_, LOW);
    digitalWrite(backward_pin_, LOW);
    analogWrite(enable_pin_, 0);
}

void StepperMotor::setMaxSpeed(int max_speed)
{
    // Sets the maximum speed of the motor
    max_speed_ = constrain(max_speed, 0, 100); // Make sure max_speed is in valid range
}

void StepperMotor::setDirection(Direction direction)
{
    direction_ = direction; // Store the direction
    switch (direction_)
    {
        case Direction::CW:
            digitalWrite(forward_pin_, HIGH);
            digitalWrite(backward_pin_, LOW);
            break;
        case Direction::CCW:
            digitalWrite(forward_pin_, LOW);
            digitalWrite(backward_pin_, HIGH);
            break;
    }
}

void StepperMotor::start()
{
    // To start the motor, we just need to set target speed
    if (!isMoving()) {
        setDirection(direction_); // Sets the motor direction
        setSpeed(max_speed_); // Sets the target speed
        is_moving_ = true;
    }
}

void StepperMotor::stop()
{
    // To stop the motor, we set target speed to zero
    if (isMoving()) {
        setSpeed(0);
        is_moving_ = false; // Set moving state to false when speed is 0
    }
}

bool StepperMotor::isMoving()
{
    return is_moving_;
}

void StepperMotor::setPosition(int position)
{
    // Sets the target position of the motor
    target_position_ = position;
}

int StepperMotor::getPosition()
{
    // Returns the current position of the motor
    return position_;
}

// Private methods

void StepperMotor::setSpeed(int speed)
{
    // Map speed to duty cycle
    target_speed_ = constrain(speed, 0, max_speed_); // Make sure speed is in valid range
}

void StepperMotor::update() 
{
    // Use this function in your loop() to gradually ramp speed and update position
    if(speed_ < target_speed_) {
        speed_++;
        position_++;
    } 
    else if(speed_ > target_speed_) {
        speed_--;
        position_--;
    }

    if (speed_ == 0) {
        is_moving_ = false; // If speed is 0, set isMoving to false
    }

    int duty_cycle = map(speed_, 0, max_speed_, 0, 255);
    analogWrite(enable_pin_, duty_cycle);
}
