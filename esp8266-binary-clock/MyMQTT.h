#ifndef MYMQTT_H
#define MYMQTT_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "WithTicker.h"
#include "MyDHT.h"

class MyMQTT : public WithTicker {
    public:
        MyMQTT(MyDHT* dht,
               const char* ssid, const char* password,
               const char* brokerHost, const uint16_t brokerPort,
               const char* mqttUser, const char* mqttPassword,
               const char* topicPrefix,
               const uint intervalSeconds,
               IPAddress staticIP = IPAddress(0,0,0,0), IPAddress gateway = IPAddress(0,0,0,0),
               IPAddress subnet = IPAddress(255,255,255,0), IPAddress dns = IPAddress(8,8,8,8));

    protected:
        void onTick();

    private:
        MyDHT* dht_;
        const char* ssid_;
        const char* password_;
        const char* brokerHost_;
        uint16_t brokerPort_;
        const char* mqttUser_;
        const char* mqttPassword_;
        const char* topicPrefix_;
        IPAddress staticIP_;
        IPAddress gateway_;
        IPAddress subnet_;
        IPAddress dns_;

        WiFiClient wifiClient_;
        PubSubClient mqttClient_;

        bool connectWifi_();
        void disconnectWifi_();
        bool connectMqtt_();
        void publish_();
        void publishValue_(const char* subtopic, int value);
};

#endif // MYMQTT_H

