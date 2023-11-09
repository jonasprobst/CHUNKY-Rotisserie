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
        MODE_ROTATION,      // Continuous rotation mode
        MODE_POSITION_SAVE, // Position mode: save limits
        MODE_POSITION,      // Position mode: use saved limits
        MODE_ANGULAR        // Angular mode
    };

    MotorControllerInterface() {}
    virtual ~MotorControllerInterface() {}

    virtual void SetMotorMode(uint8_t motor_mode_dmx) = 0;
    virtual void SetOperationMode(uint8_t operation_mode_dmx) = 0;
    virtual void SetUserMaxSpeed(uint8_t user_max_speed_dmx) = 0;
    virtual void SetSpeed(uint8_t speed_dmx) = 0;
    virtual void SetDirectionCW() = 0;
    virtual void SetDirectionCCW() = 0;
    virtual void SetCWLimitPosition() = 0;
    virtual void SetCCWLimitPosition() = 0;
    virtual void SetTargetPosition(uint16_t position_dmx16) = 0;
    virtual void Run() = 0;
    virtual void Stop() = 0;
    virtual bool IsRunning() = 0;
    virtual void EnableMotor() = 0;
    virtual void DisableMotor() = 0;
};

#endif // MOTOR_CONTROLLER_INTERFACE_H