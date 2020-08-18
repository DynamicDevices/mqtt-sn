/*

  PubSubClient.cpp - A simple client for MQTT.
  Nick O'Leary
  http://knolleary.net
*/

#include "PubSubClientSN.h"
#include "Arduino.h"

// MQTT-SN support functions    

void mqttsn_send(void* userData, const unsigned char* buf, unsigned bufLen, bool broadcast)
{
    PubSubClient *psc = static_cast<PubSubClient *>(userData);

    if (broadcast) {
      int success = psc->udp.beginPacket(IPAddress(255,255,255,255),psc->port);
    }
    else {
      int success = psc->udp.beginPacket(psc->ip,psc->port);
    }
    int tx = psc->udp.write(buf, bufLen);

    int success = psc->udp.endPacket();
}

void mqttsn_timer_callback(void *userData) {
  PubSubClient *psc = (PubSubClient *)userData;
  mqttsn_client_tick(psc->_snClient);
}

void mqttsn_timer(void* userData, unsigned ms)
{
    PubSubClient *psc = (PubSubClient *)userData;

    /* Program asynchronous timer to tick after requested number of milliseconds. When expires invoke mqttsn_client_tick() */
    psc->timerId = psc->timer.setTimeout(ms, mqttsn_timer_callback, psc);
    psc->timerStartMs = millis();
}

unsigned mqttsn_cancel_timer(void* userData)
{
    PubSubClient *psc = (PubSubClient *)userData;

    /* cancel timer */
    psc->timer.deleteTimer(psc->timerId);    
    return (unsigned)(millis() - psc->timerStartMs); /* return number of elapsed milliseconds */
}

void mqttsn_message_handler(void* userData, const MqttsnMessageInfo* msgInfo)
{
//   PubSubClient *psc = (PubSubClient *)userData;

   /* handle application message */
   printf("Application message");
}
 
// MQTT-SN connect callback complete

void mqttsn_connect_complete(void* userData, MqttsnAsyncOpStatus status)
{
    PubSubClient *psc = (PubSubClient *)userData;

    if (status == MqttsnAsyncOpStatus_Successful) {
      psc->isConnected = true;
    }
    else {
      psc->isErrored = true;
    }
}

// MQTT-SN disconnect complete

void mqttsn_disconnect_complete(void* userData, MqttsnAsyncOpStatus status)
{
    PubSubClient *psc = (PubSubClient *)userData;

  if (status == MqttsnAsyncOpStatus_Successful) {
    psc->isConnected = false;
  }
  else {
    psc->isErrored = true;
  }
}

// MQTT-SN subscribe complete

void mqttsn_subscribe_complete(void* userData, MqttsnAsyncOpStatus status, MqttsnQoS qos)
{
    PubSubClient *psc = (PubSubClient *)userData;

  if (status == MqttsnAsyncOpStatus_Successful) {
    psc->isSubscribed = true;
  }
  else {
    psc->isErrored = true;
  }
}

// MQTT-SN unsubscribe complete

void mqttsn_unsubscribe_complete(void* userData, MqttsnAsyncOpStatus status)
{
    PubSubClient *psc = (PubSubClient *)userData;

  if (status == MqttsnAsyncOpStatus_Successful) {
    psc->isUnsubscribed = true;
  }
  else {
    psc->isErrored = true;
  }
}

// MQTT-SN publish complete

void mqttsn_publish_complete(void* userData, MqttsnAsyncOpStatus status)
{
    PubSubClient *psc = (PubSubClient *)userData;

    if (status == MqttsnAsyncOpStatus_Successful) {
        psc->isPublished = true;
    }
    else {
        psc->isErrored = true;
    }
}

void PubSubClient::handleUDPRxComms() {
    int packetSize = udp.parsePacket();
    if(packetSize) {
      printf("Rx Data\n");
      int rxd = udp.read(rxBuffer, sizeof(rxBuffer));
      unsigned consumed = mqttsn_client_process_data(_snClient, rxBuffer, rxd);
      if (consumed < rxd) {
        printf("Error: Consumed size wrong\n");
      }
      udp.flush();
    }
}

