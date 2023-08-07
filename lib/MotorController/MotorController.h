#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include "MotorControllerInterface.h"
#include "Settings.h"
#include <AccelStepper.h>

/**
 * @class MotorController
 * @brief Motor Controller for AccelStepper library.
 *
 * This serves as a wrapper for the AccelStepper library to control the motor.
 */

class MotorController : public MotorControllerInterface
{
public:
    /**
     * @brief Construct a new Motor Controller object.
     *
     * @param motor_mode The mode the motor is in.
     * @warning This is the motor mode configured via Webserver.
     *          (e.g. ramp-speed, manual, etc.)
     *          Not the operation mode.
     */
    MotorController(uint8_t motor_mode);

    /**
     * @brief Destroy the Motor Controller object.
     */
    ~MotorController();

    /**
     * @brief Set the Operation Mode.
     * @warning This is the operation mode that can be set via DMX channel 6
     *          (e.g. continuous rotation, angular mode, etc.).
     *          Not the motor mode set via Webserver!
     *
     * @param operation_mode The operation mode to set.
     */
    void SetOperationMode(OperationMode operation_mode);

    /**
     * @brief Set max speed of the motor.
     * @warning The max speed is limited to MOTOR_MAX_SPEED.
     *
     * @return uint8_t The max speed.
     */
    void SetMaxSpeed(uint8_t max_speed);

    /**
     * @brief Set the Speed of motor relative to max speed.
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
     * @brief Set soft limits for position mode.
     *
     * @param direction The direction to set the limit for.
     */
    void SaveLimitPosition(Direction direction);

    /**
     * @brief Set the Target Position object.
     *
     * @param position The target position to set.
     */
    void SetTargetPosition(uint16_t position);

    /**
     * @brief Move the motor according to the current mode.
     *
     * @return true if the motor moved successfully.
     */
    bool Move();

    /**
     * @brief Stop the motor.
     *
     * @return true if the motor is stopped.
     */
    bool Stop();

    /**
     * @brief Enable the motor.
     * @warning The motor will not move until enabled.
     *          The constructor does not enable the motor.
     */
    void Enable();

    /**
     * @brief Disable the motor.
     * @warning This resets the motor's speed, position, saved limits and
     *          target position. Only use this for an emergency stop.
     *
     * No power will be applied to the motor and it can be manually moved freely.
     */
    void Disable();

private:
    static constexpr uint16_t MOTOR_MAX_SPEED = 500; // The Maximum Speed the Motor can operate at
    static constexpr uint16_t MOTOR_MAX_RAMP = 1000; // The Maximum Ramp the Motor can operate at

    AccelStepper *stepper_ = nullptr;          // AccelStepper instance
    OperationMode operation_mode_ = MODE_STOP; // Motor mode
    float ramp_ = 0;                           // Motor ramp
    float max_speed_ = 0;                      // Motor max speed
    float speed_ = 0;                          // Motor speed
    Direction direction_ = DIRECTION_CW;       // Motor direction
    uint16_t saved_cw_position_ = 0;           // Servo position 1
    uint16_t saved_ccw_position_ = 0;          // Servo position 2
    uint16_t target_position_ = 0;             // Motor target position
    bool is_moving_ = false;                   // Motor moving flag TODO: Better use the AccelStepper method?
    bool is_enabled_ = false;                  // Motor enabled flag

    /**
     * @brief Set the Motor Mode.
     * 
     * @param motor_mode The motor mode to set.
    */
   void SetMotorMode(uint8_t motor_mode);

    /**
     * @brief Setup the stepper motor instance.
     */
    void SetupStepper();

    /**
     * @brief Set Ramp (aka Acceleration) of the motor.
     *        This is set via MotorMode in Webserver.
     *
     * @param ramp The ramp to set.
     */
    void SetRamp(uint8_t ramp);
};

#endif // MOTORCONTROLLER_H