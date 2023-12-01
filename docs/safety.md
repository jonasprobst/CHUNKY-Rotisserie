# Safety

## Savety Considerations

- Safety of the stage personell: provide emergency stop and ensure that in case of prolonged dmx communication errors or signal loss the motor is stopped.
- Implement Hardware safety features like fuses and curent limiting.
- Automated Stoping (Loosing DMX Connection, Current Sensor?, temperature sensor?)
- Lightningdesk Personell (Max Speed - Channel 3 to 0), **setup a set all 0 button on Desk**
- Housing of the device (grounded), choice of cables (big where needed), keeping things savely stored away
- Double saftey for things attached to shaft [like this](https://images.app.goo.gl/RfDvGF1uAmsQuWuMA)
- Seperate circuits for DMX Data and Device (Groundloop protection)
- Savety Checklist


## ESCON Servo Driver

The ESCON 36/2 DC has protective circuits against over current, excess temperature, under- and over-voltage, against voltage transients and against short-circuits in the motor cable. Furthermore it is equipped with protected digital inputs and outputs and an adjustable current limitation for protecting the motor and the load. The motor current and the actual speed of the motor shaft can be monitored by means of the analog output voltage.