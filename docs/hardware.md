# Hardware

## Components

- Powerplug with Switch and Fuse (10A)
- Powwersuply 230VAC to 24VDC (6A fuse on output)
- Spannungsregler 24VDC zu 5VCD (1A fuse on output)
- Maxon ESCON Servo Driver
- Maxon Motor with Planetary Gearhead and Encoder
- Sparkfun DMX-Shield
- Sparkfun ESP32 Thing Plus C
- Repurposed Fitness Bike Frame with custom axel
- Timing Belt 5M, 9mm
- Gearhead 5M, 12 Teeth, 9mm
- Gearhead 5M, 18 Teeth, 15mm
- 2 clamps 50mm for truss attachment, fixed 
- 1 clamp 50mm for truss attachment, position flexible
- Custom metal housing
- Various wires, criped
- Various screews, nuts and bolts

### Powersupply (wireing)

- Powerplug (10A Sicherung) -> Netzteil 24VDC
- Netzteil -> 6A Sicherung -> ESCON @ 24VDC
- Netzteil -> 6A Sicherung -> Spannungsregler 5V -> 1A Sicherung -> DMX Controller 5VDC
- DMX Controller 5VDC -> Onboard Spannungswandler 3.3VDC -> ESP32 Thing
- DMX Controller 5VDC -> Optocopler -> DMX Anschluss

## Drive

- Maxon Motor DCX 22S GB KL 24V
- Planetary Gearhead GPX22 A 138:1
- Encoder ENC30 HEDL 5540 500IMP
- [Maxon ESCON 36/2 DC](https://www.maxongroup.ch/maxon/view/product/control/4-Q-Servokontroller/403112), 4-Q Servokontroller, 10-36 V, 2A/4A
- Timing belt - Gearhead Output to Shaft: 12:16 = 0.75:1 | 12:18 = 0.666 (Gears: 12, 16, 18, 60)
- V belt - Shaft to Axcel: 1:7.77

### Calc speed at swung object

NominalSpeedMotor = 10'800rpm
GearRatio = (138/1) x (12/16) x (7.77/1) = 138 x 0.75 x 7.77 =  804.2
OutputSpeed = 10'800rpm / 804.2 = 13.43rpm
AngularSpeed = 13.43 x (2 Pi rad / 60) = 1.41rad/s
LinearSpeed = AngularSpeed x Radius = 1.41 x (5m/2) = 3.52/s
kmh = 3.52 x 3.6 = 12.66km/h (~max speed at swung object)

timing gear ratio options:

- 1:1 -> 2.64 m/s -> 9.49 km/h (too slow)
- 12:16 -> 3.52 m/s -> 12.66 km/h <== plan B
- 12:18 -> 3.96 m/s -> 14.24 km/h <== plan A (if belt fit's)
- 12:60 -> 13.18 m/s -> 47.46 km/h (belt wo't fit)
- 16:18 -> 2.97 m/s -> 10.68 km/h (too slow)

## ESCON Servo Driver

### Protection

The ESCON 36/2 DC has protective circuits against overcurrent, excess temperature, under- and over-voltage, against voltage transients and against short-circuits in the motor cable. Furthermore it is equipped with protected digital inputs and outputs and an adjustable current limitation for protecting the motor and the load. The motor current and the actual speed of the motor shaft can be monitored by means of the analog output voltage.

### ESCON Setup (in Studio)

Following the settings set via setup wizard in ESCON Studio. Make sure the firmware is up to date.

- Speed Constant: 520 rpm/V (Motor Configuration - p.5)
- Thermal time constant winding: 20s (Motor Configuration - p.5)
- Max permissible Speed: 12000 (Motor Configuration - p.5 -> no load speed 12400, recomended speed gearbox 12000)
- Nominal current: 2 A (default)
- Max output current limit: 4A (ESCON 36/2 limit)
- Speed sensor: Digital Incremental Encoder
- Encoder Resolution: 500 Counts/turn (Motor Configuration - p.2)
    - Encoder Direction: Maxon (guess since it's a maxon sensor)
- Mode of operation: Speed Controller closed loop (my choice)
    - Inner current control loop (default)
- Enable: Enable & Direction (how my firmware works)
    - Enable: Digital Input 2, high active (enabled on high, disabled on low)
    - Direction: Digital Input 3, high active (ccw on high, cw on low)
- Set Value (speed): PWM Set Value (my choice -> higher resolution & pwm library with interrupts)
    - Input: Digital Input 1
    - speed at 10%: 0 rpm
    - speed at 90%: 12000rpm (correct?!)
- Current Limit type: Fixed Current Limit
    - Current limit: 4 A (according to Datasheet motor can take 6 peak...)
- Ramp type: Analog Ramp (//TODO change this to analog in 1 and use pwm on esp32 if setup is working?)
    - Input: Potentiometer 1 (P1 in Datasheet, on board -> Liniar 210°)
    - Ramp at 0%: 0rpm/s
    - Ramp at 100%: 1000rpm/s
- Offset type: Fixed Offset
    - Offset: 0rpm (-> no offset)
- Digital Inputs & Outputs
    - D1 (in) -> PWM - Set Value for Speed (10% -> 0rpm, 90% -> 12000rpm)
    - D2 (in) -> Enable (high -> enable)
    - D3 (in) -> Direction (high -> ccw)
    - D4 (out) -> Speed Comparator (high -> set speed is reached resp. "actual speed average")
- Analog Inputs
    - A1 (in) -> None
    - A2 (in) -> None
    - P1 (in) -> Speed ramp (on board potentiometer - configured above to set ramp speed)
- Analog Output
    - A1 (out) -> None
    - A2 (out) -> None
- Digital Output 4
    - Mode: Limit
    - Polarity: High above limit
    - Direction: CW&CCW
    - Limit: 0rpm



