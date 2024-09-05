#include "TickerController.h"

TickerController::TickerController(): tickers_() {
}

TickerController::~TickerController() {
    this->stopAllTickers();
}

void TickerController::addTicker(WithTicker* ticker) {
  tickers_.push_back(ticker);
}

void TickerController::startAllTickers() {
    Serial.println("Starting Tickers");
    for (auto ticker : tickers_) {
        ticker->startTicker();
    }
}

void TickerController::stopAllTickers() {
    Serial.println("Starting Tickers");
    for (auto ticker : tickers_) {
        ticker->stopTicker();
    }
}

void TickerController::onTimeAdvanced() {
    const unsigned long currentMillis = millis();
    for (auto ticker : tickers_) {
        ticker->onTimeAdvanced(currentMillis);
    }
}