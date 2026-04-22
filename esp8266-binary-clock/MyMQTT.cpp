#include "MyMQTT.h"

#define WIFI_CONN_TIMEOUT 30000
#define WIFI_CONN_TICK    1000
#define MQTT_CONN_TIMEOUT 5000
#define MQTT_CONN_TICK    500
#define MQTT_BEFORE_DISCONNECT_TICK 3000

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
      mqttClient_(wifiClient_),
      defaultInterval_(intervalSeconds * 1000),
      wifiSyncInterval_(WIFI_CONN_TICK),
      mqttSyncInterval_(MQTT_CONN_TICK),
      mqttDisconnectInterval_(MQTT_BEFORE_DISCONNECT_TICK)
{}

void MyMQTT::onTick() {
    switch (this->state_) {
        case State::IDLE:
            this->startPublishCycle_();
            break;
        case State::WIFI_CONNECTING:
            this->onWifiConnectTick_();
            break;
        case State::MQTT_CONNECTING:
            this->onMqttConnectTick_();
            break;
        case State::MQTT_PUBLISHED:
            this->onMqttPublishedTick_();
            break;
        case State::MQTT_DISCONNECTING:
            this->onMqttDisconnectTick_();
            break;
        default:
            break;
    }
}

void MyMQTT::startPublishCycle_() {
    Serial.println("MQTT: starting publish cycle");
    if (!this->dht_->hasData()) {
        Serial.println("MQTT: no DHT data available, skipping");
        return;
    }

    if (this->staticIP_ != IPAddress(0, 0, 0, 0)) {
        WiFi.config(this->staticIP_, this->gateway_, this->subnet_, this->dns_);
    }
    WiFi.persistent(true);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin(this->ssid_, this->password_);
    Serial.print("MQTT: connecting to WiFi");

    this->pollRetries_ = WIFI_CONN_TIMEOUT / WIFI_CONN_TICK;
    this->state_ = State::WIFI_CONNECTING;
    this->setInterval(this->wifiSyncInterval_);
}

void MyMQTT::onWifiConnectTick_() {
    wl_status_t status = WiFi.status();

    if (status == WL_CONNECTED) {
        Serial.println(" connected");
        this->onWifiConnectionResolved_();
        return;
    }
    if (status == WL_NO_SSID_AVAIL || status == WL_CONNECT_FAILED || status == WL_NO_SHIELD) {
        Serial.println(" WiFi error");
        this->disconnectWifi_();
        this->state_ = State::IDLE;
        this->setInterval(this->defaultInterval_);
        return;
    }
    if (this->pollRetries_-- <= 0) {
        Serial.println(" timed out");
        this->disconnectWifi_();
        this->state_ = State::IDLE;
        this->setInterval(this->defaultInterval_);
        return;
    }
    Serial.print(".");
}

void MyMQTT::onWifiConnectionResolved_() {
    this->mqttClient_.setServer(this->brokerHost_, this->brokerPort_);
    Serial.print("MQTT: connecting to broker");
    this->state_ = State::MQTT_CONNECTING;
    this->pollRetries_ = MQTT_CONN_TIMEOUT / MQTT_CONN_TICK;
    this->setInterval(this->mqttSyncInterval_);
    // Attempt first connection immediately
    this->mqttClient_.connect("esp8266-binary-clock", this->mqttUser_, this->mqttPassword_);
}

void MyMQTT::onMqttConnectTick_() {
    if (this->mqttClient_.connected()) {
        Serial.println(" connected");
        this->publish_();
        Serial.println("MQTT: published, waiting before disconnect");
        this->state_ = State::MQTT_PUBLISHED;
        this->setInterval(this->mqttDisconnectInterval_);
        return;
    }
    if (this->pollRetries_-- <= 0) {
        Serial.print(" timed out, state=");
        Serial.println(this->mqttClient_.state());
        this->disconnectWifi_();
        this->state_ = State::IDLE;
        this->setInterval(this->defaultInterval_);
        return;
    }
    Serial.print(".");
}

void MyMQTT::onMqttPublishedTick_() {
    Serial.print("MQTT: disconnecting from broker");
    this->mqttClient_.disconnect();
    this->pollRetries_ = MQTT_CONN_TIMEOUT / MQTT_CONN_TICK;
    this->state_ = State::MQTT_DISCONNECTING;
    this->setInterval(this->mqttSyncInterval_);
}

void MyMQTT::onMqttDisconnectTick_() {
    if (!this->mqttClient_.connected()) {
        Serial.println(" disconnected");
        this->disconnectWifi_();
        this->state_ = State::IDLE;
        this->setInterval(this->defaultInterval_);
        return;
    }

    if (this->pollRetries_-- <= 0) {
        Serial.print(" timed out, state=");
        Serial.println(this->mqttClient_.state());
        this->disconnectWifi_();
        this->state_ = State::IDLE;
        this->setInterval(this->defaultInterval_);
        return;
    }

    Serial.print(".");
}

void MyMQTT::disconnectWifi_() {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    Serial.println("MQTT: WiFi disconnected");
}

void MyMQTT::publishValue_(const char* subtopic, float value) {
    char topic[128];
    char valStr[16];

    snprintf(topic, sizeof(topic), "%s/%s", this->topicPrefix_, subtopic);
    snprintf(valStr, sizeof(valStr), "%.1f", value);
    this->mqttClient_.publish(topic, valStr, true);

    Serial.print("MQTT: published ");
    Serial.print(topic);
    Serial.print(" = ");
    Serial.println(valStr);
}

void MyMQTT::publish_() {
    this->publishValue_("temperature", this->dht_->getTemperature());
    this->publishValue_("humidity", this->dht_->getHumidity());
}
