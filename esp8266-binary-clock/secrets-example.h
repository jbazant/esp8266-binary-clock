#ifndef SECRETS_H
#define SECRETS_H

#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"

#define STATIC_IP   192, 168, 1, 100
#define GATEWAY_IP  192, 168, 1,   1
#define SUBNET_MASK 255, 255, 255, 0
#define DNS_IP        8,   8,  8,  8

// MQTT
#define MQTT_HOST          "your-mqtt-broker-host"
#define MQTT_PORT          1883
#define MQTT_USER          "your-mqtt-user"
#define MQTT_PASSWORD      "your-mqtt-password"
#define MQTT_TOPIC_PREFIX  "home/binary-clock"

#endif //SECRETS_H
