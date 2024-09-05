#include "LedController.hpp"
#include "MyDHT.h"
#include "Buttons.h"
#include "ClockDisplay.h"
#include "MyNTPClient.h"
#include "secrets.h"

// ------------------------- CONFIGURATION -------------------------
// NTP
#define NTP_SERVER "europe.pool.ntp.org"
#define NTP_INTERVAL_S 12 * 3600

// DHT
#define DHT_PIN D4
#define DHT_INTERVAL_S 60

// BUTTONS
#define INTENSITY_BUTTON_PIN D3
#define ON_OFF_BUTTON_PIN D2

// DISPLAY
#define DIN_PIN D7
#define CLK_PIN D5
#define CS_PIN D8

// ------------------------- GLOBAL OBJECTS -------------------------

MyNTPClient ntpClient(NTP_SERVER, WIFI_SSID, WIFI_PASSWORD, NTP_INTERVAL_S);
MyDHT dhtSensor(DHT_PIN, DHT_INTERVAL_S);
Buttons buttons(INTENSITY_BUTTON_PIN, ON_OFF_BUTTON_PIN);
ClockDisplay display(DIN_PIN, CLK_PIN, CS_PIN, &dhtSensor, &ntpClient);

// ------------------------- PROGRAM LOGIC -------------------------
void initTimezoneAndDST() {
    ntpClient.ruleDST("CEST", Last, Sun, Mar, 2, 120); // last sunday in march 2:00, timetone +120min (+1 GMT + 1h summertime offset)
    ntpClient.ruleSTD("CET", Last, Sun, Oct, 3, 60); // last sunday in october 3:00, timezone +60min (+1 GMT)
}

void changeIntensity() {
    display.changeIntensity();
}

void switchSegment() {
    display.switchSegment();
}

void startAllTickers() {
    dhtSensor.startTicker();
    buttons.startTicker();
    display.startTicker();
    ntpClient.startTicker();
}

void advanceAllTickers() {
    unsigned long currentMillis = millis();
    dhtSensor.onTimeAdvanced(currentMillis);
    buttons.onTimeAdvanced(currentMillis);
    display.onTimeAdvanced(currentMillis);
    ntpClient.onTimeAdvanced(currentMillis);
}

void stopAllTickers() {
    dhtSensor.stopTicker();
    buttons.stopTicker();
    display.stopTicker();
    ntpClient.stopTicker();
}

void setup() {
    Serial.begin(9600);
    Serial.println("");
    Serial.println("-------------");
    Serial.println("Initializing:");

    buttons.onIntensityPressed(changeIntensity);
    buttons.onOnOffPressed(switchSegment);

  	display.renderLoading();
    initTimezoneAndDST();
    ntpClient.syncTimeSync();

    Serial.println("Starting Tickers");
    startAllTickers();
}

void loop() {
    advanceAllTickers();
}
