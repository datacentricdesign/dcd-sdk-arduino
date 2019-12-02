# DCD SDK Arduino

This libray allows to connect to dcd hub and update value of thing property provided by property_id.

Currently this library uses MQTT interface to connect to the DCD hub and update particular thing property. In future release there will be more functionality added to create a new property or search for exising one.

You can find simple example of updating property values from the example folder provided with the library.

# Required Libraries
Because the DCD Arduino SDK uses on other libraries as dependencies, You need to install those libraries first. 

The libraries you need to installs are:

1. WiFiNINA: enables WiFi enabled Arduino to connect to the internet.
2. ArduinoMqttClient: This library allows you to connect to any MQTT server and send a message as the topic.

# Changelog

The initial beta version 0.0.1 of the dcd-sdk-arduino library to update property value on the DCD hub.

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]



### Added
