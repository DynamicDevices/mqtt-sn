//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

/// @file
/// @brief Functions of MQTT-SN client library.

#pragma once

#include "mqttsn/client/common.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/// @brief Allocate new client.
/// @details When work with the client is complete, mqttsn_client_free()
///     function, must be invoked.
/// @return Handle to allocated client object. This handle needs to be passed
///     as first parameter to all other API functions.
MqttsnClientHandle mqttsn_client_new();

/// @brief Free previously allocated client.
/// @details When used communication channel to the gateway is no longer
///     needed, the client data structes allocated with 
///     mqttsn_client_new() must be released using this function.
/// @param[in] client Handle returned by mqttsn_client_new() function.
void mqttsn_client_free(MqttsnClientHandle client);

/// @brief Set callback to call when time measurement is required.
/// @details The MQTT-SN client may require to measure time. When such 
///     measurement is required, the provided callback will be invoked with
///     the timeout duration in milliseconds. After requested time expires,
///     the mqttsn_client_tick() function must be invoked.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] fn Callback function.
/// @param[in] data Pointer to any user data structure. It will passed as one 
///     of the parameters in callback invocation. May be NULL.
void mqttsn_client_set_next_tick_program_callback(
    MqttsnClientHandle client,
    MqttsnNextTickProgramFn fn,
    void* data);
    
/// @brief Set callback to terminate current time measurement.
/// @details The client may request termination of currently running time 
///     measurement, previously requested via callback, which was set using
///     mqttsn_client_set_next_tick_program_callback() function. This function
///     sets appropriate callback. When invoked, it must returne number of
///     elapsed milliseconds since previoius time measurement request.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] fn Callback function.
/// @param[in] data Pointer to any user data structure. It will passed as one 
///     of the parameters in callback invocation. May be NULL.
void mqttsn_client_set_cancel_next_tick_wait_callback(
    MqttsnClientHandle client,
    MqttsnCancelNextTickWaitFn fn,
    void* data);
    
/// @brief Set callback to send raw data over I/O link.
/// @details The callback is invoked when there is a need to send data
///     to the gateway. The callback is invoked for every single message
///     that need to be sent as a single datagram. 
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] fn Callback function.
/// @param[in] data Pointer to any user data structure. It will passed as one 
///     of the parameters in callback invocation. May be NULL.
void mqttsn_client_set_send_output_data_callback(
    MqttsnClientHandle client,
    MqttsnSendOutputDataFn fn,
    void* data);
    
/// @brief Set callback to report status of the gateway.
/// @details The callback is invoked when gateway status has changed.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] fn Callback function.
/// @param[in] data Pointer to any user data structure. It will passed as one 
///     of the parameters in callback invocation. May be NULL.
void mqttsn_client_set_gw_status_report_callback(
    MqttsnClientHandle client,
    MqttsnGwStatusReportFn fn,
    void* data);

/// @brief Set callback to report unsolicited disconnection of the gateway.
/// @details The callback will be invoked when gateway sends unsolicited 
///     @b DISCONNECT message or does not reply to @b PINGREQ message.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] fn Callback function.
/// @param[in] data Pointer to any user data structure. It will passed as one 
///     of the parameters in callback invocation. May be NULL.
void mqttsn_client_set_gw_disconnect_report_callback(
    MqttsnClientHandle client,
    MqttsnGwDisconnectReportFn fn,
    void* data);

/// @brief Set callback to report incoming messages.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] fn Callback function.
/// @param[in] data Pointer to any user data structure. It will passed as one 
///     of the parameters in callback invocation. May be NULL.
void mqttsn_client_set_message_report_callback(
    MqttsnClientHandle client,
    MqttsnMessageReportFn fn,
    void* data);
    
/// @brief Start the library's operation.
/// @details The function will check whether all necessary callback functions
///     were set. In not @ref MqttsnErrorCode_BadParam will be returned.
///     If search for gateways is enabled (see description of 
///     mqttsn_client_set_searchgw_enabled()), the library may
///     send @b SEARCHGW message by invoking the callback, set by
///     mqttsn_client_set_send_output_data_callback().
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_start(MqttsnClientHandle client);

/// @brief Stop the library's operation.
/// @details The operation may be resumed using mqttsn_client_start(). 
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @return Error code indicating success/failure status of the operation
MqttsnErrorCode mqttsn_client_stop(MqttsnClientHandle client);

