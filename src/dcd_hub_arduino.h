/*
 *  This library will allow you to connect Data-Centeric Design hub through mqtt server and update property values.
 *
 * Created by
 * Nirav Malsattar <n.malsattar@tudelft.nl>
 * Adriaan Bernstein <a.j.bernstein@tudelft.nl>
 * Last modified on 15 June 2021
 */

#pragma ONCE

#include "Arduino.h"
#include "jwt.h"
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

    void setup(const char *ssid, const char *password, const char *thing_id, const char *project_id, const char *private_key);
    void setup(const char *ssid, const char *password, const char *thing_id, const char *project_id, const char *private_key, const int jwt_lifetime);
    void update_property (String property_id, float values[], int value_type);
    void keep_alive_mqtt ();
    void setJwt(String token, unsigned long exp_time);
    String getJwt();
    int getJwtExpTime();

  private:

    #ifdef ESP32
    WiFiClientSecure wifiClient;
    #else
    WiFiSSLClient wifiClient;
    int wifiKeyIndex = 0; // This is needed for WEP only. Is support for this needed?
    #endif

   	MqttClient *mqttClientPtr = NULL;
    void setPrivateKey(const char *priv_key);
    void wifiConnect();
    void mqttConnect();
    bool validateJwt();
    // void find_property();   //TODO
    // void create_property(); //TODO

    // Configuration for NTP (Required for ESP32)
    const char *ntp_primary = "pool.ntp.org";
    const char *ntp_secondary = "time.nist.gov";

    // DCD Hub
    const char *broker = "dwd-test.tudelft.nl";  // mqtt host
    const char *audience = "https://dwd-test.tudelft.nl:443/bucket/api";
    const int port = 8883;                       // mqtt port

    const char *_project_id;
    const char *_thing_id;
    const char *_ssid;
    const char *_password;

    NN_DIGIT _private_key[9];
    String jwt;

    unsigned long tai = 0;         // Temps atomique international in Unix time
    unsigned long jwt_exp_time;    // TAI of jwt token expiry
    int _jwt_lifetime = 3600 * 24; // 3600 seconds in 1 hour

    String topic;
    String json;

    unsigned long previousMillis = 0;
    unsigned long currentMillis;
    const long interval = 1000;
};
