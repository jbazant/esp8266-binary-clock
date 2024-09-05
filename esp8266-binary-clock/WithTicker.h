#ifndef WITHTICKER_H
#define WITHTICKER_H

#include <Arduino.h>

/*
  We are unable to use native Ticker library, because NTP library uses delay and will cause crashes.
  Therefore here is minimal implementation of our "Ticker" class.
*/
class WithTicker {
    public:
        WithTicker(const uint interval);
        void startTicker();
        void stopTicker();
        void setInterval(const uint interval);
        void onTimeAdvanced(const unsigned long currentMillis);

    protected:
        virtual void onTick() = 0;

    private:
        uint interval_;
        bool isRunning_;
        unsigned long lastTick_;
};


#endif //WITHTICKER_H
