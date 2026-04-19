#include "MyMQTT.h"
#define WIFI_CONN_TIMEOUT 30000
#define WIFI_CONN_TICK    1000
#define MQTT_CONN_TIMEOUT 5000
MyMQTT::MyMQTT(MyDHT* dht,
               const char* ssid, const char* password,
               const char* brokerHost, const uint16_t brokerPort,
               const char* mqttUser, const char* mqttPassword,
               const char* topicPrefix,
               const uint intervalSeconds,
               IPAddress staticIP, IPAddress gateway, IPAddress subnet, IPAddress dns)
    : WithTicker(intervalSeconds * 1000),
      dht_(dht), ssid_(ssid), password_(password),
      brokerHost_(brokerHost), brokerPort_(brokerPort),
      mqttUser_(mqttUser), mqttPassword_(mqttPassword),
      topicPrefix_(topicPrefix),
      staticIP_(staticIP), gateway_(gateway), subnet_(subnet), dns_(dns),
      mqttClient_(wifiClient_)
{}

void MyMQTT::onTick() {
    Serial.println("MQTT: starting publish cycle");
    if (!dht_->hasData()) {
        Serial.println("MQTT: no DHT data available, skipping");
        return;
    }
    if (!connectWifi_()) {
        Serial.println("MQTT: WiFi connection failed, skipping");
        return;
    }
    if (!connectMqtt_()) {
        Serial.println("MQTT: broker connection failed, skipping");
        disconnectWifi_();
        return;
    }
    publish_();
    mqttClient_.disconnect();
    disconnectWifi_();
    Serial.println("MQTT: publish cycle done");
}

bool MyMQTT::connectWifi_() {
    if (staticIP_ != IPAddress(0, 0, 0, 0)) {
        WiFi.config(staticIP_, gateway_, subnet_, dns_);
    }
    WiFi.persistent(true);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid_, password_);
    Serial.print("MQTT: connecting to WiFi");
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start >= WIFI_CONN_TIMEOUT) {
            Serial.println(" timed out");
            return false;
        }
        switch (WiFi.status()) {
            case WL_NO_SSID_AVAIL:
                Serial.println(" no SSID");
                return false;
            case WL_CONNECT_FAILED:
                Serial.println(" failed");
                return false;
            case WL_NO_SHIELD:
                Serial.println(" no shield");
                return false;
            default:
                break;
        }
        delay(WIFI_CONN_TICK);
        Serial.print(".");
    }
    Serial.println(" connected");
    return true;
}

void MyMQTT::disconnectWifi_() {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    Serial.println("MQTT: WiFi disconnected");
}

bool MyMQTT::connectMqtt_() {
    mqttClient_.setServer(brokerHost_, brokerPort_);
    Serial.print("MQTT: connecting to broker");
    unsigned long start = millis();
    while (!mqttClient_.connected()) {
        if (millis() - start >= MQTT_CONN_TIMEOUT) {
            Serial.print(" timed out, state=");
            Serial.println(mqttClient_.state());
            return false;
        }
        mqttClient_.connect("esp8266-binary-clock", mqttUser_, mqttPassword_);
        if (!mqttClient_.connected()) {
            delay(500);
            Serial.print(".");
        }
    }
    Serial.println(" connected");
    return true;
}

void MyMQTT::publishValue_(const char* subtopic, int value) {
    char topic[128];
    char valStr[8];

    snprintf(topic, sizeof(topic), "%s/%s", topicPrefix_, subtopic);
    snprintf(valStr, sizeof(valStr), "%d", value);
    mqttClient_.publish(topic, valStr, true);

    Serial.print("MQTT: published ");
    Serial.print(topic);
    Serial.print(" = ");
    Serial.println(valStr);
}

void MyMQTT::publish_() {
    publishValue_("temperature", dht_->getTemperature());
    publishValue_("humidity", dht_->getHumidity());
}
