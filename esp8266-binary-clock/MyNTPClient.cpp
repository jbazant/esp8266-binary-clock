#include "MyNTPClient.h"

#define WIFI_CONN_TIMEOUT 30000
#define WIFI_CONN_TICK 1000

MyNTPClient::MyNTPClient(const char* ntpServer, const char* ssid, const char* password, const uint interval)
    : NTP(wifiUdp_), WithTicker(interval * 1000), isSyncing_(false), hasError_(false), ntpServer_(ntpServer),
      ssid_(ssid), password_(password), defaultSyncInterval_(interval * 1000), wifiSyncInterval_(WIFI_CONN_TICK)
{}

bool MyNTPClient::isSyncing() {
  return this->isSyncing_;
}

bool MyNTPClient::hasError() {
  return this->hasError_;
}

void MyNTPClient::syncTimeAsync() {
    this->isSyncing_ = true;
    this->setInterval(this->wifiSyncInterval_);

    // Connect to WiFi and wait for connection
    this->connectToWiFi_();
}

void MyNTPClient::syncTimeSync() {
    this->isSyncing_ = true;
    this->connectToWiFi_();
    this->waitForWiFiSync_();
    this->onWifiConnectionResolved();
}

void MyNTPClient::onTick() {
  	if (this->isSyncing_) {
    	this->onWifiConnectTick();
  	} else {
    	this->syncTimeAsync();
  	}
}

void MyNTPClient::onWifiConnectTick() {
  	bool keepPolling = this->checkWiFiStatus_();
    if (!keepPolling) {
        this->onWifiConnectionResolved();
    }
}

void MyNTPClient::connectToWiFi_() {
    Serial.print("Connecting to WiFi: ");
    WiFi.begin(this->ssid_, this->password_);
    this->pollRetries_ = WIFI_CONN_TIMEOUT / WIFI_CONN_TICK;
}

void MyNTPClient::waitForWiFiSync_() {
	while (this->checkWiFiStatus_()) {
      	delay(WIFI_CONN_TICK);
    }
}

bool MyNTPClient::checkWiFiStatus_() {
    bool keepPolling = true;

    // https://realglitch.com/2018/07/arduino-wifi-status-codes/
    switch (WiFi.status()) {
        case WL_CONNECTED:
            this->hasError_ = false;
            Serial.println("Connected");
            keepPolling = false;
            break;

        case WL_NO_SSID_AVAIL:
            this->hasError_ = true;
            Serial.println("No SSID available");
            keepPolling = false;
            break;

        case WL_CONNECT_FAILED:
            this->hasError_ = true;
            Serial.println("Connection failed");
            keepPolling = false;
            break;

        case WL_NO_SHIELD:
            this->hasError_ = true;
            Serial.println("No shield");
            keepPolling = false;
            break;

        default:
            Serial.print(WiFi.status());
            Serial.print("-");
            break;
    }

    return keepPolling && this->pollRetries_-- > 0;
}

void MyNTPClient::onWifiConnectionResolved() {
    // if WiFi is connected, sync time
    if (!this->hasError_) {
        this->syncTime_();
    }

    this->disconnectFromWiFi_();
    this->isSyncing_ = false;
    this->setInterval(this->defaultSyncInterval_);
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
