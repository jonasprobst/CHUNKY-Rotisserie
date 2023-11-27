# CHUNKY Rotisserie

CHUNKY-Rotisserie is a motor device designed to rotate theater props using DMX512 messages. It was invented for the performance "CHUNKY" by [Ernestyna Orlowska](https://www.ernestynaorlowska.ch), first shown at Dampfzentrale in Bern on the 02.12.2023.

This repository contains the firmware and  documentation to understand and operate the device. Heck, one could even try and build a clone. While the software should be easily adapted, the hardware might be more tricky. Since it was built around an old fitness bike frame, the hardware documentation is kept minimal.

![Add image here]

## WARNING

**DO NOT USE, THE DEVICE IF ANY DAMAGE OR ERROR IS FOUND!**

BEFORE EACH USE:

- Inspect the device for damage, wear, corrosion or absuse
- Ensure the device is mounted correctly and savely.
- Ensure the attached load is mounted correctly.

WHEN OPERATING:

- Only operate the device when there is a clear view to the device and area around it.
- Before running the device, ensure that the ‘danger zone’ next to the rotor is cleared to avoid risk of injury.

## Table of Content

- [Quick Start](/docs/quickstart.md)
- [Safety](/docs/safety.md)
- [Technical Data](#technical-data)
- [Hardware Documentation](/docs/hardware.md)
- [Software Documentation](/docs/software.md)

## Technical Data

- Dimensions (LWH):
- Power Supply: 250VAC 50/60Hz
- Power Inlet:
- DMX Control Signal: DMX 512, 6 channels used, channel numbers configurable
- DMX Connection: 3 pole XLR in and out (not sure if link works)
- Rotation Modes: Continuous Rotation, (WIP: Positional, Angular)
- Rotation Speed: >= ~20 rpm
- Load Limits: Shaft-down only! ~50kg
- Own weight: //TODO
- Mounting Clamp: //TODO
- Motor: 24 VDC, //TODO Watt, IP30??

## Special Thanks

- [Jonas](httsp://www.github.com/jonasfehr) for introducing me practically into the world of DMX and the detailed code review
- Jan for his technical consulting and machining custum parts for the hardware
- Sven for being a sparring partner in the mechanical design and answering countless of mechanical engineering questions
- Beno for lending me a motor and mechanical parts to experiment with

## Disclamer

This Project is in the state of a prototype. Any use of an actual device and information in this repository (including but not limited to the code, documentation and any instructions) happens at your own risk.

## License

The chosen license for this repository is [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.html).
