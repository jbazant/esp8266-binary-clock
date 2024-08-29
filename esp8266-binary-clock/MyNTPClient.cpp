#include "MyNTPClient.h"


MyNTPClient::MyNTPClient(const char* ntpServer, const char* ssid, const char* password, const uint interval)
    : NTP(wifiUdp_), WithTicker(interval * 1000), isSyncing_(false), ntpServer_(ntpServer), ssid_(ssid), password_(password)
{
    this->initTZ();
}

void MyNTPClient::syncTimeAsync() {
    this->stopTicker();
    this->isSyncing_ = true;
    this->connectToWiFi_();
    // TODO move to constant
    this->ticker_.attach_ms(1000, std::bind(&MyNTPClient::onWifiConnectTick, this));
}

bool MyNTPClient::isSyncing() {
  return this->isSyncing_;
}

void MyNTPClient::initTZ() {
    this->ruleDST("CEST", Last, Sun, Mar, 2, 120); // last sunday in march 2:00, timetone +120min (+1 GMT + 1h summertime offset)
    this->ruleSTD("CET", Last, Sun, Oct, 3, 60); // last sunday in october 3:00, timezone +60min (+1 GMT)
}

void MyNTPClient::syncTimeSync() {
    this->connectToWiFi_();
    this->waitForWiFiSync_();
    this->syncTime_();
    this->disconnectFromWiFi_();
}

void MyNTPClient::onTick() {
    this->syncTimeAsync();
}

void MyNTPClient::onWifiConnectTick() {
    if (WiFi.status() == WL_CONNECTED) {
        // TODO timeout, TODO error handling
        Serial.println("Connected");
        this->stopTicker();
        this->syncTime_();
        this->disconnectFromWiFi_();
        this->isSyncing_ = false;
        this->startTicker();
    } else {
        Serial.print(WiFi.status());
        Serial.print("-");
    }
}

void MyNTPClient::connectToWiFi_() {
    Serial.print("Connecting to WiFi: ");
    WiFi.begin(this->ssid_, this->password_);
}

void MyNTPClient::waitForWiFiSync_() {
    while(WiFi.status() != WL_CONNECTED) {
        // TODO timeout, TODO error handling
        delay(1000);
        Serial.print(WiFi.status());
        Serial.print("-");
    }

    Serial.println("Connected");
}

void MyNTPClient::disconnectFromWiFi_() {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    Serial.println("Disconnected from WiFi");
}

void MyNTPClient::syncTime_() {
    Serial.println("Syncing time");
    this->begin(this->ntpServer_);

    Serial.print("Time synced: ");
    Serial.println(this->formattedTime("%T %A, %d. %B %Y"));
}
