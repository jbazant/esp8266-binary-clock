#ifndef MYNTPCLIENT_H
#define MYNTPCLIENT_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "WiFiUdp.h"
#include "NTP.h"
#include "WithTicker.h"


class MyNTPClient : public NTP, public WithTicker {
    public:
        MyNTPClient(const char* ntpServer, const char* ssid, const char* password, const uint interval,
                    IPAddress staticIP = IPAddress(0,0,0,0), IPAddress gateway = IPAddress(0,0,0,0),
                    IPAddress subnet = IPAddress(255,255,255,0), IPAddress dns = IPAddress(8,8,8,8));
        void syncTimeAsync();
        void syncTimeSync();

        bool isSyncing();
        bool hasError();

    protected:
        void onTick();
        void onWifiConnectTick();

    private:
        WiFiUDP wifiUdp_;
        bool isSyncing_;
        bool hasError_;
        bool readyForNTPUpdate_;
        uint pollRetries_;
        const char* ntpServer_;
        const char* ssid_;
        const char* password_;
        IPAddress staticIP_;
        IPAddress gateway_;
        IPAddress subnet_;
        IPAddress dns_;
        uint defaultSyncInterval_;
        uint wifiSyncInterval_;

        void connectToWiFi_();
        void waitForWiFiSync_();
        bool checkWiFiStatus_();
        void onWifiConnectionResolved();
        void syncTime_();
        void disconnectFromWiFi_();
};



#endif //MYNTPCLIENT_H
