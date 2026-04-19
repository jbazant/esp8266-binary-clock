# AGENTS.md

## Project Overview

ESP8266-based binary clock running on a **Wemos D1 Mini**. The clock displays time in binary format on an **8x8 LED matrix** (MAX7219), along with **lunar phase**, **temperature**, and **humidity** readings from an onboard DHT sensor.

## Tech Stack

- **Platform:** Arduino (ESP8266)
- **Board:** Wemos D1 Mini
- **Language:** C++
- **Display:** 8x8 LED matrix driven by MAX7219 (`LedController` library)
- **Sensor:** DHT temperature & humidity sensor
- **Time sync:** NTP client with automatic DST handling (CET/CEST)
- **3D case:** OpenSCAD (with BOSL library)
- **PCB:** KiCad

## Project Structure

- `esp8266-binary-clock/` — Arduino sketch and source files
  - `esp8266-binary-clock.ino` — Main sketch (setup, loop, configuration)
  - `ClockDisplay.*` — LED matrix rendering (binary clock, lunar phase, temperature/humidity)
  - `MyNTPClient.*` — NTP time synchronization with timezone/DST rules
  - `MyDHT.*` — DHT sensor reading
  - `Buttons.*` — Button input handling (intensity, display mode switching)
  - `TickerController.*` / `WithTicker.*` — Periodic task scheduling
  - `secrets.h` — WiFi credentials (not committed)
- `pcb/` — KiCad PCB design files
- `case/` — OpenSCAD 3D-printable case designs
- `docs/` — Documentation images

## Key Concepts

- **Binary time display:** Hours, minutes, and seconds are shown as binary columns on the 8x8 LED matrix.
- **Display segments:** The display cycles between clock, lunar phase, and temperature/humidity views via button press.
- **Ticker system:** All periodic tasks (NTP sync, sensor reads, button polling, display refresh) are managed by `TickerController` with configurable intervals.
- **Configuration:** Hardware pins, refresh rates, and intervals are defined as `#define` constants in the main `.ino` file.

## Conventions

- Each module is split into `.h` / `.cpp` pairs.
- WiFi credentials are kept in `secrets.h` (see `secrets-example.h` for template).
- The main loop uses a fixed delay (`REFRESH_RATE_MS`) as the base tick; all ticker intervals must be multiples of this value.

