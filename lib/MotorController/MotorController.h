#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include "PinConfig.h"
#include <AccelStepper.h>

/**
 * @class MotorController
 * @brief Motor Controller for AccelStepper library.
 *
 * This class serves as a wrapper for the AccelStepper library.
 */
class MotorController
{
public:
    static constexpr uint8_t MOTOR_MAX_SPEED = 200; // The Maximum Speed the Motor can operate at
    
    enum Direction
    {
        CLOCKWISE,
        COUNTERCLOCKWISE
    };
    enum MotorMode
    {
        MANUAL,
        STEPPER,
        SERVO
    };

    /**
     * @brief Construct a new Motor Controller object.
     */
    MotorController();

    /**
     * @brief Destroy the Motor Controller object.
     */
    ~MotorController();

    /**
     * @brief Set the Speed object.
     *
     * @param speed The speed to set.
     */
    void SetSpeed(uint8_t speed);

    /**
     * @brief Set the Direction object.
     *
     * @param direction The direction to set.
     */
    void SetDirection(Direction direction);

    /**
     * @brief Set the Mode object.
     *
     * @param mode The mode to set.
     */
    void SetMode(MotorMode mode);

    /**
     * @brief Set the Target Position object.
     *
     * @param position The target position to set.
     */
    void SetTargetPosition(uint16_t position);

    /**
     * @brief Set the Servo Positions object.
     *
     * @param position1 The first servo position to set.
     * @param position2 The second servo position to set.
     */
    void SetServoPositions(uint16_t position1, uint16_t position2);

    /**
     * @brief Move the motor according to the current mode.
     */
    void Step();

    /**
     * @brief Stop the motor.
     */
    void Stop();

    /**
     * @brief Enable the motor.
     */
    void Enable();

    /**
     * @brief Disable the motor.
     */
    void Disable();

private:
    AccelStepper stepper_;        // AccelStepper instance
    bool enabled_;                // Motor enabled flag
    Direction current_direction_; // Motor direction
    MotorMode current_mode_;      // Motor mode
    uint16_t servo_position1_;    // Servo position 1
    uint16_t servo_position2_;    // Servo position 2
    uint16_t target_position_;    // Motor target position
    uint8_t current_speed_;      // Motor speed
};

#endif // MOTORCONTROLLER_H