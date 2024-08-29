#ifndef WITHTICKER_H
#define WITHTICKER_H

#include <Ticker.h>


class WithTicker {
    public:
        WithTicker(const uint interval);
        void startTicker();
        void stopTicker();

    protected:
        virtual void onTick() = 0;
        Ticker ticker_;

    private:
        uint interval_;
};


#endif //WITHTICKER_H
