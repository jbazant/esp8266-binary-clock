#include "LedController.hpp"
#include "MyDHT.h"
#include "Buttons.h"
#include "ClockDisplay.h"
#include "MyNTPClient.h"
#include "secrets.h"

// ------------------------- CONFIGURATION -------------------------
// NTP
#define NTP_SERVER "europe.pool.ntp.org"
//#define NTP_INTERVAL_S 20
#define NTP_INTERVAL_S 12 * 3600

// DHT
#define DHT_PIN D4
#define DHT_INTERVAL_S 60

// BUTTONS
#define INTENSITY_BUTTON_PIN D3
#define ON_OFF_BUTTON_PIN D6

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

  display.renderLoading();
  ntpClient.syncTimeSync();

  buttons.onIntensityPressed(changeIntensity);
  buttons.onOnOffPressed(switchSegment);

  Serial.println("Starting Tickers");
  startAllTickers();
}

void loop() {
  // no loop logic, everything is done through tickers
}
