#include "dcd_hub_arduino.h"

dcd_hub_arduino::~dcd_hub_arduino() {
  delete(mqttClientPtr);
}

dcd_hub_arduino::dcd_hub_arduino() {
  mqttClientPtr = new MqttClient(wifiClient);
}

void dcd_hub_arduino::wifiConnect(String ssid, String pass) {
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);

  while (status != WL_CONNECTED) {
    status =  WiFi.begin((ssid).c_str(), wifiKeyIndex, (pass).c_str());
    Serial.print(".");
    delay(3000);
  }

  Serial.println("You're connected to the network");
}

void dcd_hub_arduino::mqttConnect(String client_id) {
  mqttClientPtr->setId(client_id);

  mqttClientPtr->setUsernamePassword(_thing_id, _thing_token);

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClientPtr->connect((broker).c_str(), port))
  {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClientPtr->connectError());

    while (1)
      ;
  }

  Serial.println("You're connected to the DCD hub MQTT broker!");
}

void dcd_hub_arduino::connect(String ssid, String pass, String thing_id, String thing_token, String client_id)
{

  _ssid = ssid;
  _pass = pass;
  _client_id = client_id;
  _thing_id = thing_id;
  _thing_token = thing_token;

  wifiConnect(_ssid, _pass); //Connect to wifi
  mqttConnect(_client_id);  // Connect to mqtt broker

}

void dcd_hub_arduino::update_property (String property_id, float values[], int value_type) {

  topic = "/things/" + _thing_id + "/properties/" + property_id; // topic to publish data on MQTT for DCD hub

  if (WiFi.status() == WL_CONNECTED) {

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 500) {
      // save the last time a message was sent
      previousMillis = currentMillis;
      Serial.println();
      Serial.print("Sending message to topic: ");
      Serial.println(topic);
      String json = "{\"id\":\"";
      json.concat(property_id);
      json.concat("\",\"values\":[[");

      for (int i = 0; i < value_type; ++i) {
        json.concat(values[i]);
        if (i < value_type - 1) {
          json.concat(",");
        }
      }

      json.concat("]]}");
      Serial.print("pushing..");

      //send message, the Print interface can be used to set the message contents
      mqttClientPtr->beginMessage(topic);
      mqttClientPtr->print(json);
      mqttClientPtr->endMessage();

      Serial.println(json);
    }
  }

  else {
    Serial.println ("No internet connection....Trying to reconnect");
    wifiConnect(_ssid, _pass); //Connect to wifi
    mqttConnect(_client_id);  // Connect to mqtt broker
  }

  delay(50);
}

void dcd_hub_arduino::keep_alive_mqtt () {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    mqttClientPtr->poll();
  }
}
