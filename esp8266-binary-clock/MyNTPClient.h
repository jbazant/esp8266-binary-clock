//
// Created by Jiří Bažant on 29.08.2024.
//

#ifndef MYNTPCLIENT_H
#define MYNTPCLIENT_H

#include <ESP8266WiFi.h>
#include "WiFiUdp.h"
#include "NTP.h"
#include "WithTicker.h"


class MyNTPClient : public NTP, public WithTicker {
    public:
        MyNTPClient(const char* ntpServer, const char* ssid, const char* password, const uint interval);
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
        int pollRetries_;
        const char* ntpServer_;
        const char* ssid_;
        const char* password_;

        void connectToWiFi_();
        void waitForWiFiSync_();
        bool checkWiFiStatus_();
        void onWifiConnectionResolved();
        void syncTime_();
        void disconnectFromWiFi_();
};



#endif //MYNTPCLIENT_H
