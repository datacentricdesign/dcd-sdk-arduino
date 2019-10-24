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

    arduino_dcd_hub(String thing_id, String thing_token, String client_id);
    ~arduino_dcd_hub();
    void connect();

  private:
    WiFiSSLClient wifiClient;
   	MqttClient  * mqttClientPtr = NULL;

    const String broker = "dwd.tudelft.nl";   //mqtt host
    const int port = 8883;                    //mqtt port

    String _thing_id;
    String _thing_token;
    String _client_id;
    String _property_id;
    String topic= "";
};
