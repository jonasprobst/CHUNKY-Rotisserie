# ESCON Configuration

Following are the settings set via setup wizard in ESCON Studio.
Make sure the firmware is up to date before you precede.

- Speed Constant: 520 rpm/V (Motor Configuration - p.5)
- Thermal time constant winding: 20s (Motor Configuration - p.5)
- Max permissible Speed: 12000 (Motor Configuration - p.5 -> no load speed 12400, recommended speed gearbox 12000)
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
  - Current limit: 4 A (according to Data sheet motor can take 6 peak...)
- Ramp type: Analog Ramp (//TODO change this to analog in 1 and use pwm on esp32 if setup is working?)
  - Input: Potentiometer 1 (P1 in Data sheet, on board -> linear 210°)
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