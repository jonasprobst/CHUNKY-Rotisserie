#ifndef MY_PIN_CONFIG_H
#define MY_PIN_CONFIG_H

#include <cstdint>
#include <arduino.h>

// SparkFun ESP32 Thing Plus (https://www.sparkfun.com/products/20168)
// [xx] refers to the pin used on the Thing/ DMX Shield
// Used for communication with 
// Display - SSD1306 128x32px I2C OLED Display
// Use this library: https://github.com/olikraus/u8glib
constexpr gpio_num_t THING_SDA = GPIO_NUM_21; // I2C SDA [Pin 12]
constexpr gpio_num_t THING_SCL = GPIO_NUM_22; // I2C SCL [Pin 11]

// SparkFun DMX-To-LED Shield (https://www.sparkfun.com/products/15110)
constexpr gpio_num_t DMX_ENABLE = GPIO_NUM_4; // GPIO_NUM_21; // Driver Output Enable (DE) of MAX3485. High for TX. [Pin 16]
constexpr gpio_num_t DMX_TX = GPIO_NUM_17;    // UART2 TX [Pin 14]
constexpr gpio_num_t DMX_RX = GPIO_NUM_16;    // UART2 RX [Pin 15]

// SparkFun ESP32 Thing Plus (USB-C) connected to SparkFun DMX-To-LED Shield
// From LED Shield to ESCON, the following pins are used:
constexpr gpio_num_t MOTOR_ENABLE = GPIO_NUM_18;    // DMX-Shield DATA2  | ESCON D1gIn 2 (brown)
constexpr gpio_num_t MOTOR_DIRECTION = GPIO_NUM_19; // DMX-Shield DATA1  | ESCON D1gIn 3 (green)
constexpr gpio_num_t MOTOR_STEP = GPIO_NUM_23;      // DMX-Shield DATA0  | ESCON D1gIn 1 (white)
constexpr gpio_num_t MOTOR_RUNNING = GPIO_NUM_35;    // DMX-Shield CLOCK | ESCON D1gOut 4 (yellow)


// Maxon Servokontroller: ESCON 36/2 DC, 4-Q Servokontroller, 10-36 V, 2A/4A (https://www.maxongroup.ch/maxon/view/product/control/4-Q-Servokontroller/403112)
// Maxon Motor: DCX 22S GB KL 24V with Planetary Gearhead GPX22 A 138:1 and Encoder ENC30 HEDL 5540 500IMP
// - D1 in -> Speed (PWM Value: 10% -> 0rpm, 90% -> 12000rpm)
// - D2 in -> Enable (high -> enable)
// - D3 in -> Direction (high -> ccw)
// - D4 out -> Speed Comparator (high -> motor speed is > 0)
// - P1 in -> Speed Ramp (Potentiometer on board 0 - 2000rpm. Set <=50% to start)
// IMPORTANT: Currently the ramp is set on the potentiometer on the ESCON board
// TODO: Feature: PWM out to set the speed ramp via webserver.


// Push Buttons
constexpr gpio_num_t BUTTON_AP = GPIO_NUM_14;
constexpr gpio_num_t BUTTON_STOP = GPIO_NUM_32;
// TODO: Feature: Add buttons to stop the motor and turn the access point on/off

#endif // MY_PIN_CONFIG_H