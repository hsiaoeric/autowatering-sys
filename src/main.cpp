/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/
*********/

// Import required libraries
#include <Arduino.h>
#include <UniversalTelegramBot.h>
#include "wifi_MQTT_stuff.h"

#define soilPIN A0    // Digital pin connected to the DHT sensor
#define soilPowerPIN 10

#define MQTT_HOST IPAddress(192, 168, 68, 165)
#define MQTT_PORT 1883
#define MQTT_PUB_SOIL "esp8266/soilW"

#define BOTtoken "7250313678:AAHaIIbkEH5nUDcRABepckuwrLcMxI8Xr3w"  // your Bot Token (Get from Botfather)

UniversalTelegramBot bot(BOTtoken, client);




#define wateringPIN 0
void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);
  
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  //mqttClient.onSubscribe(onMqttSubscribe);
  //mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  // If your broker requires authentication (username and password), set them below
  //mqttClient.setCredentials("REPlACE_WITH_YOUR_USER", "REPLACE_WITH_YOUR_PASSWORD");
  
  connectToWifi();
  pinMode(wateringPIN, OUTPUT);
  pinMode(soilPowerPIN, OUTPUT);

}

float convert(int& val){
  return (float) val/505*100;

}
void loop(){  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    // Read temperature as Celsius (the default)
    int sum = 0;
    digitalWrite(soilPowerPIN, HIGH);
    int newL = analogRead(soilPIN);
    for (int i = 0; i < 32; i++)
      sum += newL;
    sum >>= 5;
    digitalWrite(soilPowerPIN, HIGH);
    float soilP = convert(sum);
    if(soilP <= 60){
      digitalWrite(wateringPIN, HIGH);
    }else digitalWrite(wateringPIN, LOW);
    Serial.println(soilP);
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_SOIL, 1, true, String(soilP).c_str());                            
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_SOIL, packetIdPub1);
    Serial.printf("Message: %.1f \n", soilP);
    
    // if (soilP < 30)
    //   digitalWrite()
    // }
  }
}
