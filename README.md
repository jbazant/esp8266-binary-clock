# WEMOS D1 mini Binary clock

This is a private project. No support is provided.

Binary clock based on a WEMOS D1 mini and a 8x8 LED matrix. Shows the seconds, minutes, hours, day in month, day in week, month, moon phase and last two decimal digits of year in binary format.
Also uses a DHT sensor to show the temperature and humidity.
Time is synchronized via NTP.

Two buttons allow to disable display and set the brightness.


## Hardware
- WEMOS D1 mini
- 8x8 LED matrix with MAX7219 controller
- DHT22 sensor
- 2 buttons
- 3 resistors 10kOhm (2 for the buttons, 1 for the DHT sensor)


## Software
The software is written as arduino project mostly in C++ and can be found in the `esp8266-binary-clock` folder (to satisfy arduino project structure).

### Dependencies
- **DHT library** by Bert Melis ([github](https://github.com/bertmelis/DHT))
- **NTP library** by Stefan Staub ([github](https://github.com/sstaub/NTP))
- **LedController** library by Noah Kirschmann ([github](https://github.com/noah1510/LedController))
- **moonPhaser** library by Steve Sienkowski ([github](https://github.com/steve-sienk/moonPhaser-avr0))

### Configuration
- Create `secrets.h` in the `esp8266-binary-clock` and specify your WiFi credentials. (see `secrets.h.example`)
- Adjust `CONFIGURATION` section and `initTimezoneAndDST` method in `esp8266-binary-clock.ino` to your needs.


## PCB
The PCB is designed in KiCad and can be found in the `pcb` folder.


## Case
The case is designed in OpenScad and can be found in the `case` folder. Optimized for FDM 3D printing.

**Printable case design is currently in process**