// The example uses an arduno sdk for DCD hub to connect and update
// 3-dimensional accelerometer values on thing's property.
//
// In this example, We are using Adafruit_BNO055 driver to derive X, Y, Z co-ordinates within the sensor.
// https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/overview
//
// Once you uploaded the code, You can check live update on your dcd hub dashboard after a successful
// connection here: https://dwd.tudelft.nl/subject.
//
// by Nirav Malsattar <n.malsattar@tudelft.nl>
// https://github.com/datacentricdesign/dcd-sdk-arduino

#include <Wire.h>
#include <Adafruit_Sensor.h> //Adaruit Sensor Library (must to include before BNO055)
#include <Adafruit_BNO055.h> //Adafruit BNO055 Library
#include <utility/imumaths.h> // Algorithm to calculates exact orientation data

#include "arduino_secrets.h" //Include the credentials
#include <dcd_hub_arduino.h> //Include DCD SDK

dcd_hub_arduino dcdHub; //creates a class object from dcd sdk
Adafruit_BNO055 bno = Adafruit_BNO055(55); //Creates class object from BNO Sensor library

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

  /* Initialise the BNO055 sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 Sensor detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);

}

void loop() {

  // call keep_alive_mqtt() regularly to avoids being disconnected by the MQTT broker
  dcdHub.keep_alive_mqtt();

  /* Get a new BNO055 sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  /* Display the floating point data derived from BNO sensor*/
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");

  float orientationValue[] = {event.orientation.x, event.orientation.y, event.orientation.z};

  delay(100);
  
  //call to an update_property object to update property value as an array according to it's "proeprty_id"
  //{class_object}.update property (property_id, value[], dimension)

  dcdHub.update_property("my-random-property2-563b", orientationValue, 3);
}
