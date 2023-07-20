# Drama Dynamo: Unleashing Dynamic Theater Performances

Welcome to Drama Dynamo, the device that brings theater props to life! We harness the power of DMX, the language of stage lighting and effects, decoded by the versatile ESP32 microcontroller. This enables any prop to move on cue - straight from the lightiong desk. Configuring your dynamic performance is a breeze through a Wi-Fi accessible webpage. Drama Dynamo, your key to creating truly immersive and interactive theater experiences.

## Setup

Drama Dynamo is designed to control a motorized theater prop using DMX messages interpreted by an ESP32 device.

### Components

1. **ESP32 DevKit V1**: This microcontroller will act as the brain of the system, interpreting DMX messages and controlling the movement of the motor. (or D1 mini?)
2. **DMX Shield**: The Sparfunk DMX to LED Shield is used to receive DMX messages sent from the lighting desk.
3. **Motor Driver Shield**: This shield will control the motor, following the instructions provided by the ESP32.
4. **Stepper Motor**: The motor is responsible for the physical movement of the prop.
5. **Wi-Fi Access Point**: The ESP32 will host a standalone Wi-Fi access point to enable easy configuration of the system.
6. **Webpage for Configuration**: A simple webpage will be hosted on the ESP32. This will allow the user to set the mode and base channel for DMX communication.
7. **LED Indicators**: Three LEDs will be used to provide status feedback about the system, access point, and motor state.
8. **Button for AP Control**: A button will be used to manually control the state of the Access Point.

## System States

The ESP32 device will save the mode and base channel so that these settings persist even after the system is powered off. The device will operate in one of two modes:

//TODO: update

- **Mode 1 (Fully Controlled Mode)**: In this mode, speed and direction are fully controlled by the lighting desk.
- **Mode 2 (Start-End Mode)**: In this mode, a start and end position is set by driving the motor to a specific location and sending a save command on a DMX channel. Once these positions are set, the motor can move back and forth between the two positions. The speed can be adjusted, and when the speed is 0, the motor stops.

The status of the system, Access Point, and motor will be indicated by LEDs.

## Software Architecture

//TODO: Update

1. **Hardware Abstraction Layer (HAL):** Interacts with the DMX shield, WiFi module, stepper motor, LEDs, and the AP button. Contains classes/functions to handle hardware-specific tasks. It must implement robust error handling due to direct interface with hardware.
2. **DMX Controller:** Interprets the DMX messages received, mapping DMX channels to controls like position, speed, direction, etc.
3. **Motor Controller:** Controls the stepper motor based on interpreted commands and manages the Motor LED state.
4. **WIFI AP Config Server:** Serves the webpage for mode and base channel settings, manages AP, and shuts down after 5 minutes of idleness.
5. **NVS Settings:** Manages saving and retrieving the mode and base channel configuration persistently across power cycles (NVS).
6. **System Controller:** Main controller for the system, manages the system LED and oversees the AP button functionality.
7. **LED Controller:** Controls the LED indicators for system status, AP status, and motor status, based on states received from the System Controller, Web Server, and Motor Controller.

### Additional considerations

- **Exception and Error Handling:** Implement robust error handling, especially in the Hardware Abstraction Layer (HAL), where we are directly interfacing with hardware.
- **Unit and Integration Tests:** Write and execute tests for all the major functionalities and classes as we go forward.
- **Documenting the Code:** Provide enough comments and documentation in the code to make it understandable to others or ourselves in the future.
- **Safety of the stage personell:** provide emergency stop and ensure that in case of prolonged dmx communication errors or signal loss the motor is stopped. Implement Hardware safety features like fuses and curent limiting.

//TODO: Update
- Automated Stoping (Loosing DMX Connection, Limit Switches?, Current Sensor?, temperature sensor?)
- Lightningdesk Personell (Max Speed - Channel 3 to 0, setup a set all 0 button on Desk)
- Housing of the device (grounded), choice of cables (big where needed), keeping thing savely stored away
- Double saftey for things attached to shaft [like this](https://images.app.goo.gl/RfDvGF1uAmsQuWuMA)
- Seperate circuits for DMX Data and Device (Groundloop protection)

