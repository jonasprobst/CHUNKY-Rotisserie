#ifndef MY_PIN_CONFIG_H
#define MY_PIN_CONFIG_H

#include <cstdint>
#include <arduino.h>

// SparkFun ESP32 Thing Plus (https://www.sparkfun.com/products/20168)
// [xx] refers to the pin used on the Thing/ DMX Shield
constexpr gpio_num_t THING_SDA = GPIO_NUM_23; // I2C SDA [Pin 12]
constexpr gpio_num_t THING_SCL = GPIO_NUM_23; // I2C SCL [Pin 11]

// SparkFun DMX-To-LED Shield (https://www.sparkfun.com/products/15110)
constexpr gpio_num_t DMX_ENABLE = GPIO_NUM_4; // GPIO_NUM_21; // Driver Output Enable (DE) of MAX3485. High for TX. [Pin 16]
constexpr gpio_num_t DMX_TX = GPIO_NUM_17;    // UART2 TX [Pin 14]
constexpr gpio_num_t DMX_RX = GPIO_NUM_16;    // UART2 RX [Pin 15]

// Maxon Servokontroller: ESCON 36/2 DC, 4-Q Servokontroller, 10-36 V, 2A/4A (https://www.maxongroup.ch/maxon/view/product/control/4-Q-Servokontroller/403112)
// Maxon Motor: DCX 22S GB KL 24V with Planetary Gearhead GPX22 A 138:1 and Encoder ENC30 HEDL 5540 500IMP
// - D1 in -> Speed (PWM Value: 10% -> 0rpm, 90% -> 12000rpm)
// - D2 in -> Enable (high -> enable)
// - D3 in -> Direction (high -> ccw)
// - D4 out -> Speed Comparator (high -> motor speed is > 0)
// - P1 in -> Speed Ramp (Potentiometer on board 0 - 2000rpm. Set <=50% to start)
// IMPORTANT: Currently the ramp is set on the potentiometer on the ESCON board
constexpr gpio_num_t MOTOR_ENABLE = GPIO_NUM_27;    // DMX-Shield DATA2 - shifted to 5V - [Pin 6] 
constexpr gpio_num_t MOTOR_DIRECTION = GPIO_NUM_19; // DMX-Shield DATA1 - shifted to 5V - [Pin 13]
//TODO: Check PWM Pin and if we can take the signal directly from esp32. Avoid the level shifter.
constexpr gpio_num_t MOTOR_STEP = GPIO_NUM_18;     //TODO: see above. DMX-Shield DATA0 - shifted to 5V - [Pin 12]
constexpr gpio_num_t MOTOR_RUNNING = GPIO_NUM_5;    // DMX-Shield CLOCK - shifted to 5V - [Pin 7]
// TODO: New feature: analog output (PWM) to set the speed ramp via webserver.



/*
// Stepper Motor Driver - TB6600 (common cathode wiring)
constexpr gpio_num_t MOTOR_ENABLE = GPIO_NUM_27;    // DMX-Shield DATA2 - shifted to 5V - [Pin 6]
constexpr gpio_num_t MOTOR_DIRECTION = GPIO_NUM_19; // DMX-Shield DATA1 - shifted to 5V - [Pin 13]
constexpr gpio_num_t MOTOR_STEP = GPIO_NUM_18;      // DMX-Shield DATA0 - shifted to 5V - [Pin 12]
*/

// Push Buttons
constexpr gpio_num_t BUTTON_AP = GPIO_NUM_25;
constexpr gpio_num_t BUTTON_STOP = GPIO_NUM_26;

/***************************************************
 * NOT IMPLEMENTED YET - CHECK PINS (!!!) BEFORE USE

// Display - SSD1306 128x32px I2C OLED Display
constexpr uint8_t DISPLAY_SDA = 21; //TODO: used by DMX Shield -> setup a different i2c bus. Stop the default?
constexpr uint8_t DISPLAY_SCL = 22;

// ?? Encoder
constexpr uint8_t ENCODER_A = 12;
constexpr uint8_t ENCODER_B = 13;
*/

#endif // MY_PIN_CONFIG_H