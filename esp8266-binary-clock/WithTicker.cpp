#include "WithTicker.h"

WithTicker::WithTicker(const uint interval) : interval_(interval) {
}

void WithTicker::startTicker() {
    ticker_.attach_ms(interval_, std::bind(&WithTicker::onTick, this));
}

void WithTicker::stopTicker() {
    ticker_.detach();
}
