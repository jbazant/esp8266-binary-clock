#include "HardwareSerial.h"
#include "MyDHT.h"

void ICACHE_RAM_ATTR MyDHT::ISR_handleData_(float h, float t) {
  this->humidity_ = h;
  this->temperature_ = t;
  this->hasData_ = true;
  this->hasError_ = false;
  Serial.print("DHT data received: ");
  Serial.print(this->getTemperature());
  Serial.print("°C ");
  Serial.print(this->getHumidity());
  Serial.println("%");
  this->isReading_ = false;
}

void ICACHE_RAM_ATTR MyDHT::ISR_handleError(uint8_t e) {
  this->hasError_ = true;
  Serial.print("DHT error: ");
  Serial.println(e);
  this->isReading_ = false;
}

MyDHT::MyDHT(const uint8_t pin, const uint interval)
    : WithTicker(interval * 1000), hasData_(false), hasError_(false), isReading_(false)
{
  this->sensor.setPin(pin);
  this->sensor.onData([&](float h, float t){
    this->ISR_handleData_(h, t);
  });
  this->sensor.onError([&](uint8_t e){
    this->ISR_handleError(e);
  });

  this->onTick();
}

void MyDHT::onTick() {
  this->sensor.read();
  if (this->isReading()) {
    this->hasError_ = true;
  }
  this->isReading_ = true;
}

bool MyDHT::hasData() {
  return this->hasData_;
}

bool MyDHT::hasError() {
  return this->hasError_;
}

bool MyDHT::isReading() {
  return this->isReading_;
}

int MyDHT::getTemperature() {
  return int(trunc(this->temperature_));
}

int MyDHT::getHumidity() {
  return int(trunc(this->humidity_));
}
