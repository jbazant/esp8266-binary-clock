#ifndef MyDHT_h
#define MyDHT_h

#include <Arduino.h>
#include <DHT.h>
#include "WithTicker.h"

class MyDHT : public WithTicker {
  public:
    MyDHT(const uint8_t pin, const uint interval, const float temperatureOffset = 0.0f, const float humidityOffset = 0.0f);

    // TODO desctructor

    bool hasData();
    bool hasError();
    bool isReading();
    float getTemperature();
    float getHumidity();

  protected:
    void onTick();

  private:
    DHT22 sensor;

    volatile bool hasData_;
    volatile float temperature_;
    volatile float humidity_;
    volatile bool hasError_;
    volatile bool isReading_;
    float temperatureOffset_;
    float humidityOffset_;


    void ICACHE_RAM_ATTR ISR_handleData_(float h, float t);
    void ICACHE_RAM_ATTR ISR_handleError(uint8_t e);
    void readDHT_();
};

#endif
