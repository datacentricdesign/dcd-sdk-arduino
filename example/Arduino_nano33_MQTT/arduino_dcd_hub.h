/*

  Library to connect arduino nano 33 IoT to Data-Centeric Design hub through mqtt server

*/

#pragma ONCE

#include "Arduino.h"
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

class arduino_dcd_hub
{

  public:

    arduino_dcd_hub();
    ~arduino_dcd_hub();
    void connect(String ssid, String pass, String thing_id, String thing_token, String client_id);
    void update_property (String property_id, int values);
    void keep_alive_mqtt ();
    String _thing_id;
    String _property_id;

  private:
    WiFiSSLClient wifiClient;
   	MqttClient  * mqttClientPtr = NULL;

    const String broker = "dwd.tudelft.nl";   //mqtt host
    const int port = 8883;                    //mqtt port
    
    String topic= "";
    String json= "";

    unsigned long previousMillis = 0;
    const long interval = 1000;
};
