#ifndef STEPPER_MOTOR_CONTROLLER_H_
#define STEPPER_MOTOR_CONTROLLER_H_

#include <AccelStepper.h>
#include "StepperMotorControllerInterface.h"

/**
 * @class StepperMotorController
 * @brief Implements the StepperMotorControllerInterface for controlling a stepper motor.
 * 
 * This class provides the implementation details for the StepperMotorControllerInterface,
 * using the AccelStepper library to control the stepper motor.
 */
class StepperMotorController : public StepperMotorControllerInterface {
 public:
  /**
   * @brief Construct a new StepperMotorController object.
   *
   * @param interface_type The interface type of the stepper motor. See AccelStepper's documentation for values.
   * @param enable_pin The pin connected to the enable input.
   * @param direction_pin The pin connected to the direction input.
   * @param step_pin The pin connected to the step input.
   */
  StepperMotorController(int8_t enable_pin, int8_t direction_pin, int8_t step_pin);

  void setSpeed(float speed) override;
  void setDirection(Direction direction) override;
  void setPosition(int32_t absolute_position) override;
  void setCurrentPosition(int32_t position) override;
  int32_t getCurrentPosition() const override;
  int32_t getDistanceToGo() override; 
  bool isMoving() override;
  void enableMotor() override;
  void disableMotor() override;

 private:
  AccelStepper stepper_;  ///< AccelStepper object to control the stepper motor.
  int8_t enable_pin_;       ///< The pin connected to the enable input.
};

#endif  // STEPPER_MOTOR_CONTROLLER_H_
