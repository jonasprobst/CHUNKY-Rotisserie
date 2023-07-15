#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

#include <cstdint>

// SparkFun ESP32 Thing Plus (https://www.sparkfun.com/products/20168)
// [xx] refers to the pin used on the Thing/ DMX Shield
constexpr uint8_t THING_SDA = 23; // I2C SDA [Pin 12]
constexpr uint8_t THING_SCL = 23; // I2C SCL [Pin 11]

// SparkFun DMX-To-LED Shield (https://www.sparkfun.com/products/15110)
constexpr uint8_t DMX_ENABLE = 21; // Driver Output Enable (DE) of MAX3485. High for TX. (GPIO21) - [Pin 16]
constexpr uint8_t DMX_PORT = 1;    // UART2 (0 is used by the console for debugging)
constexpr uint8_t DMX_TX = 17;     // UART2 TX (GPIO17) - [Pin 14]
constexpr uint8_t DMX_RX = 16;     // UART2 RX (GPIO16) - [Pin 15]
constexpr uint8_t DMX_UNUSED = 5;  // DMX-Shield LED-CLOCK (GPIO5) - shifted to 5V - [Pin 11]

// Stepper Motor Driver - TB6600 (common cathode wiring)
constexpr uint8_t STEPPER_ENABLE = 27;    // DMX-Shield DATA2 (GPIO27) - shifted to 5V - [Pin 6]
constexpr uint8_t STEPPER_DIRECTION = 19; // DMX-Shield DATA1 (GPIO19) - shifted to 5V - [Pin 13]
constexpr uint8_t STEPPER_STEP = 18;      // DMX-Shield DATA0 (GPIO18) - shifted to 5V - [Pin 12]

// Push Buttons
constexpr uint8_t BUTTON_AP = 25;   // GPIO25
constexpr uint8_t BUTTON_STOP = 26; // GPIO26

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

#endif // PIN_CONFIG_H