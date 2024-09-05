#ifndef TICKERCONTROLLER_H
#define TICKERCONTROLLER_H

#include "WithTicker.h"

class TickerController {
    public:
        TickerController();
        ~TickerController();

        void addTicker(WithTicker* ticker);
        void startAllTickers();
        void stopAllTickers();
        void onTimeAdvanced();

    private:
        std::vector<WithTicker*> tickers_;
};

#endif //TICKERCONTROLLER_H
