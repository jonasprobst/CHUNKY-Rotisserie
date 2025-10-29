# Quickstart

:bangbang: **WARNING**: Please read the chapter safety in the readme before use. 

## Setup

1. Securely mount CHUNKY-Rotisserie in safe environment, make sure it's lever can rotate freely
2. Connect CHUNKY-Rotisserie to Power: 230VAC at 50Hz with a 10A fuse
3. Power on the device - don't connect DMX yet!
4. Connect to the wifi access point "CHUNK" with the password "r0t1SS3r13.me" (AP only available for 5min. after boot)
5. In a webbroswer navigate to "192.168.04.01"
6. Choose a base channel for dmx communication (e.g. 500 means the device will respond to channels 501 - 506)
7. Choose a motor-mode (ramping speed). The heavier to object you're rotating the slower the ramp should be.
8. Restart CHUNKY-Rotisserie for the Settings to take effect
9. Ensure all relevant DMX-channels are set to 0!
10. Connect the DMX-Cable
11. Clear out of the danger zone and have fun rotating :smiley:

## Operating

1. Choose the operation-mode with channel 6 (e.g. if base channel was set to 500 this is 506)
2. Choose the maximum speed with channel 3
3. Operate with the other channels according to the chose mode
   1. e.g. in Continuous Rotation, changing channel 4 will rotate the motor clock wise relative to the set max speed. Changes in speed will happen slowly as the motor is ramping up and down to avoid internal damage.


## DMX-Channel Overview

| DMX Channel | Function |
| :---------- | :------- |
| 1           | Position rough (High Byte of 16bit DMX Channel)        |
| 2           | Position fine (Low Byte of 16bit DMX Channel)         |
| 3           | Set the maximum speed         |
| 4           | Rotate CW and set soft CW limit         |
| 5           | Rotate CCW and set soft CCW limit         |
| 6           | Modus Operandi         |

## Modus Operandi

## ESCON - Setup

### Motor Mode (Webserver)

    // Motor mode sets the ramp (up and down) of the stepper motor. It is set via Webserver.
    // IMPORTANT: this is not the same as the operation mode (see SetOperationMode for details)
    // IMPORTANT: Parameter motor_mode is ignored since the ramp is set via potentiometer on the ESCON board.

    // WARNING: In the current setup the ramp speed is set via the potentiometer on the ESCON board.
    // Changes to the parameter motor_mode have absolutley no effect.
    // A future feature would be to set the ramp via an analog output (PWM). The ESCON board supports this,
    // but would need to be reconfigured via the ESCON Studio software.

### Modus Operandi

    // Operation mode is set as a percentage of the DMX Channel 6 value.
    // IMPORTANT: this is not the same as motor mode (see SetMotorMode for details)

    // WARNING: The Escon 36/2 DC only supports continuous rotation
    // 0: STOP
    // 1-255: Continuous Rotation

    



