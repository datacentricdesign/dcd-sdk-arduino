// The example uses an arduno sdk for DCD hub to connect and update
// Force Sensitive Register (FSR) values on thing's property.
//
// In this example, We are using Adafruit Force Sensitive Register (FSR) to measure the presure on the FSR pad.
// https://learn.adafruit.com/force-sensitive-resistor-fsr
//
// Once you uploaded the code, You can check live update on your dcd hub dashboard after a successful
// connection here: https://dwd.tudelft.nl/subject.
//
// by Nirav Malsattar <n.malsattar@tudelft.nl>
// https://github.com/datacentricdesign/dcd-sdk-arduino
//
// Pin Configuration
// FSR Pin (2nd) - Arduino 3.5v / 5v
// FSR Pin (Any) - Arduino nano A0 with the 3.3KOhm Register connected to the ground (Check the schematic from the link below)
//
// Schematic: https://datacentricdesign.org/docs/2019/04/30/sensors-force

 
#define PRESSURE_PIN  A0

#include "arduino_secrets.h"
#include <dcd_hub_arduino.h>

dcd_hub_arduino dcdHub; //creates a class object from library

int value, prev_value = - 10000;     // int values (read from analog port, both the current and the previous)
int deviation = 0;              // setting the minimum deviation between the measurements (0 by default)
                                // up to 512 (although that is pretty useless)
double voltage_value, newton_value;           // Converted to Voltage 

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

  pinMode(PRESSURE_PIN, INPUT); // setting pinmode to read analog value 

  deviation = 10;  // since there's a bit of a drift in the values if you put the same pressure over a certain period
                   // we ignore a divergence of around 1 percent around the previous value. 
}

// This is based on a 3.3kΩ resistor being used, with weights on the center that do not linger more than 10 seconds
// For situations that deviate from this, accuracy can be lost.
double convert_to_newtons( double voltage) 
{
  /* General fitting model Exp2:
     f(x) = a*exp(b*x) + c*exp(d*x)
     Coefficients (with 95% confidence bounds):
       a =     0.01419  (0.01163, 0.01676)
       b =      0.9523  (0.8922, 1.012)
       c =    -0.01461  (-0.02317, -0.006043)
       d =      -2.231  (-6.605, 2.142)
       Goodness of fit:
       SSE: 7.906e-06
       R-square: 0.9999
       Adjusted R-square: 0.9997
       RMSE: 0.001988
   */
   double a = 0.01419 ;
   double b = 0.9523;
   double c = -0.01461;
   double d = -2.231;
    
  return( (a*exp(b*voltage) + c*exp(d* voltage)) * 9.81 ); // the result of the fit is in KgF to convert to newton we simply
                                                      // multiply by 9.81, if you want data in KgF, remove the final multiplication!
}


void loop() {

  // call keep_alive_mqtt() regularly to avoids being disconnected by the MQTT broker
  dcdHub.keep_alive_mqtt();

  delay(100);                   // Here we can set the sampling rate, right now 10 Hz
  value = analogRead(PRESSURE_PIN); // reading our analog voltage, careful we only have 10 bit resolution so each
                                    // measurement step is only 5V ÷ 1024, so our result will be 0 - 1023

  // if value is within the range of [ previous - σ , previous + σ], ignore it (if value is relatively the same)
  // this will help with having data ocuppy your buffer that is not a significant deviation.
  if( value >= (prev_value - deviation) && value <= (prev_value + deviation) )
    return;
   
  voltage_value = double((value*5)) / 1023; // converting to voltage [ 0, 5] v.
  newton_value = convert_to_newtons(voltage_value); // getting actual force value (careful using this, accuracy may not be ideal) 
                                                    // sensitivity after 1Kgf and before 0.06kgf is limited, you can lower the deviation
                                                    // for some improvements
  Serial.print("Pressure: ");
  Serial.print(value);
  Serial.print(" (0 - 1023) steps,  ");
  Serial.print(voltage_value);
  Serial.print(" (v),  ");
  Serial.print(newton_value);
  Serial.println(" N.");

  prev_value = value;
  
  // Some random 1D, 2D, 3D values to upload on the hub. Later, you can replace these with your sensors value.  
  float forceValue[] = {newton_value};

  //call to an update_property object to update property value as an array according to it's "proeprty_id"
  //{class_object}.update property (property_id, value[], dimension)
  
  dcdHub.update_property("random-02d3",forceValue, 1);
}
