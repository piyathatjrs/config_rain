#include "DHT.h"
#include <WiFi.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include<PubSubClient.h>



#define WIFI_SSD "กรอก SSID"
#define WIFI_PASS "กรอก password"

#define MQTT_SERVER   "tailor.cloudmqtt.com"
#define MQTT_PORT       14090
#define MQTT_USERNAME "zpfdcyrx"
#define MQTT_PASSWORD "OypjtCmtYhqp"
#define MQTT_NAME     "ESSSSS"

#define rainAnalog 35


WiFiClient client;

PubSubClient mqtt(client);

void callback(char* topic, byte* payload, unsigned int length) {
  String topic_str = "";
  String payload_str = "";
  for (int j = 0 ; j < strlen(topic) ; j++) {
    topic_str += topic[j];
  }
  for (int i = 0 ; i < length ; i++) {
    Serial.print((char)payload[i]);
    payload_str += (char)payload[i];
  }

  Serial.print("[ ");
  Serial.print(topic);
  Serial.print(" ]");
  Serial.print(payload_str);

}

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  pinMode(LED_BUILTIN, OUTPUT);


  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSD);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSD, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);

  connectMQTT();

}


void loop() {

int rainAnalogVal = analogRead(rainAnalog);

  int map_value_rain=0;
  
    map_value_rain = map(rainAnalogVal , 4095 , 840 , 0 ,100);
  Serial.println(rainAnalogVal);
  Serial.println(map_value_rain);
  if(map_value_rain >= 100){
    Publish("TEST/MQTT_RAIN", String(map_value_rain));
    }else {
      Publish("TEST/MQTT_RAIN", String(map_value_rain));
      }
  delay(1500);




}

void connectMQTT() {
  Serial.print("MQTT connect... ");
  if (mqtt.connect(MQTT_NAME , MQTT_USERNAME , MQTT_PASSWORD )) {
    Serial.print("success");
  } else {
    Serial.print("Failed");
    delay(5000);
  }
  Serial.println();

}

void Publish(String key , String message) {
  char topic[100] , payload[100];
  key.toCharArray(topic, 100);
  message.toCharArray(payload, 100);
  mqtt.publish(topic , payload);
}
int analog_value = 0;
double temp = 0;
