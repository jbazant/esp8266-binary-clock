#include "WithTicker.h"

WithTicker::WithTicker(const uint interval) : interval_(interval) {
}

void WithTicker::startTicker() {
  this->lastTick_ = millis();
  this->isRunning_ = true;
}

void WithTicker::stopTicker() {
  this->isRunning_ = false;
}

void WithTicker::setInterval(const uint interval) {
  this->interval_ = interval;
}

void WithTicker::onTimeAdvanced(const unsigned long currentMillis) {
  if (this->isRunning_ && (currentMillis - this->lastTick_ >= this->interval_)) {
    this->onTick();
    this->lastTick_ = currentMillis;
  }
}
