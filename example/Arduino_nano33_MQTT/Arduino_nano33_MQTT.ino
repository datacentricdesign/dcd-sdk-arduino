#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "arduino_secrets.h"
#include "arduino_dcd_hub.h"

arduino_dcd_hub dcdHub;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  dcdHub.connect(SECRET_SSID, SECRET_PASS, THING_ID, THING_TOKEN, "Arduino Nano 33 IoT");
  Serial.println();
}

void loop() {
  // call keep_alive_mqtt() regularly to send MQTT keep alives every one seconds which
  // avoids being disconnected by the broker
  dcdHub.keep_alive_mqtt();
  
  float value[] = {random(5)};
  float value2[] = {random(80), random(25)};
  float value3[] = {random(80), random(25), random(60)};

  dcdHub.update_property("random-02d3",value, 1);
  dcdHub.update_property("random-02d3",value2, 2);
  dcdHub.update_property("random-3d-499b",value3, 3);
}