/// @brief Provide data, received over I/O link, to the library for processing.
/// @details This call may cause invocation of some callbacks, such as 
///     request to cancel the currently running time measurement, send some messages to
///     the gateway, report incoming application message, and (re)start time
///     measurement.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] buf Pointer to the buffer of data to process.
/// @param[in] bufLen Number of bytes in the data buffer.
/// @return Number of processed bytes.
/// @note The function returns number of bytes that were actually consumed, and
///     can be removed from the holding buffer.
unsigned mqttsn_client_process_data(MqttsnClientHandle client, const unsigned char* buf, unsigned bufLen);

/// @brief Notify client about requested time expiry.
/// @details The reported amount of milliseconds needs to be from the 
///     last request to program timer via callback (set by
///     mqttsn_client_set_next_tick_program_callback()).
///     It can be less than actually requested via the callback. If this
///     function is called, the library assumes that previously requested
///     timeout measurement is not in progress any more, and will request
///     new measurement if needed.
///     This call may cause invocation of some other callbacks, such as a request
///     to send new data to the gateway.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] ms Number of elapsed @b milliseconds.
void mqttsn_client_tick(MqttsnClientHandle client);

/// @brief Set retry period to wait between resending unacknowledged message to the gateway.
/// @details Some messages, sent to the gateway, may require acknowledgement by 
///     the latter. The delay (in seconds) between such attempts to resend the
///     message may be specified using this function. The default value is
///     @b 15 seconds.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] value Number of @b seconds to wait before making an attempt to resend.
void mqttsn_client_set_retry_period(MqttsnClientHandle client, unsigned value);

/// @brief Set number of retry attempts to perform before reporting unsuccessful result of the operation.
/// @details Some messages, sent to the gateway, may require acknowledgement by 
///     the latter. The amount of retry attempts before reporting unsuccessful result
///     of the operation may be specified using this function. The default value
///     is @b 3.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] value Number of retry attempts.
void mqttsn_client_set_retry_count(MqttsnClientHandle client, unsigned value);

/// @brief Set broadcast radius.
/// @details When searching for gateways, the client library broadcasts @b SEARCHGW
///     messages. It contains the broadcast radius value. This value can be
///     set using this function. Default radius value is 0.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] value Broadcast radius.
void mqttsn_client_set_broadcast_radius(MqttsnClientHandle client, unsigned char value);

/// @brief Enable/Disable search for gateways.
/// @details According to @b MQTT-SN protocol specification the client may 
///     search for the gateways by broadcasting @b SEARCHGW message. 
///     If address of the gateway is known (such as known UDP host), there may
///     be no need to send these messages. To enable/disable search for available
///     gateways can be done using this function. By default the search for
///     the gateway is @b enabled.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] value @b true to enable, and @false to disable.
void mqttsn_client_set_searchgw_enabled(MqttsnClientHandle client, bool value);

/// @brief Send @b SEARCHGW message.
/// @details This function performs one send of @b SEARCHGW message regardless
///     of whether the search for gateways is enabled or disabled. 
/// @param[in] client Handle returned by mqttsn_client_new() function.
void mqttsn_client_search_gw(MqttsnClientHandle client);

/// @brief Discard information about the gateway.
/// @details The client library maintains the list of gateways that either
///     advertised their presence or replied to previously sent @b SEARCHGW
///     messages. This function causes information about specified gateway
///     to be dropped. If such information exists, the callback, set
///     using mqttsn_client_set_gw_status_report_callback() function,
///     will be invoked reporting @ref MqttsnGwStatus_Discarded as status value.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] gwId ID of the gateway.
void mqttsn_client_discard_gw(MqttsnClientHandle client, unsigned char gwId);

/// @brief Discard information about all gateways.
/// @details The client library maintains the list of gateways that either
///     advertised their presence or replied to previously sent @b SEARCHGW
///     messages. This function causes the callback, set
///     using mqttsn_client_set_gw_status_report_callback() function,
///     to be invoked reporting @ref MqttsnGwStatus_Discarded as status value for
///     all the recorded gateways.
/// @param[in] client Handle returned by mqttsn_client_new() function.
void mqttsn_client_discard_all_gw(MqttsnClientHandle client);

/// @brief Cancel current asynchronous operation.
/// @details The library provides support for multiple asynchronous operations,
///     which report their completion via provided callback. The library also
///     doesn't support start of the new operation before previous one completed.
///     This function provides an ability to cancel existing operation to allow
///     issue of the new request. When successfully cancelled the callback of 
///     the asyncrhonous operation will report @ref MqttsnAsyncOpStatus_Aborted
///     as operation result status. 
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @return true in case an asynchronous operation was cancelled,
///     false otherwise.
bool mqttsn_client_cancel(MqttsnClientHandle client);

