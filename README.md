# WEMOS D1 mini Binary clock

Binary clock based on a WEMOS D1 mini and a 8x8 LED matrix. Shows the seconds, minutes, hours, day in month, day in week, month, moon phase and last two decimal digits of year in binary format.
Also uses a DHT sensor to show the temperature and humidity.
Time is synchronized via NTP.

Two buttons allow to disable display and set the brightness.

## Dependencies
- **DHT library** by Bert Melis ([github](https://github.com/bertmelis/DHT))
- **NTP library** by Stefan Staub ([github](https://github.com/sstaub/NTP))
- **LedController** library by Noah Kirschmann ([github](https://github.com/noah1510/LedController))

## Case
The case is designed in OpenScad and can be found in the `case` folder.