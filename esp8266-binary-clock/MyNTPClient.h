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


    protected:
        void onTick();
        void onWifiConnectTick();
        void initTZ();

    private:
        WiFiUDP wifiUdp_;
        bool isSyncing_;
        bool readyForNTPUpdate_;
        const char* ntpServer_;
        const char* ssid_;
        const char* password_;

        void connectToWiFi_();
        void waitForWiFiSync_();
        void disconnectFromWiFi_();
        void syncTime_();
};



#endif //MYNTPCLIENT_H
