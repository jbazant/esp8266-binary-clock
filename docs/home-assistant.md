# Home Assistant Integration

The clock publishes temperature and humidity readings via MQTT every 10 minutes (configurable via `MQTT_PUBLISH_INTERVAL_S`).

## MQTT Sensor Configuration

Add the following to your Home Assistant `configuration.yaml`:

```yaml
mqtt:
  sensor:
    - name: "Binary Clock Temperature"
      unique_id: binary_clock_temperature
      state_topic: "home/binary-clock/temperature"
      unit_of_measurement: "°C"
      device_class: temperature
      state_class: measurement
      expire_after: 1200  # mark unavailable if no update for 20 minutes (2x publish interval)

    - name: "Binary Clock Humidity"
      unique_id: binary_clock_humidity
      state_topic: "home/binary-clock/humidity"
      unit_of_measurement: "%"
      device_class: humidity
      state_class: measurement
      expire_after: 1200  # mark unavailable if no update for 20 minutes (2x publish interval)
```

> **Note:** Adjust `state_topic` values if you changed `MQTT_TOPIC_PREFIX` in `secrets.h`.
> The prefix maps directly: `<MQTT_TOPIC_PREFIX>/temperature` and `<MQTT_TOPIC_PREFIX>/humidity`.

## Automation Example

An example automation that alerts when humidity exceeds 70%:

```yaml
automation:
  - alias: "Binary Clock High Humidity Alert"
    trigger:
      - platform: numeric_state
        entity_id: sensor.binary_clock_humidity
        above: 70
    action:
      - service: notify.notify
        data:
          message: "High humidity: {{ states('sensor.binary_clock_humidity') }}%"
```

## Lovelace Card Example

```yaml
type: entities
title: Binary Clock Sensors
entities:
  - entity: sensor.binary_clock_temperature
    name: Temperature
  - entity: sensor.binary_clock_humidity
    name: Humidity
```

