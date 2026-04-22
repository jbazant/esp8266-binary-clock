#include "LedController.hpp"
#include <ESP8266WiFi.h>
#include "MyDHT.h"
#include "Buttons.h"
#include "ClockDisplay.h"
#include "MyNTPClient.h"
#include "WithTicker.h"
#include "TickerController.h"
#include "MyMQTT.h"
#include "secrets.h"

// ------------------------- CONFIGURATION -------------------------
#define REFRESH_RATE_MS 250

// NTP
#define NTP_SERVER "europe.pool.ntp.org"
#define NTP_INTERVAL_S 12 * 3600

// DHT
#define DHT_PIN D4
#define DHT_INTERVAL_S 60
#define DHT_TEMPERATURE_OFFSET -3.0f
#define DHT_HUMIDITY_OFFSET 0.0f

// BUTTONS
#define INTENSITY_BUTTON_PIN D3
#define ON_OFF_BUTTON_PIN D2
#define BUTTONS_READ_INTERVAL_MS 500

// DISPLAY
#define DIN_PIN D7
#define CLK_PIN D5
#define CS_PIN D8

// MQTT
#define MQTT_PUBLISH_INTERVAL_S 10 * 60

// ------------------------- GLOBAL OBJECTS -------------------------

MyNTPClient ntpClient(NTP_SERVER, WIFI_SSID, WIFI_PASSWORD, NTP_INTERVAL_S,
                      IPAddress(STATIC_IP), IPAddress(GATEWAY_IP), IPAddress(SUBNET_MASK), IPAddress(DNS_IP));
MyDHT dhtSensor(DHT_PIN, DHT_INTERVAL_S, DHT_TEMPERATURE_OFFSET, DHT_HUMIDITY_OFFSET);
Buttons buttons(INTENSITY_BUTTON_PIN, ON_OFF_BUTTON_PIN, BUTTONS_READ_INTERVAL_MS);
ClockDisplay display(DIN_PIN, CLK_PIN, CS_PIN, &dhtSensor, &ntpClient, REFRESH_RATE_MS);
TickerController tickerController;
MyMQTT mqttClient(&dhtSensor, WIFI_SSID, WIFI_PASSWORD, MQTT_HOST, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_TOPIC_PREFIX, MQTT_PUBLISH_INTERVAL_S,
                  IPAddress(STATIC_IP), IPAddress(GATEWAY_IP), IPAddress(SUBNET_MASK), IPAddress(DNS_IP));

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
    tickerController.addTicker(&mqttClient);
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
    // note that this is a smallest delay possible, all ticker must be multiples of this delay
    delay(REFRESH_RATE_MS);
}
