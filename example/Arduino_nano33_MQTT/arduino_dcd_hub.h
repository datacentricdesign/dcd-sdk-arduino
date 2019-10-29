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
    void update_property (String property_id, float values[], int value_type);
    void keep_alive_mqtt ();
    

  private:
    WiFiSSLClient wifiClient;
   	MqttClient  * mqttClientPtr = NULL;
    int wifiKeyIndex = 0;
    int status = WL_IDLE_STATUS; 
    void wifiConnect(String ssid, String pass);
    void mqttConnect(String client_id);
    
    const String broker = "dwd.tudelft.nl";   //mqtt host
    const int port = 8883;                    //mqtt port

    String _ssid;
    String _pass;
    String _thing_id;
    String _thing_token;
    String _client_id;
    String _property_id;
    String topic= "";
    String json= "";

    unsigned long previousMillis = 0;
    const long interval = 1000;
};
