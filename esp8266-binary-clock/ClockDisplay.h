#ifndef ClockDisplay_h
#define ClockDisplay_h

#include "LedController.hpp"
#include "MyDHT.h"
#include "MyNTPClient.h"
#include "WithTicker.h"

class ClockDisplay: public WithTicker {
  public:
    ClockDisplay(const uint8_t dinPin, const uint8_t clkPin, const uint8_t csPin, MyDHT* dhtSensor, MyNTPClient* ntpClient);

    void changeIntensity();
    void switchSegment();
    void renderLoading();

  protected:
    void onTick();

  private:
    LedController<1, 1> lc_;
    MyDHT* dhtSensor_;
    MyNTPClient* ntpClient_;

    uint8_t currentIntensity_;
    bool isSegmentOn_;

    uint dowShifted_();
    void renderClockRows_();
    void renderDHTRows_();
    void render_();

    uint8_t getDHTFlags_();
    uint8_t getNTPFlags_();

  // TODO moonphase
  // https://github.com/Sheaffy/ArduinoMoonPhase/blob/master/MoonPhase.cpp

  // TODO moon shifted
};

#endif
