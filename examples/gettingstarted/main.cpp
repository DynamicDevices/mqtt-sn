#include <Arduino.h>
#include <WiFi.h>

// Includes

#include "PubSubClientSN.h"

// Defines
#define SSID "guest"
#define PASSWORD ""

#define UDP_ADDRESS "192.168.0.219"
#define UDP_PORT 10000

#define SUB_TOPIC "my/sub/topic/#"
#define PUB_TOPIC "my/pub/topic/1"

// Declarations
void callback(char* topic, byte* payload, unsigned int length);

// Statics

PubSubClient pubSubClient(UDP_ADDRESS, UDP_PORT, callback);

// Support functions

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

// Main functions

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // Connect to AP
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    printf("Connecting to WiFi (%d)\n", WiFi.status());

    if(WiFi.status() == WL_CONNECT_FAILED)
      WiFi.begin(SSID, PASSWORD);
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

// Generate client name based on MAC address and last 8 bits of microsecond counter
  String clientName;
  clientName += "esp8266-";
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  clientName += "-";
  clientName += String(micros() & 0xff, 16);

  Serial.print("Connecting to ");
  Serial.print(UDP_ADDRESS);
  Serial.print(" as ");
  Serial.println(clientName);
  
  IPAddress serverIP;
  serverIP.fromString(UDP_ADDRESS);

  // TODO: setServer(domain, port) not yet supported
  pubSubClient.setServer( serverIP, UDP_PORT);

  if (pubSubClient.connect((char*) clientName.c_str())) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic is: ");
    Serial.println(PUB_TOPIC);
    
    if (pubSubClient.publish(PUB_TOPIC, "hello from ESP8266")) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}

void loop() {
  static int counter = 0;
  
  String payload = "{\"micros\":";
  payload += micros();
  payload += ",\"counter\":";
  payload += counter;
  payload += "}";
  
  if (pubSubClient.connected()){
    Serial.print("Sending payload: ");
    Serial.println(payload);
    
    if (pubSubClient.publish(PUB_TOPIC, (char*) payload.c_str())) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  ++counter;
  delay(5000);
}