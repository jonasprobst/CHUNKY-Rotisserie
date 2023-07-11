#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <AccelStepper.h>

class MotorController
{
public:
    // Pin definitions
    static const uint8_t STEP_PIN = 0;
    static const uint8_t DIRECTION_PIN = 1;
    static const uint8_t ENABLE_PIN = 2;
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

    // Constructor
    MotorController();

    // Motor control functions
    void SetSpeed(uint32_t speed);
    void SetDirection(Direction direction);
    void SetMode(MotorMode mode);
    void SetTargetPosition(uint16_t position);
    void SetServoPositions(uint16_t position1, uint16_t position2);
    void Step();
    void Stop();
    void Enable();
    void Disable();

private:
    AccelStepper stepper_;        // AccelStepper instance
    bool enabled_;                // Motor enabled flag
    Direction current_direction_; // Motor direction
    MotorMode current_mode_;      // Motor mode
    uint16_t servo_position1_;    // Servo position 1
    uint16_t servo_position2_;    // Servo position 2
    uint16_t target_position_;    // Motor target position
    uint32_t current_speed_;      // Motor speed
};

#endif // MOTORCONTROLLER_H