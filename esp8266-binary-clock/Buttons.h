#ifndef Buttons_h
#define Buttons_h

#include <Arduino.h>
#include "WithTicker.h"

class Buttons : public WithTicker {
  public:
    static volatile bool intensityChangeRequested_;
    static volatile bool switchRequested_;

    Buttons(const uint8_t intensityButtonPin, const uint8_t onOffButtonPin, const uint refreshRate);

    // TODO desctructor
    void onIntensityPressed(void (*callback)());
    void onOnOffPressed(void (*callback)());

  protected:
    void onTick();

  private:
    void (*onIntensityPressed_)();
    void (*onOnOffPressed_)();

    static void ICACHE_RAM_ATTR ISR_intensityButtonPressed_();
    static void ICACHE_RAM_ATTR ISR_onoffButtonPressed_();

    void checkForIntensityUpdate_();
    void checkForSegmentSwitch_();
};

#endif
