#ifndef ESCONDCCONTROLLER_H
#define ESCONDCCONTROLLER_H

#include "MotorControllerInterface.h"
#include "Settings.h"

/**
 * @class EsconDCController
 * @brief Wrapper to control the motor via Maxon Motor ESCON 32/2 DC 4-quadrant PWM servo controller.
 * @warning This only supports continuous rotation and the ramp is set via potentiometer on the ESCON board.
 */
class EsconDCController : public MotorControllerInterface
{
public:
    /**
     * @brief Construct a new Servo Controller object.
     *
     * @param motor_mode The mode the motor is in.
     * @warning This is the motor mode configured via Webserver to set ramp-speed. Not the operation mode.
     */
    EsconDCController(uint8_t motor_mode);

    /**
     * @brief Destroy the Servo Controller object.
     */
    ~EsconDCController();

    /**
     * @brief Set the Operation Mode.
     * This code is based on the Wahloberg Rotator (https://wahlberg.dk/products/dmx-rotators/dmx-rotator).
     * Unfortunately Whalberg has made a mess with naming the modes.
     * There is the mode that is set by the rotary switches on the hardware: in this code reffered to "Motor Mode" (see SetMotorMode for details)
     * And the mode that is set via DMX Channel 6: in this code reffered to as "Operation Mode".
     * Operation mode is set as a percentage of the DMX Channel 6 value.
     *  - 0-50% Continuous rotation mode
     *  - 51-54% Position Mode (set limits enabled)
     *  - 55-79% Position mode
     *  - 80-100% Angular mode
     *
     * @param operation_mode The operation mode to set.
     */
    void SetOperationMode(uint8_t operation_mode);

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
     * @brief Set the Direction of the motor to counter-clockwise.
     */
    void SetDirectionCW();

    /**
     * @brief Set the Direction of the motor to clockwise.
     */
    void SetDirectionCCW();

    /**
     * @brief Sets soft limit for clockwise direction.
     * @warning This is required to enable position mode.
     */
    void SetCWLimitPosition();

    /**
     * @brief Sets soft limit for counter-clockwise direction.
     * @warning This is required to enable position mode.
     */
    void SetCCWLimitPosition();

    /**
     * @brief Set the Target Position object.
     *
     * @param position The target position to set.
     */
    void SetTargetPosition(uint16_t position);

    /**
     * @brief Run the motor according to the current operation mode.
     * @warning This has to be called repeatedly in the main loop.
     */
    void Run();

    /**
     * @brief Check if the motor is moving.
     *
     * @return true if the motor is moving.
     */
    bool IsRunning();

    /**
     * @brief Check if the motor is enabled.
     *
     * @return true if the motor is enabled.
     */
    bool IsEnabled();

    /**
     * @brief Stop the motor.
     * @warning Initiates a controlled deceleration to stop the motor, rather than stopping it abruptly.
     */
    void Stop();

private:
    static constexpr uint16_t MOTOR_MAX_SPEED = 12000;   // The Maximum Speed the Motor can operate at
    static constexpr uint16_t STEPS_PER_ROTATION = 500;  // Feedback by Encoder: 500imp/rev
    static constexpr uint16_t RAMP_SLOW = 250; // TODO: Test and adapt these ramp settings
    static constexpr uint16_t RAMP_NORMAL = 500;
    static constexpr uint16_t RAMP_FAST = 1000;

    OperationMode operation_mode_ = MODE_STOP; // Motor mode
    float max_speed_ = 0;                      // Motor max speed
    float speed_ = 0;                          // Motor speed
    uint16_t cw_limit_position_ = 0;           // CW limit position (position mode)
    uint16_t ccw_limit_position_ = 0;          // CCW limit position (position mode)
    uint16_t target_position_ = 0;             // Target position
    uint16_t absolute_position_ = 0;           // Absolute position
    uint16_t previous_position_ = 0;           // Previous position (helper to calculate absolute position)
    bool is_enabled_ = false;                  // Motor enabled flag
    bool is_direction_cw_ = true;              // Direction flag

    /**
    * @brief Sets the motor mode.
    * The motor mode is based on the Wahloberg Rotator (https://wahlberg.dk/products/dmx-rotators/dmx-rotator).
    * Unfortunately Whalberg has made a mess naming the modes.
    * There is the mode that is set via DMX Channel 6: in this code reffered to as "Operation Mode" (see SetOperationMode for details).
    * And the mode that is set on the rotary switches on the hardware: in this code reffered to as "Motor Mode".
    * The Motor Mode is set via the Webserver in this implementation - requires a restart to take effect!
    * Motor Mode 0-3 determines the ramping (up and down) of the motor (mode 0: neutral, mode 1: slow, mode 2: normal, mode 3: fast).
    * Motor Mode 4-6 have no function (not implemented).
    * Motor Mode 6-9 is a manual controll interface on the original hardware via the rotary switch buttons (not implemented).
    * @warning This code only implements motor mode 0-3. The other modes are not implemented.
    * @warning Changes to the motor mode need a restart of the ESP32 to take effect!
    * 
    * @param motor_mode The operation mode to set.
    */

    void SetMotorMode(uint8_t motor_mode);

    /**
     * @brief Enable the motor.
     * @warning The motor will not move until enabled.
     *          The constructor does not enable the motor.
     */
    void EnableMotor();

    /**
     * @brief Disable the motor.
     *
     *  //TODO: Following True? ->No power will be applied to the motor and it can be manually moved freely.
     */
    void DisableMotor();

    /**
     * @brief Rotate the motor continuously in the direction at speed.
     */
    void ContinuousRotation();

    /**
     * @brief Moves the motor between two limit positions. If the motor reaches a limit,
     *        it reverses its direction. If the motor's target isn't one of the limits,
     *        it corrects the target based on the current direction.
     * @warning cw_limit_position_ and ccw_limit_position_ must be set.
     */
    void MoveBetweenLimitPositions();

    /**
     * @brief Move the motor on the shortest path to the set target angle
     * @warning The motor must be homed before using this mode.
     */
    void MoveToAngle();
};

#endif // MOTORCONTROLLER_H