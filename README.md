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
The case is designed in OpenScad and can be found in the `case` folder. Optimized (well as much as I was able to...) for FDM 3D printing.

- optimized for 0.4mm nozzle and 0.2mm layer height
- tested with PLA
- no supports needed

Case is divided into 3 parts:
- top (`case_top.scad`)
- bottom (`case_bottom.scad`)
- display cover (`display_plate.scad`)

One can render all parts with the `case.scad` file.

All the parts should snap together without any screws or glue.

### Dependencies
- **BOSL library** by Revar Desmera ([github](https://github.com/revarbat/BOSL)).
- **Roboto** (bold) font by Christian Robertson ([Google fonts](https://fonts.google.com/specimen/Roboto0)).

### Case top
- (Optional) color change at heights:
  - start with color for "ventilation" grids
  - change color at 1.4mm for the base one

### Case bottom
Case is printed with supports that are included in the model. After printing, remove the "triangle" simply by tilting it back and forth
and "mouse ears" with a knife.

Assembled PCB (optimized for 1.6mm height) should fit into the model without any screws or glue. Some for is needed to snap it in. 

### Display cover
- Use some pretty contrast colors for the display cover. Change filament at the height of 1.2mm.
(Optional) Use nozzle of 0.25mm or smaller for better readability of the labels.
