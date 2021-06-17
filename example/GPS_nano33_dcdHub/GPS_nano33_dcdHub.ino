// The example uses an arduno sdk for DCD hub to connect and update
// GPS values (Two Dimensional/ Longitude & Latitude ) thing's property.
//
// In this example, We are using Adafruit Ultimate GPS Breakout v3 for getting actual position from the sensor using Using I2C
// https://www.adafruit.com/product/746
//
// Once you uploaded the code, You can check live update on your dcd hub dashboard after a successful
// connection here: https://dwd.tudelft.nl/subject.
//
// by Nirav Malsattar <n.malsattar@tudelft.nl>
// https://github.com/datacentricdesign/dcd-sdk-arduino
//
// Pin Configuration:
// Arduino nono33 RX (pin RX0) -> GPS TX
// Arduino nano33 TX (pin TX1) -> GPS RX
// Arduino nano33 VTM -> GPS vIn
// Arduino nano33 GND -> GPS GND

#include <Adafruit_GPS.h> //Include Adafruit GPS library

#include "arduino_secrets.h" //Include the credentials
#include <dcd_hub_arduino.h> //Include DCD SDK

dcd_hub_arduino dcdHub; //creates a class object from library
Adafruit_GPS GPS(&Wire); // Connect to the GPS on the hardware I2C port

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint32_t timer = millis();

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

  GPS.begin(0x10);  // The I2C address to use is 0x10
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //TurnON RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA); // Request updates on antenna status, comment out to keep quiet

  delay(1000);
}

void parseGPS() {
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    if (GPS.hour < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);

      float gpsCoordinates[2] = {GPS.lat, GPS.lon};
      
      //call to an update_property object to update property value as an array according to it's "proeprty_id"
      //{class_object}.update property (property_id, value[], dimension)

      dcdHub.update_property("my-random-property3-e0cf", gpsCoordinates, 2);
    }
    Serial.println("NO GPS Fix. Make sure you have GPS with the square ceramic antenna pointing up with a clear sky view");
  }
}

void loop() {

  // call keep_alive_mqtt() regularly to avoids being disconnected by the MQTT broker
  dcdHub.keep_alive_mqtt();

  parseGPS();

  delay(500);
}
