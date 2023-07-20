#ifndef MY_PIN_CONFIG_H
#define MY_PIN_CONFIG_H

#include <cstdint>
#include <arduino.h>

// SparkFun ESP32 Thing Plus (https://www.sparkfun.com/products/20168)
// [xx] refers to the pin used on the Thing/ DMX Shield
constexpr gpio_num_t THING_SDA = GPIO_NUM_23; // I2C SDA [Pin 12]
constexpr gpio_num_t THING_SCL = GPIO_NUM_23; // I2C SCL [Pin 11]

// SparkFun DMX-To-LED Shield (https://www.sparkfun.com/products/15110)
constexpr gpio_num_t DMX_ENABLE = GPIO_NUM_4; //GPIO_NUM_21; // Driver Output Enable (DE) of MAX3485. High for TX. [Pin 16]
constexpr gpio_num_t DMX_TX = GPIO_NUM_17;     // UART2 TX [Pin 14]
constexpr gpio_num_t DMX_RX = GPIO_NUM_16;     // UART2 RX [Pin 15]
// constexpr uint8_t DMX_UNUSED = 5;  // DMX-Shield LED-CLOCK - shifted to 5V - [Pin 11]

// Stepper Motor Driver - TB6600 (common cathode wiring)
constexpr gpio_num_t STEPPER_ENABLE = GPIO_NUM_27;    // DMX-Shield DATA2 - shifted to 5V - [Pin 6]
constexpr gpio_num_t STEPPER_DIRECTION = GPIO_NUM_19; // DMX-Shield DATA1 - shifted to 5V - [Pin 13]
constexpr gpio_num_t STEPPER_STEP = GPIO_NUM_18;      // DMX-Shield DATA0 - shifted to 5V - [Pin 12]

// Push Buttons
constexpr gpio_num_t BUTTON_AP = GPIO_NUM_25;
constexpr gpio_num_t BUTTON_STOP = GPIO_NUM_26; 

/***************************************************
 * NOT IMPLEMENTED YET - CHECK PINS (!!!) BEFORE USE

// Display - SSD1306 128x32px I2C OLED Display
constexpr uint8_t DISPLAY_SDA = 21; //TODO: used by DMX Shield -> setup a different i2c bus. Stop the default?
constexpr uint8_t DISPLAY_SCL = 22;

// Limit Switches
constexpr uint8_t LIMIT_1 = 34; // TODO: input only
constexpr uint8_t LIMIT_2 = 35; // TODO: not available on things plus

// ?? Encoder
constexpr uint8_t ENCODER_A = 12;
constexpr uint8_t ENCODER_B = 13;

// ?? Ethernet - W5500 Ethernet Shield
constexpr uint8_t ETH_MOSI = 23; //TODO: Used for SDA
constexpr uint8_t ETH_MISO = 19; //TODO: Used by DMX Shield
constexpr uint8_t ETH_SCLK = 18; //TODO: Used by DMX Shield
constexpr uint8_t ETH_SS = 5; //TODO: Used by DMX Shield

// ?? Current Sensor -  ACS712 Hall Effect Current Sensor
constexpr uint8_t CURRENT_SENSE = 36; //TODO: Input only
*/

#endif // MY_PIN_CONFIG_H