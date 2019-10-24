#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "arduino_secrets.h"
#include "arduino_dcd_hub.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiSSLClient wifiClient;
MqttClient mqttClient(wifiClient);

arduino_dcd_hub dcdHub (THING_ID, THING_TOKEN, "Arduino Nano 33 IoT");

const long interval = 1000;
unsigned long previousMillis = 0;
int values[] = {0, 0, 0};
int count = 0;

//void messageReceived(int messageSize) {
//  // we received a message, print out the topic and contents
//  Serial.print("Received a message with topic '");
//  Serial.print(mqttClient.messageTopic());
//  Serial.print("', duplicate = ");
//  Serial.print(mqttClient.messageDup() ? "true" : "false");
//  Serial.print(", QoS = ");
//  Serial.print(mqttClient.messageQoS());
//  Serial.print(", retained = ");
//  Serial.print(mqttClient.messageRetain() ? "true" : "false");
//  Serial.print("', length ");
//  Serial.print(messageSize);
//  Serial.println(" bytes:");
//
//  // use the Stream interface to print the contents
//  while (mqttClient.available()) {
//    Serial.print((char)mqttClient.read());
//  }
//  Serial.println();
//
//  Serial.println();
//}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(3000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  dcdHub.connect();
  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  //  mqttClient.poll();
  //
  //  unsigned long currentMillis = millis();
  //
  //  if (currentMillis - previousMillis >= interval) {
  //    // save the last time a message was sent
  //    previousMillis = currentMillis;
  //
  //    Serial.print("Sending message to topic: ");
  //    Serial.println(topic);
  //
  //    int value = random(5);
  //
  //    String json = "{\"id\":\"random-02d3\",\"values\":[[";
  //    json.concat(value);
  //    json.concat("]]}");
  //    Serial.print("pushing...");
  //
  //    //send message, the Print interface can be used to set the message contents
  //    mqttClient.beginMessage(topic);
  //    mqttClient.print(json);
  //    mqttClient.endMessage();
  //
  //    Serial.println();
  //
  //    count++;
  //  }
}
