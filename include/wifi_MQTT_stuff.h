// #ifndef WIFI_MQTT_STUFF_H
// #define WIFI_MQTT_STUFF_H


#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncMqttClient.h>
#include <Ticker.h>

extern WiFiEventHandler wifiConnectHandler;
extern WiFiEventHandler wifiDisconnectHandler;
extern Ticker wifiReconnectTimer;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
extern unsigned long previousMillis;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
extern const unsigned long interval;  


// Replace with your network credentials
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;

extern WiFiClientSecure client;
extern AsyncMqttClient mqttClient;
extern Ticker mqttReconnectTimer;

void connectToWifi();
void connectToMqtt();
void onWifiConnect(const WiFiEventStationModeGotIP& event);
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttPublish(uint16_t packetId);
// #endif
