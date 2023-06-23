// StepperMotor.h

#pragma once
#include <Arduino.h>
#include <AccelStepper.h>

/**
 * @class StepperMotor
 * @brief This class controls a stepper motor using the AccelStepper library.
 * The update() function must be called in the main loop to ensure correct functionality.
 */
class StepperMotor {
public:
  /**
   * @brief Construct a new Stepper Motor object
   * 
   * @param forward_pin The pin to control the forward direction of the motor
   * @param backward_pin The pin to control the backward direction of the motor
   */
  StepperMotor(int forward_pin, int backward_pin);

  /**
   * @brief Set the target position for the motor to move to
   * 
   * @param position The target position
   */
  void setTargetPosition(long position);

  /**
   * @brief Get the current position of the motor
   * 
   * @return long The current position
   */
  long getCurrentPosition();

  /**
   * @brief Set the max speed for the motor
   * 
   * @param speed The max speed
   */
  void setMaxSpeed(float speed);

  /**
   * @brief Set the acceleration for the motor
   * 
   * @param acceleration The acceleration
   */
  void setAcceleration(float acceleration);

  /**
   * @brief Start the motor and move to the target position
   */
  void run();

  /**
   * @brief Stop the motor immediately
   */
  void stop();

  /**
   * @brief Update the motor state and control logic. 
   * This method needs to be called periodically in the main loop.
   */
  void update();

  /**
   * @brief Get the current state of the motor (moving or not)
   * 
   * @return true If the motor is moving
   * @return false If the motor is not moving
   */
  bool isMoving();

private:
  int forward_pin_;
  int backward_pin_;
  long current_position_;
  long target_position_;
  float max_speed_;
  float acceleration_;
  bool is_moving_;
  AccelStepper stepper_;
};