// END MQTT-SN support

PubSubClient::PubSubClient() {
    this->_state = MQTT_DISCONNECTED;
    this->_client = NULL;
    this->stream = NULL;
    setCallback(NULL);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(IPAddress addr, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr, port);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(IPAddress addr, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr,port);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(IPAddress addr, uint16_t port, MQTT_CALLBACK_SIGNATURE) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr, port);
    setCallback(callback);
    this->_client = NULL;
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(IPAddress addr, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr, port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(IPAddress addr, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip, port);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip,port);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip, port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(const char* domain, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(const char* domain, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(const char* domain, uint16_t port, MQTT_CALLBACK_SIGNATURE) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setCallback(callback);
    this->_client = NULL;
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(const char* domain, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::PubSubClient(const char* domain, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);

    _snClient = mqttsn_client_new();

    void *vpThis = static_cast<void*>(this);
    mqttsn_client_set_send_output_data_callback(_snClient, &mqttsn_send, vpThis);
    mqttsn_client_set_next_tick_program_callback(_snClient, &mqttsn_timer, vpThis);
    mqttsn_client_set_cancel_next_tick_wait_callback(_snClient, &mqttsn_cancel_timer, vpThis);
    mqttsn_client_set_message_report_callback(_snClient, &mqttsn_message_handler, vpThis);

    // TODO: Integrate these with PubSubClient properties?
    mqttsn_client_set_retry_period(_snClient, 5); // retry after 5 seconds
    mqttsn_client_set_retry_count(_snClient, 4); // perform 4 retry attempts

    // We don't support searching for gateways
    mqttsn_client_set_searchgw_enabled(_snClient, false); 
}

PubSubClient::~PubSubClient() {
  mqttsn_client_free(_snClient);
  free(this->buffer);
}

boolean PubSubClient::connect(const char *id) {
    return connect(id,NULL,NULL,0,0,0,0,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass) {
    return connect(id,user,pass,0,0,0,0,1);
}

boolean PubSubClient::connect(const char *id, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage) {
    return connect(id,NULL,NULL,willTopic,willQos,willRetain,willMessage,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage) {
    return connect(id,user,pass,willTopic,willQos,willRetain,willMessage,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage, boolean cleanSession) {
    if (!isConnected) {

        udp.begin(port);

        isConnected = false;
        isErrored = false;

        MqttsnErrorCode result = mqttsn_client_start(_snClient);
        if(result != MqttsnErrorCode_Success) {

            _state = MQTT_CONNECT_FAILED;
            udp.stop();
            return false;
        }

        /*
            MqttsnErrorCode mqttsn_client_connect(
            MqttsnClientHandle client,
            const char* clientId,
            unsigned short keepAliveSeconds,
            bool cleanSession,
            const MqttsnWillInfo* willInfo,
            MqttsnAsyncOpCompleteReportFn callback,
            void* data)
            */


        // TODO: LWT NOT SUPPORTED YET
        // TODO: I don't seem to be able to provide a user/password ?
        result = mqttsn_client_connect(_snClient, id, this->keepAlive, cleanSession, NULL, &mqttsn_connect_complete, this);
        if(result != MqttsnErrorCode_Success) {

            _state = MQTT_CONNECT_FAILED;
            udp.stop();
            return false;
        }

        while(!isConnected && !isErrored) {
            handleUDPRxComms();
            timer.run();
            delay(50);
        }

        if(isErrored) {
//            _state = MQTT_CONNECTION_TIMEOUT;
            _state = MQTT_CONNECT_FAILED;
            udp.stop();
            return false;
        }

        return true;
    }
    return false;
}

boolean PubSubClient::loop() {
    
    if(isConnected) {
        handleUDPRxComms();
        return true;
    }
    return false;
}

boolean PubSubClient::publish(const char* topic, const char* payload) {
    return publish(topic,(const uint8_t*)payload, payload ? strnlen(payload, this->bufferSize) : 0,false);
}

boolean PubSubClient::publish(const char* topic, const char* payload, boolean retained) {
    return publish(topic,(const uint8_t*)payload, payload ? strnlen(payload, this->bufferSize) : 0,retained);
}

boolean PubSubClient::publish(const char* topic, const uint8_t* payload, unsigned int plength) {
    return publish(topic, payload, plength, false);
}

boolean PubSubClient::publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
    if (!isConnected)
        return false;

    isPublished = false;
    isErrored = false;

    /*
        MqttsnErrorCode mqttsn_client_publish(
        MqttsnClientHandle client,
        const char* topic,
        const unsigned char* msg,
        unsigned msgLen,
        MqttsnQoS qos,
        bool retain,
        MqttsnAsyncOpCompleteReportFn callback,
        void* data)
    */
    MqttsnErrorCode result = mqttsn_client_publish(
      _snClient, 
      topic,
      (const unsigned char *)payload,
      plength,
      MqttsnQoS_AtLeastOnceDelivery,
      retained,
      mqttsn_publish_complete,
      this);

    if(result != MqttsnErrorCode_Success) {
        return false;
    }

    while(!isPublished && !isErrored)
    {
      handleUDPRxComms();
      timer.run();
      delay(50);
    }

    return (!isErrored);
}

// TODO: Need to look at what _P means in terms of reading from program memory

boolean PubSubClient::publish_P(const char* topic, const char* payload, boolean retained) {
    return publish_P(topic, (const uint8_t*)payload, payload ? strnlen(payload, this->bufferSize) : 0, retained);
}

boolean PubSubClient::publish_P(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {

    if (!isConnected) {
        return false;
    }

    isPublished = false;
    isErrored = false;

    /*
        MqttsnErrorCode mqttsn_client_publish(
        MqttsnClientHandle client,
        const char* topic,
        const unsigned char* msg,
        unsigned msgLen,
        MqttsnQoS qos,
        bool retain,
        MqttsnAsyncOpCompleteReportFn callback,
        void* data)
    */
    MqttsnErrorCode result = mqttsn_client_publish(
      _snClient, 
      topic,
      (const unsigned char *)payload,
      plength,
      MqttsnQoS_AtLeastOnceDelivery,
      retained,
      mqttsn_publish_complete,
      this);

    if(result != MqttsnErrorCode_Success) {
        return false;
    }

    while(!isPublished && !isErrored)
    {
      handleUDPRxComms();
      timer.run();
      delay(50);
    }

    return (!isErrored);
}

#if false

boolean PubSubClient::beginPublish(const char* topic, unsigned int plength, boolean retained) {

    if (!isConnected) 
        return false;

    isPublished = false;
    isErrored = false;

    /*
        MqttsnErrorCode mqttsn_client_publish(
        MqttsnClientHandle client,
        const char* topic,
        const unsigned char* msg,
        unsigned msgLen,
        MqttsnQoS qos,
        bool retain,
        MqttsnAsyncOpCompleteReportFn callback,
        void* data)
    */
    MqttsnErrorCode result = mqttsn_client_publish(
      _snClient, 
      topic,
      (const unsigned char *)payload,
      plength,
      MqttsnQoS_AtLeastOnceDelivery,
      retained,
      mqttsn_publish_complete,
      this);

    // TODO Check return code
    return true;
}

int PubSubClient::endPublish() {

    while(!isPublished && !isErrored)
    {
      handleUDPRxComms();
      timer.run();
      delay(50);
    }

    return (!isErrored);
}

#endif

boolean PubSubClient::subscribe(const char* topic) {
    return subscribe(topic, 0);
}

boolean PubSubClient::subscribe(const char* topic, int8_t qos) {

    if (topic == 0) {
        return false;
    }

/* TODO: Put this buffer size check in correctly
    if (this->bufferSize < 9 + topicLength) {
        // Too long
        return false;
    }
*/

    // TODO: How are we going to map MQTT QoS to MQTTSN qos?
    if (!isConnected)
        return false;
        
    isSubscribed = false;
    isErrored = false;

    /*
        MqttsnErrorCode mqttsn_client_subscribe(
        MqttsnClientHandle client,
        const char* topic,
        MqttsnQoS qos,
        MqttsnSubscribeCompleteReportFn callback,
        void* data
        )
    */
    MqttsnErrorCode result = mqttsn_client_subscribe(
        _snClient, 
        topic,
        (MqttsnQoS)qos, /* max QoS */
        mqttsn_subscribe_complete,
        this);

    if(result != MqttsnErrorCode_Success) {
        return false;
    }

    while(!isSubscribed && !isErrored)
    {
        handleUDPRxComms();
        timer.run();
        delay(50);
    }
 
    return !isErrored;
}

boolean PubSubClient::unsubscribe(const char* topic) {

    if(!isConnected)
        return false;

    if(topic == 0)
        return false;

    /*
        MqttsnErrorCode mqttsn_client_unsubscribe(
        MqttsnClientHandle client,
        const char* topic,
        MqttsnAsyncOpCompleteReportFn callback,
        void* data
    */
    MqttsnErrorCode result =  mqttsn_client_unsubscribe(
        _snClient,
         topic,
        &mqttsn_unsubscribe_complete,
        this);

    if(result != MqttsnErrorCode_Success) {
        return false;
    }

    while(!isUnsubscribed && !isErrored)
    {
        handleUDPRxComms();
        timer.run();
        delay(50);
    }

    return !isErrored;
}

void PubSubClient::disconnect() {

    if(!isConnected)
        return;

    /*
        MqttsnErrorCode mqttsn_client_disconnect(
        MqttsnClientHandle client,
        MqttsnAsyncOpCompleteReportFn callback,
        void* data)
    */
   
    MqttsnErrorCode result = mqttsn_client_disconnect(_snClient, mqttsn_disconnect_complete, this);
    if(result != MqttsnErrorCode_Success)
        goto errorexit;

    while(isConnected && !isErrored) {
        handleUDPRxComms();
        timer.run();
        delay(50);
    }

errorexit:

    mqttsn_client_stop(_snClient);

    isConnected = false;
    isErrored = false;

    udp.stop();

    return;
}

boolean PubSubClient::connected() {
    return isConnected;
}

PubSubClient& PubSubClient::setServer(uint8_t * ip, uint16_t port) {
    IPAddress addr(ip[0],ip[1],ip[2],ip[3]);
    return setServer(addr,port);
}

PubSubClient& PubSubClient::setServer(IPAddress ip, uint16_t port) {
    this->ip = ip;
    this->port = port;
    this->domain = NULL;
    return *this;
}

PubSubClient& PubSubClient::setServer(const char * domain, uint16_t port) {
    this->domain = domain;
    this->port = port;
    return *this;
}

// TODO: Look at all this...

PubSubClient& PubSubClient::setCallback(MQTT_CALLBACK_SIGNATURE) {
    this->callback = callback;
    return *this;
}

PubSubClient& PubSubClient::setClient(Client& client){
    this->_client = &client;
    return *this;
}

PubSubClient& PubSubClient::setStream(Stream& stream){
    this->stream = &stream;
    return *this;
}

int PubSubClient::state() {
    return this->_state;
}

boolean PubSubClient::setBufferSize(uint16_t size) {
    if (size == 0) {
        // Cannot set it back to 0
        return false;
    }
    if (this->bufferSize == 0) {
        this->buffer = (uint8_t*)malloc(size);
    } else {
        uint8_t* newBuffer = (uint8_t*)realloc(this->buffer, size);
        if (newBuffer != NULL) {
            this->buffer = newBuffer;
        } else {
            return false;
        }
    }
    this->bufferSize = size;
    return (this->buffer != NULL);
}

uint16_t PubSubClient::getBufferSize() {
    return this->bufferSize;
}
PubSubClient& PubSubClient::setKeepAlive(uint16_t keepAlive) {
    this->keepAlive = keepAlive;
    return *this;
}
PubSubClient& PubSubClient::setSocketTimeout(uint16_t timeout) {
    this->socketTimeout = timeout;
    return *this;
}
