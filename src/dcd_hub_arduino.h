/*

  This library will allow you to connect Data-Centeric Design hub through mqtt server and update property values.

*/

#pragma ONCE

#include "Arduino.h"
#include "dcd_thing.h"
#include <ArduinoMqttClient.h>

#ifdef ESP32
#include <Client.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#else
#include <WiFiNINA.h>
#endif

class dcd_hub_arduino
{

  public:

    dcd_hub_arduino();
    ~dcd_hub_arduino();

    void connect(String ssid, String pass, String thing_id, String thing_token, String client_id);
    void update_property (String property_id, float values[], int value_type);
    void keep_alive_mqtt ();

  private:

    #ifdef ESP32
    WiFiClientSecure wifiClient;
    #else
    WiFiSSLClient wifiClient;
    int wifiKeyIndex = 0; //This is needed for WEP only. Is support for this needed?
    #endif

   	MqttClient  * mqttClientPtr = NULL;
    int status = WL_IDLE_STATUS;
    void wifiConnect(String ssid, String pass);
    void mqttConnect(String client_id);
    void find_property();
    void create_property();

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
