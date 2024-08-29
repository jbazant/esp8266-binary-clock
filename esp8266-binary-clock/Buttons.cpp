#include "Buttons.h"

volatile bool Buttons::intensityChangeRequested_ = false;
volatile bool Buttons::switchRequested_ = false;

void ICACHE_RAM_ATTR Buttons::ISR_intensityButtonPressed_() {
  Buttons::intensityChangeRequested_ = true;
}

void ICACHE_RAM_ATTR Buttons::ISR_onoffButtonPressed_() {
  Buttons::switchRequested_ = true;
}

Buttons::Buttons(const uint8_t intensityButtonPin, const uint8_t onOffButtonPin): WithTicker(300) {
  pinMode(intensityButtonPin, INPUT);
  pinMode(onOffButtonPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(intensityButtonPin), Buttons::ISR_intensityButtonPressed_, RISING);
  attachInterrupt(digitalPinToInterrupt(onOffButtonPin),  Buttons::ISR_onoffButtonPressed_, RISING);
}

void Buttons::onIntensityPressed(void (*callback)()) {
  this->onIntensityPressed_ = callback;
}

void Buttons::onOnOffPressed(void (*callback)()) {
    this->onOnOffPressed_ = callback;
}

void Buttons::onTick() {
  checkForSegmentSwitch_();
  checkForIntensityUpdate_();
}

void Buttons::checkForIntensityUpdate_() {
  if (Buttons::intensityChangeRequested_) {
    Buttons::intensityChangeRequested_ = false;
    this->onIntensityPressed_();
  }
}

void Buttons::checkForSegmentSwitch_() {
  if (Buttons::switchRequested_) {
    Buttons::switchRequested_ = false;
    this->onOnOffPressed_();
  }
}
