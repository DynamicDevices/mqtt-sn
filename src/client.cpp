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

#include "client.h"
#include "BasicClient.h"
#include "ClientMgr.h"
#include "option.h"
#include "ParsedOptions.h"

namespace
{

typedef std::tuple<
    
> ClientOptions;

typedef mqttsn::client::ParsedOptions<ClientOptions> ParsedClientOptions;

typedef mqttsn::client::BasicClient<ParsedClientOptions> MqttsnClient;
typedef mqttsn::client::ClientMgr<MqttsnClient, ParsedClientOptions> MqttsnClientMgr;

MqttsnClientMgr& getClientMgr()
{
    static MqttsnClientMgr Mgr;
    return Mgr;
}

}  // namespace

MqttsnClientHandle mqttsn_client_new()
{
    auto client = getClientMgr().alloc();
    return client.release();
}

void mqttsn_client_free(MqttsnClientHandle client)
{
    getClientMgr().free(reinterpret_cast<MqttsnClient*>(client));
}

void mqttsn_client_set_next_tick_program_callback(
    MqttsnClientHandle client,
    MqttsnNextTickProgramFn fn,
    void* data)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->setNextTickProgramCallback(fn, data);
}

void mqttsn_client_set_cancel_next_tick_wait_callback(
    MqttsnClientHandle client,
    MqttsnCancelNextTickWaitFn fn,
    void* data)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->setCancelNextTickWaitCallback(fn, data);
}

void mqttsn_client_set_send_output_data_callback(
    MqttsnClientHandle client,
    MqttsnSendOutputDataFn fn,
    void* data)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->setSendOutputDataCallback(fn, data);
}

void mqttsn_client_set_gw_status_report_callback(
    MqttsnClientHandle client,
    MqttsnGwStatusReportFn fn,
    void* data)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->setGwStatusReportCallback(fn, data);
}

void mqttsn_client_set_gw_disconnect_report_callback(
    MqttsnClientHandle client,
    MqttsnGwDisconnectReportFn fn,
    void* data)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->setGwDisconnectReportCallback(fn, data);
}

void mqttsn_client_set_message_report_callback(
    MqttsnClientHandle client,
    MqttsnMessageReportFn fn,
    void* data)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->setMessageReportCallback(fn, data);
}

MqttsnErrorCode mqttsn_client_start(MqttsnClientHandle client)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->start();
}

MqttsnErrorCode mqttsn_client_stop(MqttsnClientHandle client)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->stop();
}

unsigned mqttsn_client_process_data(
    void* client,
    const unsigned char* from,
    unsigned len)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->processData(from, len);
}

void mqttsn_client_tick(void* client)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->tick();
}

void mqttsn_client_set_retry_period(MqttsnClientHandle client, unsigned value)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->setRetryPeriod(value);
}

void mqttsn_client_set_retry_count(MqttsnClientHandle client, unsigned value)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->setRetryCount(value);
}

void mqttsn_client_set_broadcast_radius(MqttsnClientHandle client, unsigned char value)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->setBroadcastRadius(value);
}

void mqttsn_client_set_searchgw_enabled(
    MqttsnClientHandle client, 
    bool value)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->setSearchgwEnabled(value);
}   

void mqttsn_client_search_gw(MqttsnClientHandle client)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->sendSearchGw();
} 

void mqttsn_client_discard_gw(
    MqttsnClientHandle client, 
    unsigned char gwId)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->discardGw(gwId);
}    
    
void mqttsn_client_discard_all_gw(MqttsnClientHandle client)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    clientObj->discardAllGw();
}    

bool mqttsn_client_cancel(MqttsnClientHandle client)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->cancel();
}

MqttsnErrorCode mqttsn_client_connect(
    MqttsnClientHandle client,
    const char* clientId,
    unsigned short keepAliveSeconds,
    bool cleanSession,
    const MqttsnWillInfo* willInfo,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->connect(
        clientId,
        keepAliveSeconds,
        cleanSession,
        willInfo,
        callback,
        data);
}

MqttsnErrorCode mqttsn_client_reconnect(
    MqttsnClientHandle client,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->reconnect(callback, data);
}

MqttsnErrorCode mqttsn_client_disconnect(
    MqttsnClientHandle client,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->disconnect(callback, data);
}

MqttsnErrorCode mqttsn_client_publish_id(
    MqttsnClientHandle client,
    MqttsnTopicId topicId,
    const unsigned char* msg,
    unsigned msgLen,
    MqttsnQoS qos,
    bool retain,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->publish(topicId, msg, msgLen, qos, retain, callback, data);
}    

MqttsnErrorCode mqttsn_client_publish(
    MqttsnClientHandle client,
    const char* topic,
    const unsigned char* msg,
    unsigned msgLen,
    MqttsnQoS qos,
    bool retain,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->publish(topic, msg, msgLen, qos, retain, callback, data);
}    

MqttsnErrorCode mqttsn_client_subscribe_id(
    MqttsnClientHandle client,
    MqttsnTopicId topicId,
    MqttsnQoS qos,
    MqttsnSubscribeCompleteReportFn callback,
    void* data
)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->subscribe(topicId, qos, callback, data);
}

MqttsnErrorCode mqttsn_client_subscribe(
    MqttsnClientHandle client,
    const char* topic,
    MqttsnQoS qos,
    MqttsnSubscribeCompleteReportFn callback,
    void* data
)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->subscribe(topic, qos, callback, data);
}

MqttsnErrorCode mqttsn_client_unsubscribe_id(
    MqttsnClientHandle client,
    MqttsnTopicId topicId,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->unsubscribe(topicId, callback, data);
}


MqttsnErrorCode mqttsn_client_unsubscribe(
    MqttsnClientHandle client,
    const char* topic,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->unsubscribe(topic, callback, data);
}

MqttsnErrorCode mqttsn_client_will_update(
    MqttsnClientHandle client,
    const MqttsnWillInfo* willInfo,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->willUpdate(willInfo, callback, data);
}

MqttsnErrorCode mqttsn_client_will_topic_update(
    MqttsnClientHandle client,
    const char* topic,
    MqttsnQoS qos,
    bool retain,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->willTopicUpdate(topic, qos, retain, callback, data);
}

MqttsnErrorCode mqttsn_client_will_msg_update(
    MqttsnClientHandle client,
    const unsigned char* msg,
    unsigned msgLen,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->willMsgUpdate(msg, msgLen, callback, data);
}

MqttsnErrorCode mqttsn_client_sleep(
    MqttsnClientHandle client,
    unsigned short duration,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->sleep(duration, callback, data);
}

MqttsnErrorCode mqttsn_client_check_messages(
    MqttsnClientHandle client,
    MqttsnAsyncOpCompleteReportFn callback,
    void* data
)
{
    auto* clientObj = reinterpret_cast<MqttsnClient*>(client);
    return clientObj->checkMessages(callback, data);
}
