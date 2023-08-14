#ifndef MOTOR_CONTROLLER_INTERFACE_H
#define MOTOR_CONTROLLER_INTERFACE_H

#include <Arduino.h>

class MotorControllerInterface
{
public:

    // Ramp speed (set via motor mode on the webserver)
    const uint16_t RAMP_SLOW = 300;
    const uint16_t RAMP_NORMAL = 600;
    const uint16_t RAMP_FAST = 900;

    // Operation mode: the mode the motor is currently in (DMX channel 6)
    enum OperationMode
    {
        MODE_STOP,          // Stop mode
        MODE_ROTAION,      // Continuous rotation mode
        MODE_POSITION_SAVE, // Position mode: save limits
        MODE_POSITION,      // Position mode: use saved limits
        MODE_ANGULAR        // Angular mode
    };

    // Direction indicator: Symbolic names for the direction the motor is turning.
    enum Direction
    {
        DIRECTION_CW,
        DIRECTION_CCW
    };

    MotorControllerInterface() {}
    virtual ~MotorControllerInterface() {}

    virtual void SetRamp(uint8_t ramp) = 0;
    virtual void SetOperationMode(OperationMode mode) = 0;
    virtual void SetMaxSpeed(uint8_t speed) = 0;
    virtual void SetSpeed(uint8_t SetMaxSpeed) = 0;
    virtual void SetDirection(Direction direction) = 0;
    virtual void SaveLimitPosition(Direction direction) = 0;
    virtual void SetTargetPosition(uint16_t position) = 0;
    virtual void Move() = 0;
    virtual void Stop() = 0;
    virtual bool IsMoving() = 0;
    virtual void Enable() = 0;
    virtual void Disable() = 0;
};

#endif // MOTOR_CONTROLLER_INTERFACE_H