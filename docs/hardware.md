# Hardware

## Technical Data

- Dimensions (LWH): ca. 55cm x 42cm x 25cm
- Power Supply: 250VAC 50/60Hz, 10A fuse
- Power Inlet: Universal Power Cord C13/T12
- DMX Control Signal: 6 channels, configurable (DMX512)
- DMX Connection: 3 pole XLR in and out (not sure if link works)
- Rotation Modes: Continuous Rotation, (WIP: Positional, Angular)
- Rotation Speed: <= ~20 rpm at output
- Load Limits: ~50kg (shaft down)
- Own weight: ~20kg
- Mounting Clamp: Global Truss 823 Half Coupler, for 48-51mm pipes, 500kg max load
- Motor: 24 VDC, 24 Watt, gear ratio 804:1

## Components

- Power supply: 230VAC to 24VDC, [Power Inlet T12](/docs/datasheets/Power-inlet_IEC-60320_T12.pdf), 10A fuse
  - Switching regulator: 24VDC to 5VCD 1A, [Recom DC/DC converter R-78T5.0-1.0](/docs/datasheets/Recom_R-78T50_1-0.pdf)
- [Drive](/docs/datasheets/Maxon%20Configuration-B74421A7E48E_4.pdf):
  - Motor: Maxon DCX 22S GB KL 24V
  - Planetary gear head: Maxon GPX22 A 138:1
  - Encoder: Maxon ENC30 HEDL 5540 500IMDC7DC converter
- Servo Controller: [Maxon ESCON 36/2 DC](/docs/datasheets/Maxon%20ESCON-36-2-DC-Hardware-Reference-En.pdf), [configuration](/docs/escon_config.md)
- [Sparkfun DMX-Shield](/docs/datasheets/SparkFun_ESP32_Thing_Plus_DMX_to_LED_Shield.pdf)
- [Sparkfun ESP32 Thing Plus C](/docs/datasheets/SparkFun_Thing_Plus_ESP32_Pin%20Configuration.pdf)
- Timing Belt 5M, 9mm
- [Gear head 5M, 12 Teeth](/docs/datasheets/Zahnriemenscheibe_12-5M-9.pdf)
- [Gear head 5M, 18 Teeth](/docs/datasheets/Zahnriemenscheibe_18-5M-9.pdf)
- 3 Mounting Clamp: [Global Truss 823 Half Coupler](/docs/datasheets/GlobalTruss_823-Half-Coupler.jpg), for 48-51mm pipes, 500kg max load
- Repurposed fitness bike frame
- Custom metal housing
- Various wires, crimped
- Various screws, nuts and bolts

### Wiring

- Powerplug (10A Sicherung) -> Netzteil 24VDC
- Netzteil -> 6A Sicherung -> ESCON @ 24VDC
- Netzteil -> 6A Sicherung -> Spannungsregler 5V -> 1A Sicherung -> DMX Controller 5VDC
- DMX Controller 5VDC -> Onboard Spannungswandler 3.3VDC -> ESP32 Thing
- DMX Controller 5VDC -> Optocopler -> DMX Anschluss


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