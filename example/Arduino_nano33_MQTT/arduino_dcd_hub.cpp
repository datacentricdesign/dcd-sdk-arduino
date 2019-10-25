#include "arduino_dcd_hub.h"



arduino_dcd_hub::~arduino_dcd_hub() {
  delete(mqttClientPtr);
}

arduino_dcd_hub::arduino_dcd_hub() {
  mqttClientPtr = new MqttClient(wifiClient);
}

void arduino_dcd_hub::connect(String ssid, String pass, String thing_id, String thing_token, String client_id)
{

  _thing_id = thing_id;

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);

  while (WiFi.begin((ssid).c_str(), (pass).c_str()) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(3000);
  }

  Serial.println("You're connected to the network");

  mqttClientPtr->setId(client_id);

  mqttClientPtr->setUsernamePassword(thing_id, thing_token);

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


void arduino_dcd_hub::update_property (String property_id, int values[]) {

  topic = "/things/" + _thing_id + "/properties/" + property_id; // topic to publish data on MQTT for DCD hub
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    Serial.print("Sending message to topic: ");
    Serial.println(topic);

    String json = "{\"id\":\"";
    json.concat(property_id);
    json.concat("\",\"values\":[[");
    json.concat(values[3]);
    json.concat("]]}");
    Serial.print("pushing..");

    //send message, the Print interface can be used to set the message contents
    mqttClientPtr->beginMessage(topic);
    mqttClientPtr->print(json);
    mqttClientPtr->endMessage();

    Serial.println();
  }
  
}

void arduino_dcd_hub::keep_alive_mqtt () {
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    mqttClientPtr->poll();
  }
}
