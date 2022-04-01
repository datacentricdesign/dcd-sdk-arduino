// This example demonstrates the use of the the "DCD SDK for Arduino" library
// by connecting to a "thing" and updating an individual property.
// You can check live updates on your DCD Hub dashboard after successfully
// establishing a connection here: https://dwd.tudelft.nl/bucket/.
//
// TESTED & SUPPORTED BOARDS:
// Arduino Nano 33, ItsyBitsy M4 Express, ESP32 DevKit v1.
//
// Created by
// Nirav Malsattar <n.malsattar@tudelft.nl>
// Adriaan Bernstein <a.j.bernstein@tudelft.nl>
// Last modified on 15 June 2021


#include "arduino_secrets.h"
#include <dcd_hub_arduino.h>

dcd_hub_arduino dcdHub; // Creates a class object from library

// !!IMPORTANT!!
// If using an external Wifi module (e.g. StudioLab BitsyExpander, Adafruit AirLift)
// instead of on-board Wifi (e.g. Arduino Nano 33 IoT), the Wifi pins need to be
// configured manually using Adafruit's WifiNINA library.
//
// For instructions refer to the "Install Library" paragraph here:
// https://learn.adafruit.com/adafruit-airlift-bitsy-add-on-esp32-wifi-co-processor/arduino

// Pin configuration for the Wifi radio on the StudioLab BitsyExpander board.
// If using an Adafruit AirLift board, adjust these accordingly.
#define SPIWIFI      SPI  // The SPI port
#define SPIWIFI_CS   9    // Chip select pin
#define ESP32_RST    12   // Reset pin
#define SPIWIFI_BSY  11   // BUSY or READY pin
#define ESP32_GPIO0  -1

#if !defined(ARDUINO_SAMD_NANO_33_IOT) && !defined(ARDUINO_SAMD_MKRWIFI1010) && !defined(ARDUINO_AVR_UNO_WIFI_REV2) &&!defined(ESP32)
 #define EXTERNAL_WIFI
#endif

void setup() {


#if defined(EXTERNAL_WIFI)
  // Set up Wifi pins for externail Wifi radio.
  WiFi.setPins(SPIWIFI_CS, SPIWIFI_BSY, ESP32_RST, ESP32_GPIO0, &SPIWIFI);
#endif

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
