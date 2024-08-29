#include "ClockDisplay.h"

ClockDisplay::ClockDisplay(const uint8_t dinPin, const uint8_t clkPin, const uint8_t csPin, MyDHT* dhtSensor, MyNTPClient* ntpClient)
: WithTicker(250), currentIntensity_(10), isSegmentOn_(true), lc_(LedController<1, 1>()), dhtSensor_(dhtSensor), ntpClient_(ntpClient)
{
  this->lc_.init(dinPin, clkPin, csPin);
  this->lc_.clearMatrix();
  this->lc_.activateSegment(0);
  this->lc_.setIntensity(this->currentIntensity_);
}

void ClockDisplay::switchSegment() {
  this->isSegmentOn_ = !this->isSegmentOn_;
  this->isSegmentOn_ ? this->lc_.activateSegment(0) : this->lc_.shutdownSegment(0);
}

void ClockDisplay::changeIntensity() {
    this->currentIntensity_ += 5;
    if (this->currentIntensity_ > 15) {
      this->currentIntensity_ = 0;
    }

    this->lc_.setIntensity(this->currentIntensity_);
}

void ClockDisplay::renderLoading() {
  this->lc_.setRow(0, 7, 0b00000000);
  this->lc_.setRow(0, 6, 0b00011100);
  this->lc_.setRow(0, 5, 0b00101010);
  this->lc_.setRow(0, 4, 0b01001001);
  this->lc_.setRow(0, 3, 0b01111001);
  this->lc_.setRow(0, 2, 0b01000001);
  this->lc_.setRow(0, 1, 0b00100010);
  this->lc_.setRow(0, 0, 0b00011100);
}

void ClockDisplay::onTick() {
  this->renderClockRows_();
  this->renderDHTRows_();
}

void ClockDisplay::renderClockRows_() {
  this->lc_.setRow(0, 7, this->ntpClient_->seconds() + this->getNTPFlags_());
  this->lc_.setRow(0, 6, this->ntpClient_->minutes() + this->getDHTFlags_());
  this->lc_.setRow(0, 5, this->ntpClient_->hours());
  this->lc_.setRow(0, 4, this->ntpClient_->day() + this->dowShifted_());
  this->lc_.setRow(0, 3, this->ntpClient_->month()); // TODO add moon phase
  this->lc_.setRow(0, 2, this->ntpClient_->year() % 100);
}

void ClockDisplay::renderDHTRows_() {
  if (this->dhtSensor_->hasData()) {
    const int temperature = this->dhtSensor_->getTemperature();
    this->lc_.setRow(0, 0, temperature + (temperature < 0 ? 128 : 0));
    this->lc_.setRow(0, 1, this->dhtSensor_->getHumidity());
  } else {
    this->lc_.setRow(0, 1, 0);
    this->lc_.setRow(0, 0, 0);
  }
}

uint ClockDisplay::dowShifted_() {
  // 1 = mon ... 7 = sun
  const int dowFromMonday = (this->ntpClient_->weekDay() + 6) % 7 + 1;
  return dowFromMonday << 5;
}

uint8_t ClockDisplay::getDHTFlags_() {
  uint8_t flags = 0;

  if (this->dhtSensor_->isReading()) {
    flags += 128;
  }

  if (this->dhtSensor_->hasError()) {
    flags += 64;
  }

  return flags;
}

uint8_t ClockDisplay::getNTPFlags_() {
  uint8_t flags = 0;

  if (this->ntpClient_->isSyncing()) {
    flags += 128;
  }

  return flags;
}