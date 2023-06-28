#ifndef STEPPER_MOTOR_CONTROLLER_INTERFACE_H_
#define STEPPER_MOTOR_CONTROLLER_INTERFACE_H_

//TODO review interface and redo methods according to my needs. Icluded min. a run() method!


/**
 * @class StepperMotorControllerInterface
 * @brief Interface for a Stepper Motor Controller controlled by a motor driver.
 *
 * This interface specifies the functions that a Stepper Motor Controller should implement.
 */
class StepperMotorControllerInterface {
 public:
  enum Direction { CCW = 0 , CW = 1 };

  ~StepperMotorControllerInterface();

  /**
   * @brief Set the pins to conect the motor driver.
   *
   * @param enable_pin The pin connected to the enable input.
   * @param direction_pin The pin connected to the direction input.
   * @param step_pin The pin connected to the step input.
   */
  virtual void setPins(int8_t enable_pin, int8_t direction_pin, int8_t step_pin) = 0;

  /**
   * @brief Set the speed of the stepper motor.
   *
   * @param speed The desired speed.
   */
  virtual void setSpeed(float speed) = 0;

  /**
   * @brief Set the direction of the stepper motor.
   *
   * @param direction The desired direction. Typically, this may be a value of 1 or -1.
   */
  virtual void setDirection(Direction direction) = 0;

/**
   * @brief Set the target position for the stepper motor.
   *
   * @param absolute_position The desired absolute target position.
   */
  virtual void setPosition(int32_t absolute_position) = 0;

  /**
   * @brief Set the current position of the stepper motor.
   *
   * @param position The position .
   */
  virtual void setCurrentPosition(int32_t position) = 0;

  /**
   * @brief Retrieve the current position of the stepper motor.
   *
   * @return The current position.
   */
  virtual int32_t getCurrentPosition() const = 0;

  /**
   * @brief Retrieve the distance to the desired position.
   *
   * @return The distance to the desired position.
   */
  virtual int32_t getDistanceToGo(); //in accelstepper class not a const method.

  /**
   * @brief Check if the stepper motor is currently moving.
   *
   * @return True if the motor is moving, false otherwise.
   */
  virtual bool isMoving(); //in accelstepper class not a const method.

  /**
   * @brief Enable the stepper motor.
   */
  virtual void enableMotor() = 0;

  /**
   * @brief Disable the stepper motor.
   */
  virtual void disableMotor() = 0;
};

#endif  // STEPPER_MOTOR_CONTROLLER_INTERFACE_H_
