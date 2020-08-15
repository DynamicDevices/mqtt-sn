#include <Arduino.h>
#include <SimpleTimer.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Includes

#include "client.h"

// Defines
#define SSID "guest"
#define PASSWORD ""

#define UDP_ADDRESS "192.168.0.219"
#define UDP_PORT 10000

#define SUB_TOPIC "my/sub/topic/#"
#define PUB_TOPIC "my/pub/topic/1"

// Statics

const char* clientId = "my_client_id";

SimpleTimer timer;
MqttsnClientHandle client;
bool isConnected = false;
bool isErrored = false;
bool isSubscribed = false;
bool isPublished = false;
WiFiUDP udp;
int timerId;
unsigned long timerStartMs;

// Support Functions

// MQTT-SN send callback

void my_send(void* userData, const unsigned char* buf, unsigned bufLen, bool broadcast)
{
    if (broadcast) {
      printf("Broadcast send %d bytes\n", bufLen);

      int success = udp.beginPacket(IPAddress(255,255,255,255),UDP_PORT);

      printf("Success: %d\n", success);
    }
    else {
      printf("Send packet %d bytes\n", bufLen);

      int success = udp.beginPacket(UDP_ADDRESS,UDP_PORT);

      printf("Success: %d\n", success);

    }
    int tx = udp.write(buf, bufLen);
    printf("Wrote: %d\n", tx);

    int success = udp.endPacket();
    printf("Success: %d\n", success);

    yield();
}

void my_timer_callback() {
  printf("TimerCB\n");
  mqttsn_client_tick(client);
}

void my_timer(void* userData, unsigned ms)
{
    /* Program asynchronous timer to tick after requested number of milliseconds. When expires invoke mqttsn_client_tick() */
    printf("Create Timer timeout in %d ms\n", ms);

    timerId = timer.setTimeout(ms, my_timer_callback);

    timerStartMs = millis();
}

unsigned my_cancel_timer(void* userData)
{
    /* cancel timer */
    printf("Cancel timer\n");

    timer.deleteTimer(timerId);
    
    return (unsigned)(millis() - timerStartMs); /* return number of elapsed milliseconds */
}

void my_message_handler(void* userData, const MqttsnMessageInfo* msgInfo)
{
   /* handle application message */
   printf("Application message");
}
 
// MQTT-SN connect callback complete

void my_connect_complete(void* userData, MqttsnAsyncOpStatus status)
{
    if (status == MqttsnAsyncOpStatus_Successful) {
      printf("Connection OK\n");
      isConnected = true;
    }
    else {
      printf("Connection Error\n");
      isErrored = true;
    }
}

// MQTT-SN disconnect complete

void my_disconnect_complete(void* userData, MqttsnAsyncOpStatus status)
{
  if (status == MqttsnAsyncOpStatus_Successful) {
    printf("Disconnected OK\n");
    isConnected = false;
  }
  else {
    printf("Disconnection Error\n");
    isErrored = true;
  }
}

// MQTT-SN subscribe complete

void my_subscribe_complete(void* userData, MqttsnAsyncOpStatus status, MqttsnQoS qos)
{
  if (status == MqttsnAsyncOpStatus_Successful) {
    printf("Subscribe OK\n");
    isSubscribed = true;
  }
  else {
    printf("Subscribe Error\n");
    isErrored = true;
  }
}
 
// MQTT-SN publish complete

void my_publish_complete(void* userData, MqttsnAsyncOpStatus status)
{
  if (status == MqttsnAsyncOpStatus_Successful) {
    printf("Publish OK\n");
    isPublished = true;
  }
  else {
    printf("Publish Error\n");
    isErrored = true;
  }
}
 
// Main functions

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setDebugOutput(true);
}

uint8_t rxBuffer[128];

void handleUDPRxComms() {
    int packetSize = udp.parsePacket();
    if(packetSize) {
      printf("Rx Data\n");
      int rxd = udp.read(rxBuffer, sizeof(rxBuffer));
      unsigned consumed = mqttsn_client_process_data(client, rxBuffer, rxd);
      if (consumed < rxd) {
        printf("Error: Consumed size wrong\n");
      }
      udp.flush();
    }
}

void loop() {

  printf("- Create new client\n");

  client = mqttsn_client_new();

  // Set callbacks
  printf("- Set callbacks\n");
  mqttsn_client_set_send_output_data_callback(client, &my_send, NULL);
  mqttsn_client_set_next_tick_program_callback(client, &my_timer, NULL);
  mqttsn_client_set_cancel_next_tick_wait_callback(client, &my_cancel_timer, NULL);
  mqttsn_client_set_message_report_callback(client, &my_message_handler, NULL);

  mqttsn_client_set_retry_period(client, 5); // retry after 5 seconds
  mqttsn_client_set_retry_count(client, 4); // perform 4 retry attempts

  /* Disable sending of SEARCHGW messages to discover the gateway */
  mqttsn_client_set_searchgw_enabled(client, false); 

  printf("- Start the client\n");
  MqttsnErrorCode result = mqttsn_client_start(client);
  if (result != MqttsnErrorCode_Success) {
    /* start has failed, probably some mandatory callback hasn't been set */
    printf("*** Error starting client\n");
  }

  // Connect to AP
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    printf("Connecting to WiFi..\n");
  }
  printf("Connected to WiFi as %s\n", WiFi.localIP().toString().c_str());

  // Listen on UDP server port
  udp.begin(UDP_PORT);
  
  // Connect
  printf("- Connect client\n");
  result = mqttsn_client_connect(client, clientId, 60, true, NULL, &my_connect_complete, NULL);

  while(!isConnected && !isErrored)
  {
    handleUDPRxComms();
    timer.run();
  }
  
  // Subscribe to something
 
  mqttsn_client_subscribe(
    client, 
    SUB_TOPIC,
    MqttsnQoS_ExactlyOnceDelivery, /* max QoS */
    &my_subscribe_complete,
    NULL);

  while(!isSubscribed && !isErrored)
  {
    handleUDPRxComms();
    timer.run();
  }
 
  const char pubData[] = "Hello MQTT-SN World";
  int count = 10;

  while(count-- > 0) {
    printf("- Publish data\n");
  
    // Publish something
    mqttsn_client_publish(
      client, 
      (const char *)PUB_TOPIC,
      (const unsigned char *)pubData,
      strlen(pubData),
      MqttsnQoS_AtLeastOnceDelivery,
      false,
      &my_publish_complete,
      NULL);

    while(!isPublished && !isErrored)
    {
      handleUDPRxComms();
      timer.run();
    }
  
    sleep(10);
  }

  // Disconnect
  printf("- Disconnect client\n");
  result = mqttsn_client_disconnect(client, &my_disconnect_complete, NULL);

  while(isConnected && !isErrored) {
    handleUDPRxComms();
    timer.run();
  }

  // All Done
  printf("- Free client\n");
  mqttsn_client_free(client);

  while(1)
      timer.run();

}