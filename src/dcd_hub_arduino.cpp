/*
 *  This library will allow you to connect Data-Centeric Design hub through mqtt server and update property values.
 *
 * Created by
 * Nirav Malsattar <n.malsattar@tudelft.nl>
 * Adriaan Bernstein <a.j.bernstein@tudelft.nl>
 * Last modified on 15 June 2021
 */

#include "dcd_hub_arduino.h"
#include "jwt.h"

dcd_hub_arduino::~dcd_hub_arduino() {
  delete(mqttClientPtr);
}

dcd_hub_arduino::dcd_hub_arduino() {
  #ifdef ESP32
  wifiClient.setInsecure();
  #endif
  mqttClientPtr = new MqttClient(wifiClient);
}

void dcd_hub_arduino::setup(const char *ssid, const char *password, const char *thing_id, const char *project_id, const char *private_key)  {
  _ssid = ssid;
  _password = password;
  _project_id = project_id;
  _thing_id = thing_id;

  setPrivateKey(private_key);

  wifiConnect();  // Connect to wifi
  validateJwt();
  mqttConnect();  // Connect to mqtt broker

}

void dcd_hub_arduino::setup(const char *ssid, const char *password, const char *thing_id, const char *project_id, const char *private_key, const int jwt_lifetime) {
  _ssid = ssid;
  _password = password;
  _project_id = project_id;
  _thing_id = thing_id;
  _jwt_lifetime = jwt_lifetime;

  setPrivateKey(private_key);

  wifiConnect();  // Connect to wifi
  validateJwt();
  mqttConnect();  // Connect to mqtt broker
}

void dcd_hub_arduino::setPrivateKey(const char *priv_key) {
  if ( strlen(priv_key) != (95) ) {
    Serial.println("Warning: expected private key to be 95, was: " +
        String(strlen(priv_key)));
  }
  _private_key[8] = 0;
  for (int i = 7; i >= 0; i--) {
    _private_key[i] = 0;
    for (int byte_num = 0; byte_num < 4; byte_num++) {
      _private_key[i] = (_private_key[i] << 8) + strtoul(priv_key, NULL, 16);
      priv_key += 3;
    }
  }
}

void dcd_hub_arduino::wifiConnect() {
  Serial.print("\nAttempting to connect to network SSID: ");
  Serial.println(_ssid);

  #ifdef ESP32
    WiFi.mode(WIFI_STA); // Defines station mode: ESP32 connects to access point
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(200);
    }
  #else
    WiFi.begin(_ssid, wifiKeyIndex, _password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(200);
    }
  #endif

  Serial.println("\nYou're connected to the network.");
  Serial.print("Waiting on time sync...");

  #ifdef ESP32
    configTime(0, 0, ntp_primary, ntp_secondary);
    while (time(nullptr) < 1510644967) {
      delay(10);
    }
  #else
    while (WiFi.getTime() < 1510644967) {
      delay(10);
    }
  #endif

  Serial.println(" Success.");
}

void dcd_hub_arduino::mqttConnect() {
  mqttClientPtr->setId(_project_id);

  mqttClientPtr->setUsernamePassword(_thing_id, jwt);

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClientPtr->connect(broker, port))
  {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClientPtr->connectError());

    while (1)
    ;
  }

  Serial.println("You're connected to the DCD hub MQTT broker!");
}

bool dcd_hub_arduino::validateJwt() {
#if defined(ESP32)
  tai = time(nullptr);
#else
  tai = WiFi.getTime();
#endif

if (jwt == NULL) {
  Serial.println("Generating JWT...");
  jwt_exp_time = tai + _jwt_lifetime;
  jwt = CreateJwt(_project_id, audience, _thing_id, tai, _private_key, _jwt_lifetime);
}
else if (tai > jwt_exp_time) {
  Serial.print(tai - jwt_exp_time);
  Serial.println(" seconds since token expiry.\nGenerating new JWT...");
  jwt_exp_time = tai + _jwt_lifetime;
  jwt = CreateJwt(_project_id, audience, _thing_id, tai, _private_key, _jwt_lifetime);
}
else if (jwt_exp_time - tai > 120) {
  return false;
}
else {
  Serial.print((jwt_exp_time-tai));
  Serial.println(" seconds to token expiry!\nGenerating new JWT...");
  jwt_exp_time = tai + _jwt_lifetime;
  jwt = CreateJwt(_project_id, audience, _thing_id, tai, _private_key, _jwt_lifetime);
}

  Serial.println(jwt);
  return true;
}

void dcd_hub_arduino::update_property (String property_id, float values[], int value_type) {
  if(validateJwt()) mqttConnect();

  topic = "/things/";
  topic.concat(_thing_id);
  topic.concat("/properties/");
  topic.concat(property_id); // Topic to publish data on MQTT for DCD hub

  if (WiFi.status() == WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 500) {
      // Save the last time a message was sent
      previousMillis = currentMillis;
      Serial.println();
      Serial.print("Sending message to topic: ");
      Serial.println(topic);
      json = "{\"requestId\":";
      json.concat(random(100));
      json.concat(",\"property\":{\"id\":\"");
      json.concat(property_id);
      json.concat("\",\"values\":[[");

      for (int i = 0; i < value_type; ++i) {
        json.concat(values[i]);
        if (i < value_type - 1) {
          json.concat(",");
        }
      }
      json.concat("]]}}");
      Serial.print("pushing..");

      // Send message, the Print interface can be used to set the message contents
      mqttClientPtr->beginMessage(topic);
      mqttClientPtr->print(json);
      mqttClientPtr->endMessage();
      Serial.println(json);
    }
  }

  else {
    Serial.println ("No internet connection. Trying to reconnect...");
    wifiConnect();    // Connect to wifi
    mqttConnect();    // Connect to mqtt broker
  }

  delay(50);
}

void dcd_hub_arduino::keep_alive_mqtt () {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a message was sent
    previousMillis = currentMillis;
    mqttClientPtr->poll();
  }
}

void dcd_hub_arduino::setJwt(String token, unsigned long exp_time) {
  jwt = token;
  jwt_exp_time = exp_time;
}

String dcd_hub_arduino::getJwt() {
  return jwt;
}

int dcd_hub_arduino::getJwtExpTime() {
  return jwt_exp_time;
}
