// The example uses an arduno sdk for DCD hub to connect and update
// individual thing's property.
// You can check live update on your dcd hub dashboard after a successful
// connection here: https://dwd.tudelft.nl/subject.
//
// by Nirav Malsattar <n.malsattar@tudelft.nl>
// https://github.com/datacentricdesign/dcd-sdk-arduino

#include "arduino_secrets.h"
#include <dcd_hub_arduino.h>

dcd_hub_arduino dcdHub; //creates a class object from library

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // Connects to dcd hub using your secret credential from "arduino_secrets.h"
  //{class_object}.connect(SECRET_SSID, SECRET_PASS, THING_ID, THING_TOKEN)
  
  // Make sure you have stored your credentials on "arduino_secrets.h" before running this command
  
  dcdHub.connect(SECRET_SSID, SECRET_PASS, THING_ID, THING_TOKEN, "Arduino Nano 33 IoT");
  Serial.println();
}

void loop() {
  
  // call keep_alive_mqtt() regularly to avoids being disconnected by the MQTT broker
  dcdHub.keep_alive_mqtt();

  // Some random 1D, 2D, 3D values to upload on the hub. Later, you can replace these with your sensors value.  
  float value[] = {random(5)};
  float value2[] = {random(80), random(25)};
  float value3[] = {random(80), random(25), random(60)};

  //call to an update_property object to update property value as an array according to it's "proeprty_id"
  //{class_object}.update property (property_id, value[], dimension)
  
  dcdHub.update_property("random-02d3",value, 1);
  dcdHub.update_property("my-random-property3-e0cf",value2, 2);
  dcdHub.update_property("my-random-property2-563b",value3, 3);
}
