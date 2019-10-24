#include "arduino_dcd_hub.h"



arduino_dcd_hub::~arduino_dcd_hub(){
 delete(mqttClientPtr);
}

arduino_dcd_hub::arduino_dcd_hub(String thing_id, String thing_token, String client_id) : _thing_id(thing_id), _thing_token (thing_token), _client_id(client_id){

   mqttClientPtr = new MqttClient(wifiClient);
  
   topic = "/things/" + _thing_id + "/properties/" + _property_id; // topic to publish data on MQTT for DCD hub
}

void arduino_dcd_hub::connect()
{   
    
    mqttClientPtr->setId(_client_id);

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
