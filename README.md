# Smart Plants Arduino

## Components
- L293D dc motor controller + 5v dc motor
- RC522 rfid reader
- Moisture Sensor
- MAX7219 LED matrix
- Bluetooth HC-05

## Pin Config
See config.h

## API
Send following byte commands over Serial (9600 baud rate). Send additional byte for parameter

| Command | Byte Param | Description |
| --- | --- | --- |
| 0 | <none> | Request full state of the device |
| 1 | 1 or 0 | Change motor management between Manual and Automatic |
| 2 | <none> | Turn the motor on (must be in Manual mode) |
| 3 | <none> | Turn the motor off (must be in Manual mode) |
| 4 | <none> | Change motor speed |
| 5 | <none> | Turn LED lights on |
| 6 | <none> | Turn LED lights off |
| 7 | 0, 1, 2 | Change LED Brightness. 0(LOW), 1(MEDIUM), 2(HIGH) |
| 8 | <none> | Initial handshake to determine if bluetooth device is a smartplant |
