# Drama Dynamo: Unleashing Dynamic Theater Performances

Welcome to Drama Dynamo, the device that brings theater props to life! We harness the power of DMX, the language of stage lighting and effects, decoded by the versatile ESP32 microcontroller. This enables any prop to move on cue - straight from the lightiong desk. Configuring your dynamic performance is a breeze through a Wi-Fi accessible webpage. Drama Dynamo, your key to creating truly immersive and interactive theater experiences.

## WARNING

### BEFORE EACH USE

- Inspect the device for damage, wear, corrosion or absuse
- Ensure the device is mounted correctly and savely.
- Ensure the attached load is mounted correctly.

** DO NOT USE, THE DEVICE IF ANY DAMAGE OR ERROR IS FOUND!**

Please read the chapter [safety]() carefully and ensure any safety recomondations are implemented in your setup.

## WHEN OPERATING

Only operate the rotator when there is a clear view to the rotator and area around it.
Before running the rotator, ensure that the ‘danger zone’ next to the rotator is cleared to avoid risk of injury.


## Setup

Drama Dynamo is designed to control a motorized theater prop using DMX messages interpreted by an ESP32 device. The software inplementation and especially the DMX contoll interface is heavily based on the [Wahlberg Rotator](https://wahlberg.dk/products/dmx-rotators/dmx-rotator.), making this custom device easy to operate from a ligth desk.

### Components

1. **ESP32 Microcontroller**: The [Sparkfun ESP32 Thing Plus C]() will act as the brain of the system, interpreting DMX messages and controlling the movement of the motor.
2. **DMX Shield**: The [Sparfunk DMX to LED Shield]() is used to receive DMX messages sent from the lighting desk.
3. **Motor Driver Shield**: This shield will control the motor, following the instructions provided by the ESP32.
4. **Stepper Motor**: The motor is responsible for the physical movement of the prop.
5. **Wi-Fi Access Point**: The ESP32 will host a standalone Wi-Fi access point to enable easy configuration of the system.
6. **Webpage for Configuration**: A simple webpage will be hosted on the ESP32. This will allow the user to set the mode and base channel for DMX communication.
7. **LED Indicators**: Three LEDs will be used to provide status feedback about the system, access point, and motor state. **Not implemented**
8. **Button for AP Control**: A button will be used to manually control the state of the Access Point. **Not implemented**

## System States & Settings

The System has two distinct types of settings that determin the state the motor. In order to stay complient with the Wahlberg Rotator and it's [cheat sheet](https://wahlberg.dk/media/1186/281806004_rotator_cheat-sheet_a5.pdf) the naming has been copied. Unfortunately Wahlberg did a bad job at picking names, so here's an overview:

### Wifi Config Interface

On the wahlberg rotator theses settings are set by rotary switches on the actuall hardware. The original name for them is mode. The left side on the rotator's cheat sheet.

Configurations made via Wifi Config Interface are saved to NVS, so that they persist even after the device is powered off. The setting *Motor Mode* requires a restart to take effect.

//TODO: Test if it's possible to set basechannel to 0 (default) in webinterface
- **Base Channel**: The starting address of the devices DMX Channel range. Value Range: 0 - 500, Default: 0.
- **Motor Mode**: The ramp speed of the stepper motor (other modes are not supported). Requires a restart to take effect! Value Range: 0 - 9, Default: 2. //TODO make 2 the default value
    - 0: no function
    - 1 - 3: ramping speed (up and down synchronious) of the motor (1: slow, 2: normal, 3: fast).
    - 4 - 6: no function
    - 7 - 8: manual controll (not implemented). on the original hardware the motor can be controlled by the rotary switch buttons. 
    - 9: no function

### DMX Interface

The following Channel numbers are offset by the *Base Channel* set via the Wifi config interface. For example, if base channel is set to 50: channel 1 will be addressed with 51, channel 2 with 52 etc.

- Channel 1: Position rough (Hi of a 16bit DMX channel)
- Channel 2: Position fine (Lo of a 16bit DMX channel)
- Channel 3: Set the maximum speed (reached after ramp up)
- Channel 4: Speed CW / Set CW limit (//TODO speed or just direction?!)
- Channel 5: Speed CCW / Set CCW limit
- Channel 6: **Operation mode** (named "mode control" by Wahlberg)
    - set as a percentage of the channel value .
    - 0-50% Continuous rotation mode
    - 51-54% Position Mode (set limits enabled)
    - 55-79% Position mode
    - 80-100% Angular mode

## Software Architecture

1. **Hardware Abstraction Layer (HAL):** Interacts with the DMX shield, WiFi module, stepper motor, LEDs, and the AP button. Contains classes/functions to handle hardware-specific tasks. It must implement robust error handling due to direct interface with hardware.
2. **DMX Controller:** Interprets the DMX messages received, mapping DMX channels to controls like position, speed, direction, etc.
3. **Motor Controller:** Controls the stepper motor based on interpreted commands and manages the Motor LED state.
4. **WIFI AP Config Server:** Serves the webpage for mode and base channel settings, manages AP, and shuts down after 5 minutes of idleness.
5. **NVS Settings:** Manages saving and retrieving the motor mode and base channel configuration persistently across power cycles (NVS).
6. **System Controller:** Main controller for the system, manages the system LED and oversees the AP button functionality.
7. **LED Controller:** Controls the LED indicators for system status, AP status, and motor status, based on states received from the System Controller, Web Server, and Motor Controller.

## Safety 

- **Safety of the stage personell:** provide emergency stop and ensure that in case of prolonged dmx communication errors or signal loss the motor is stopped. 
- Implement Hardware safety features like fuses and curent limiting.
- Automated Stoping (Loosing DMX Connection, Current Sensor?, temperature sensor?)
- Lightningdesk Personell (Max Speed - Channel 3 to 0), **setup a set all 0 button on Desk**
- Housing of the device (grounded), choice of cables (big where needed), keeping things savely stored away
- Double saftey for things attached to shaft [like this](https://images.app.goo.gl/RfDvGF1uAmsQuWuMA)
- Seperate circuits for DMX Data and Device (Groundloop protection)
- Savety Checklist


## Technical Data

Dimensions (LWH): 
Power Supply:  250VAC 50/60Hz
Power Inlet:
DMX Control Signal: DMX 512/ 7 channels used
DMX Connection: 3 pole XLR in (link not supported)
Rotation Modes: Continuous Rotation, (Positional, Angular)
Rotation Speed: //TODO ~0.8 - 20 rpm
Load Limits: Shaft-down only! ~50kg
Own weight: //TODO
Mounting Clamp: //TODO
Motor: 24 VDC, //TODO Watt, IP30??





## Hardware

Übersetzung
- Motor zu Welle: 1:5 - 6
- Welle zu Achse: ~7.7:1

Motor Max Speed: 


Motor Achse: 4mm Durchmesser
Wellen Achse: 8mm Durchmesser


## Disclamer

The hardware and software is a prototype. Any use of the actual device as well as any part of the information provided inside this repository (including but not limited to the code, documentation and any instructions) happens at your own risk. The author and maker of this information and hardware can not be held liable. By using anything provided in this repository or an actual device supplied by the owner of this repository you agree to these terms.

## License

The chosen license for this repository is [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.html).


