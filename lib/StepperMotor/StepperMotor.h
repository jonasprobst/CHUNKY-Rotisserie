#pragma once
#include <Arduino.h>
#include <AccelStepper.h>

/**
 * @class StepperMotor
 * @brief This class is a wrapper around the AccelStepper library and provides an interface to control a stepper motor.
 *
 * The class uses an instance of AccelStepper for the actual stepper motor control. This includes speed control,
 * acceleration, and deceleration features. This class also provides a means of enabling and disabling the motor.
 *
 * This class is designed for use with an ESP32 microcontroller and a motor driver that can be controlled with three
 * pins: one for enabling/disabling the motor and two for controlling the direction.
 *
 * Note: The instance should be updated regularly by calling the run() method (e.g., from the main loop).
 */
class StepperMotor
{
public:
    /**
     * @brief Constructs a new StepperMotor instance.
     *
     * @param forward_pin The pin used to control forward motion of the motor.
     * @param backward_pin The pin used to control backward motion of the motor.
     * @param enable_pin The pin used to enable/disable the motor.
     *
     * Note: It's assumed that the pinMode() for these pins has been set to OUTPUT mode in the main setup function.
     */
    StepperMotor(uint8_t forward_pin, uint8_t backward_pin, uint8_t enable_pin);

    /**
     * @brief Sets the maximum speed of the motor.
     *
     * @param speed The desired maximum speed in steps per second.
     */
    void setMaxSpeed(float speed);

    /**
     * @brief Sets the target position of the motor.
     *
     * @param position The target position in steps.
     */
    void setTargetPosition(long position);

    /**
    * @brief Set the acceleration for the motor
    * 
    * @param acceleration The acceleration
    */
    void setAcceleration(float acceleration);

    /**
     * @brief Returns the current position of the motor.
     *
     * @return The current position in steps.
     */
    long getCurrentPosition() const;

    /**
     * @brief Start the motor and move to the target position
     */
    void run();

    /**
     * @brief Immediately stops the motor and clears the target position.
     */
    void stop();

    /**
     * @brief Returns whether the motor is currently running.
     *
     * @return true if the motor is running, false otherwise.
     */
    bool isRunning() const;

private:
    AccelStepper stepper_; ///< AccelStepper instance for motor control.
    uint8_t forward_pin_; ///< Pin used for forward control of the motor.
    uint8_t backward_pin_; ///< Pin used for backward control of the motor.
    uint8_t enable_pin_; ///< Pin used for enabling/disabling the motor.
    long current_position_;
    long target_position_;
    float max_speed_;
    float acceleration_;
};
