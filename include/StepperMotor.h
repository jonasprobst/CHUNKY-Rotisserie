// StepperMotor.h

#pragma once
#include <Arduino.h>

/**
 * @class StepperMotor
 * @brief This class controls a stepper motor connected to an L298N H-bridge module.
 */
class StepperMotor {
public:
  /**
   * @brief Construct a new Stepper Motor object
   * 
   * @param speed_pin The pin to output the PWM signal for speed control
   * @param forward_pin The pin to control the forward direction of the motor
   * @param backward_pin The pin to control the backward direction of the motor
   */
  StepperMotor(int speed_pin, int forward_pin, int backward_pin);
  
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
   * @brief Update the motor state and control logic
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
  int speed_pin_;
  int forward_pin_;
  int backward_pin_;

  long target_position_;
  long current_position_;

  float current_speed_;
  float max_speed_;
  float acceleration_;

  bool is_moving_;
  
  /**
   * @brief Set the speed for the motor
   * 
   * @param speed The speed
   */
  void setSpeed(float speed);
};
