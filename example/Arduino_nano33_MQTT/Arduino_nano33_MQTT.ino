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
  
  int value = random(5);

  dcdHub.update_property("random-02d3",value);
}
