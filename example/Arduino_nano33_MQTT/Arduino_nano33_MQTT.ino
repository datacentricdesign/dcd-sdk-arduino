// This example demonstrates the use of the the "DCD SDK for Arduino" library
// by connecting to a "thing" and updating an individual property.
// You can check live updates on your DCD Hub dashboard after successfully
// establishing a connection here: https://dwd.tudelft.nl/bucket/.
//
// Created by
// Nirav Malsattar <n.malsattar@tudelft.nl>
// Adriaan Bernstein <a.j.bernstein@tudelft.nl>
// Last modified on 15 June 2021

#include "arduino_secrets.h"
#include <dcd_hub_arduino.h>

dcd_hub_arduino dcdHub; // Creates a class object from library

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Connects to DCD Hub using your secret credentials stored in "arduino_secrets.h"
  // Make sure you stored your credentials in "arduino_secrets.h" before running this command.
  dcdHub.setup(ssid, password, thing_id, project_id, private_key_str);
  Serial.println();
}

void loop() {

  // Call keep_alive_mqtt() regularly to avoid being disconnected by the MQTT broker
  dcdHub.keep_alive_mqtt();

  // Some random 1D, 2D, 3D values to upload to the hub. Later, you can replace these with your sensors value.
  float value[] = {random(5)};
  float value2[] = {random(80), random(25)};
  float value3[] = {random(80), random(25), random(60)};

  // A function call to update a property should look like this:
  // {class_object}.update_property(property_id, value[], dimension);
  //
  // To update a property, you need to pass three arguments:
  //
  // property_id: The unique ID of the property you want to update.
  // value[]:     The values you want to update the property with as an array.
  // dimension:   The amount of dimensions your property has (how many values per data point).

  dcdHub.update_property("dcd:properties:s0m3-numb3rs-4nd-d1g17s", value, 1);
  // dcdHub.update_property("dcd:properties:my-random-property3-e0cf",value2, 2);
  // dcdHub.update_property("dcd:properties:my-random-property3-e0cf",value3, 3);
}
