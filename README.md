
# Overview

MQTT-SN or MQTT for Sensor Networks is a lightweight assured message delivery protocol similar to MQTT

A major difference is that MQTT-SN does not rely upon TCP for message transmission, instead leveraging UDP or any other connectionless message passing paradigm.

This means that MQTT-SN can be used for messaging in very light-weight IoT sensors which may not be able to support a full TCP/IP stack.

# This project

Platform.io is an excellent development environment for IoT devices, proving support for a wide range of hardware with easily and automatically installable libraries.

There is an existing library for MQTT communications called [PubSubClient](https://platformio.org/lib/show/89/PubSubClient) released by Nick O'Leary (@knolleary).

However there has not been an equivalent MQTT-SN library available to developers to get them going easily.

This project aims to package up MQTT-SN support in an easily accessible manner

Currently I am focussed on ESP32 and ESP8266 so this library will be released for and verified against those target platforms.

# Existing MQTT-SN implementations

Rather than start from scratch I have chosed to make use of @arobenko's MQTT-SN implementation which can be found [here](https://github.com/arobenko/mqtt-sn).

The upstream code is currently licensed under GPLv3 although this will be changing to MPL-2.0 see [here](https://github.com/DynamicDevices/mqtt-sn/issues/1).

As such the intention is that this library packaging the upstream code will follow the upstream license.

# State of Play and Roadmap

- The library currently includes a working example of MQTT-SN message publishing tested on the ESP32
- The library is a "cut and paste" of the output of the Alex Robenko's MQTT-SN repo build process. In future this library needs to better integrate with the upstream build system.
- The library now uses Nick O'Leary's PubSubClient API so that developers can easily switch between MQTT and MQTT-SN
- NOTE: ** Only publication is currently implemented ***

# Maintainer / Feedback

Alex J Lennon

For questions or comments please raise an issue [here](https://github.com/DynamicDevices/mqtt-sn/issues)
