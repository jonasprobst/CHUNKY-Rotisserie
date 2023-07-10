# TODO

- [x] Update libraries to handle the new Settings and NVSStorage class
- [x] update main and test
- [ ] Setup test hardware (DMX, UI, small Driver, small Motor)
- [ ] Document Hardware setup and Pin Config
- [ ] Test DMX reception
- [ ] Finish the UI class
- [ ] Update main and test UI integration
- [ ] Research modes from Wahlberg Rotator 
- [ ] Implement a basic MotorController for one of my small drivers and motors
- [ ] update main and test Motor Implementation
- [ ] write some tests and test things well considering personnel safety (write a e2e test protocol)

- [x] get final motor specifications -> max 10kg
- [ ] order motor if needed
- [ ] update final motor driver class
- [ ] test everything again thoroughly
- [ ] setup git hub
- [ ]upload to github
- [ ] ask for a code view

- [ ] design construction
- [ ] design housing
- [ ] assemble
- [ ] test, test, test (emergency Stops! Blocked motor, fuse, signal loss, and other edge cases)
- [ ] document things in the README.md for future me and technician
- [ ] celebrate

- [ ] test on stage, fix, repeat
- [ ] test real setup with construction
- [ ] test with real people
- [ ] document everything
- [ ] celebrate

## Notes

### On Interference

Ferrite choke:

1. **Frequency Range**: As you're working in a stage setting, you'll be dealing with a mix of frequencies. As a starting point, look for a choke that covers a wide range of frequencies - anywhere from 1 MHz to 1 GHz should cover most of the high-frequency noise you're likely to encounter.
2. **Size and Shape**: The specific size will depend on the thickness of your PowerCon cables. As an estimate, a ferrite choke with an inner diameter of about 0.5 inches (around 12-13mm) should be adequate for most power cables. Again, measure your cables to be sure. A snap-on or clip-on ferrite choke will be the most convenient for this application.
3. **Rated Current**: The specific current rating will depend on the current your device will draw. As a safe estimate, a choke rated for at least 5-10 Amps should provide a good safety margin for most applications. But you should check the power requirements of your devices to make sure.
4. **Material**: Ferrite chokes are made of different materials classified by "material types". The types are usually denoted by a number. For dealing with high-frequency noise, Material Type 43 or 61 could be a good starting point. However, check the datasheet for the specific frequency performance of each material type.