/// @brief Issue the connect request to the gateway.
/// @details This function needs to be called to connect to the gateway. 
///     When the  operation is complete, the provided callback
///     will be invoked. Note, that the function cannot be successfully called when
///     client is already connected to the gateway. In order to change the
///     will information use mqttsn_client_will_update() function.
///     If some other operation timed out, use mqttsn_client_reconnect()
///     to refresh the connection.
///     In order to change client ID and/or "keep alive" value disconnect
///     first (using mqttsn_client_disconnect()) and then try to 
///     connect again. 
///
///     @b IMPORTANT : The buffers containing client ID string, as well as
///     will topic and message (if such exist) must be preserved intact
///     until the connect operation is complete (provided
///     callback is invoked). The @b MqttsnWillInfo structure passed as
///     @b willInfo parameter may reside on the stack and be destructed 
///     immediately after this function returns, but the buffers its data
///     members point to must be preserved. 
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] clientId Client ID string.
/// @param[in] keepAliveSeconds Maximum number of seconds allowed between the 
///     messages sent to the gateway. If no publish is performed during this
///     timeframe, the library will generate @b PINGREQ message and send to the
///     gateway.
/// @param[in] cleanSession Request to establish clean session.
/// @param[in] willInfo Requested will information, can be NULL.
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_connect(
    MqttsnClientHandle client,
    const char* clientId,
    unsigned short keepAliveSeconds,
    bool cleanSession,
    const MqttsnWillInfo* willInfo,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data);
    
/// @brief Reconnect to the gateway using previously used client ID and keep alive
///     period.
/// @details The invocation will work only when in "connected" or "asleep" state,
///     It will not work after invocation of mqttsn_client_disconnect(). 
///     When the operation is complete, the provided callback
///     will be invoked. If successful completion is reported, the client
///     is properly connected to the gateway again.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_reconnect(
    MqttsnClientHandle client,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
);

/// @brief Disconnect from the gateway.
/// @details When the operation is complete, the provided callback
///     will be invoked. Regardless of the reported disconnect result,
///     the library assumes "disconnected" internal state and some functions,
///     such as requests to publish and/or subscribe may not work.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_disconnect(
    MqttsnClientHandle client,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data);

/// @brief Publish message with predefined topic ID.
/// @details When publish operation is complete, the provided callback
///     will be invoked. Note, that
///     the callback will be invoked immediately for publish operation with
///     QoS=-1 or QoS=0.
///
///     @b IMPORTANT : The buffer containing message data must be preserved
///     intact until the end of the operation (provided callback is invoked).
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] topicId Predefined topic ID.
/// @param[in] msg Pointer to buffer containing data to be published.
/// @param[in] msgLen Size of the buffer containing data to be published.
/// @param[in] qos Quality of service level.
/// @param[in] retain Retain flag.
/// @param[in] callback Callback to be invoked when operation is complete.
///     It can be NULL for publish requests with QoS=-1 or QoS=0. Such publish
///     requests do not require acknowledgement from the gateway and report
///     their completion immediatelly.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_publish_id(
    MqttsnClientHandle client,
    MqttsnTopicId topicId,
    const unsigned char* msg,
    unsigned msgLen,
    MqttsnQoS qos,
    bool retain,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
);

/// @brief Publish message with topic string.
/// @details When publish operation is complete, the provided callback
///     will be invoked. Note, that
///     the callback MAY be invoked immediately for publish operation with
///     QoS=0 if requested topic is already registered.
///
///     @b IMPORTANT : The buffer containing message data must be preserved
///     intact until the end of the operation (provided callback is invoked).
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] topic Topic string.
/// @param[in] msg Pointer to buffer containing data to be published.
/// @param[in] msgLen Size of the buffer containing data to be published.
/// @param[in] qos Quality of service level.
/// @param[in] retain Retain flag.
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_publish(
    MqttsnClientHandle client,
    const char* topic,
    const unsigned char* msg,
    unsigned msgLen,
    MqttsnQoS qos,
    bool retain,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
);

/// @brief Subscribe to topic having predefined topic ID.
/// @details When subscribe operation is complete, the provided callback
///     will be invoked. 
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] topicId Predefined topic ID.
/// @param[in] qos Maximum level of quality of service the messages are sent to this client.
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_subscribe_id(
    MqttsnClientHandle client,
    MqttsnTopicId topicId,
    MqttsnQoS qos,
    MqttsnSubscribeCompleteReportFn callback,
    void* data
);

