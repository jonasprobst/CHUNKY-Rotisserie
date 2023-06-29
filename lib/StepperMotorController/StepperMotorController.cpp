#include "StepperMotorController.h"
#include "esp_log.h"

StepperMotorController::StepperMotorController(int8_t enablePin, int8_t directionPin, int8_t stepPin) :
    stepper_(AccelStepper::DRIVER, stepPin, directionPin) {
    enable_pin_ = enablePin;
    pinMode(enable_pin_, OUTPUT);
    disableMotor();
}

void StepperMotorController::setSpeed(float speed) {
    stepper_.setMaxSpeed(speed);
}

void StepperMotorController::setDirection(Direction direction) {
    stepper_.setPinsInverted(direction == Direction::CW, false, false);
}

void StepperMotorController::enableMotor() {
    digitalWrite(enable_pin_, HIGH);
}

void StepperMotorController::disableMotor() {
    digitalWrite(enable_pin_, LOW);
}

void StepperMotorController::setPosition(int32_t position) {
    stepper_.setCurrentPosition(position);
}


int32_t StepperMotorController::getDistanceToGo() { 
    return stepper_.distanceToGo();
}


bool StepperMotorController::isMoving() {
    return stepper_.distanceToGo() != 0;
}


/*
void StepperMotorController::processMotor() {
    stepper_.run();
}
*/