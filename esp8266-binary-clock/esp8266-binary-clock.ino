#include "LedController.hpp"
#include "MyDHT.h"
#include "Buttons.h"
#include "ClockDisplay.h"
#include "MyNTPClient.h"
#include "WithTicker.h"
#include "TickerController.h"
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
TickerController tickerController;

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

void initSerial() {
    Serial.begin(9600);
    Serial.println("");
    Serial.println("-------------");
}

void registerTickers() {
    tickerController.addTicker(&ntpClient);
    tickerController.addTicker(&dhtSensor);
    tickerController.addTicker(&buttons);
    tickerController.addTicker(&display);
}

void setup() {
    initSerial();
    Serial.println("Initializing:");

  	display.renderLoading();

    buttons.onIntensityPressed(changeIntensity);
    buttons.onOnOffPressed(switchSegment);

    initTimezoneAndDST();
    ntpClient.syncTimeSync();

    registerTickers();
    tickerController.startAllTickers();

    Serial.println("Initialized");
}

void loop() {
    tickerController.onTimeAdvanced();
    delay(200);
}