/// @brief Subscribe to topic.
/// @details When subscribe operation is complete, the provided callback
///     will be invoked. 
///
///     @b IMPORTANT : The buffer containing topic string must be preserved
///     intact until the end of the operation (provided callback is invoked).
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] topic Topic string.
/// @param[in] qos Maximum level of quality of service the messages are sent to this client.
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_subscribe(
    MqttsnClientHandle client,
    const char* topic,
    MqttsnQoS qos,
    MqttsnSubscribeCompleteReportFn callback,
    void* data
);

/// @brief Unsubscribe from messages having predefined topic ID.
/// @details When unsubscribe operation is complete, the provided callback
///     will be invoked. 
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] topicId Predefined topic ID.
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_unsubscribe_id(
    MqttsnClientHandle client,
    MqttsnTopicId topicId,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
);

/// @brief Unsubscribe from messages having specified topic.
/// @details When unsubscribe operation is complete, the provided callback
///     will be invoked. 
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] topic Topic string.
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_unsubscribe(
    MqttsnClientHandle client,
    const char* topic,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
);

/// @brief Update will recorded with the gateway/broker.
/// @details When the operation is complete, the provided callback
///     will be invoked. 
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] willInfo New will full information. The parameter can be NULL
///     or the @b topic data member of the @ref MqttsnWillInfo struct may be
///     null. In this case the request means erase current will information
///     recorded with the gateway/broker.
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_will_update(
    MqttsnClientHandle client,
    const MqttsnWillInfo* willInfo,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
);

/// @brief Update topic of the will recorded with the gateway/broker.
/// @details When the operation is complete, the provided callback
///     will be invoked. 
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] topic New will topic string. The parameter can be NULL.
///     In this case the request means erase current will information
///     recorded with the gateway/broker.
/// @param[in] qos Quality of service for the will.
/// @param[in] retain Retain flag.
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_will_topic_update(
    MqttsnClientHandle client,
    const char* topic,
    MqttsnQoS qos,
    bool retain,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
);

/// @brief Update will message body recorded with the gateway/broker.
/// @details When the operation is complete, the provided callback
///     will be invoked. 
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] topic New will topic string. The parameter can be NULL.
///     In this case the request means erase current will information
///     recorded with the gateway/broker.
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_will_msg_update(
    MqttsnClientHandle client,
    const unsigned char* msg,
    unsigned msgLen,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
);

/// @brief Notify gateway about entering the sleep state.
/// @details When the operation is complete, the provided callback
///     will be invoked. If successful completion is reported, the client may
///     enter the low power mode and do not send any messages within the
///     "keep alive" period specified in mqttsn_client_connect() 
///     invocation. However, there is a need to either wake up (using 
///     mqttsn_client_reconnect()) or check for the accumulated messages
///     (using mqttsn_client_check_messages()) within the period 
///     specified in this function invocation.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] duration Muximal number of seconds the client will remain in
///     the sleep state until next check of accumulated pending messages 
///     (use mqttsn_client_check_messages() for this purpose) or
///     full wake up (use mqttsn_client_reconnect() for this purpose).
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_sleep(
    MqttsnClientHandle client,
    unsigned short duration,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
);

/// @brief Check for accumulated pending messages the gateway has to report.
/// @details Can be invoked after successful entering the sleep state using
///     mqttsn_client_sleep(). 
///     According to MQTT-SN protocol specification, this function will
///     force sending @b PINGREQ message to the gateway. The gateway then will
///     deliver all the pending messages, which may cause multiple invocation
///     of message report callback (set with
///     mqttsn_client_set_message_report_callback()). After gateway
///     reports all accumulated messages, it will send @b PINGRESP message,
///     which will case invocation of the provided callback. After the
///     callback is invoked, the client may return to "asleep" state and will
///     be required to check messages again or wake up after number of seconds
///     provided in mqttsn_client_sleep() call.
/// @param[in] client Handle returned by mqttsn_client_new() function.
/// @param[in] callback Callback to be invoked when operation is complete,
///     must @b NOT be NULL.
/// @param[in] data Pointer to any user data, it will be passed as the first
///     parameter to the invoked completion report callback, can be NULL.
/// @return Error code indicating success/failure status of the operation.
MqttsnErrorCode mqttsn_client_check_messages(
    MqttsnClientHandle client,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
);

#ifdef __cplusplus
}
#endif
