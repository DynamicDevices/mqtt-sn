//
// Copyright 2019 - 2020 (C). Alex Robenko. All rights reserved.
//

// This library is free software: you can redistribute it and/or modify
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
/// Contains extra logic to help with dispatching message types and objects

#pragma once

#include <type_traits>

#include "comms/Message.h"
#include "comms/details/dispatch_impl.h"

namespace comms
{

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using polymorphic behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename TMsg,
    typename THandler>
auto dispatchMsgPolymorphic(TId&& id, TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    using MsgType = typename std::decay<decltype(msg)>::type;
    static_assert(comms::isMessage<MsgType>(), "msg param must be a valid message");
    using HandlerType = typename std::decay<decltype(handler)>::type;
    return 
        details::DispatchMsgPolymorphicHelper<TAllMessages, MsgType, HandlerType>::
            dispatch(std::forward<TId>(id), msg, handler);
}

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using polymorphic behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] index Index (or offset) of the message type among those having the same ID.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename TMsg,
    typename THandler>
auto dispatchMsgPolymorphic(TId&& id, std::size_t index, TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    using MsgType = typename std::decay<decltype(msg)>::type;
    static_assert(comms::isMessage<MsgType>(), "msg param must be a valid message");
    using HandlerType = typename std::decay<decltype(handler)>::type;
    return 
        details::DispatchMsgPolymorphicHelper<TAllMessages, MsgType, HandlerType>::
            dispatch(std::forward<TId>(id), index, msg, handler);
}

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using polymorphic behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page.
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TMsg,
    typename THandler>
auto dispatchMsgPolymorphic(TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    using MsgType = typename std::decay<decltype(msg)>::type;
    static_assert(comms::isMessage<MsgType>(), "msg param must be a valid message");
    using HandlerType = typename std::decay<decltype(handler)>::type;
    return 
        details::DispatchMsgPolymorphicHelper<TAllMessages, MsgType, HandlerType>::
            dispatch(msg, handler);
}

/// @brief Dispatch message id into appropriate @b handle() function in the
///     provided handler using polymorphic behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_type section of the 
///     @ref page_dispatch tutorial page.
/// @return @b true in case the appropriate @b handle() member function of the
///     handler object has been called, @b false otherwise.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename THandler>
bool dispatchMsgTypePolymorphic(TId&& id, THandler& handler) 
{
    using HandlerType = typename std::decay<decltype(handler)>::type;
    return 
        details::DispatchMsgTypePolymorphicHelper<TAllMessages, HandlerType>::
            dispatch(std::forward<TId>(id), handler);
}

/// @brief Dispatch message id into appropriate @b handle() function in the
///     provided handler using polymorphic behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] index Index (or offset) of the message type among those having the same ID.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_type section of the 
///     @ref page_dispatch tutorial page.
/// @return @b true in case the appropriate @b handle() member function of the
///     handler object has been called, @b false otherwise.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename THandler>
bool dispatchMsgTypePolymorphic(TId&& id, std::size_t index, THandler& handler)
{
    using HandlerType = typename std::decay<decltype(handler)>::type;
    return 
        details::DispatchMsgTypePolymorphicHelper<TAllMessages, HandlerType>::
            dispatch(std::forward<TId>(id), index, handler);
}

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using static binary search behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] index Index (or offset) of the message type among those having the same ID.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename TMsg,
    typename THandler>
auto dispatchMsgStaticBinSearch(TId&& id, std::size_t index, TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    static_assert(details::allMessagesHaveStaticNumId<TAllMessages>(), 
        "All messages in the provided tuple must statically define their numeric ID");


    return 
        details::DispatchMsgStaticBinSearchHelper<TAllMessages>::dispatch(
            std::forward<TId>(id),
            index,
            msg,
            handler);
}

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using static binary search behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page.
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename TMsg,
    typename THandler>
auto dispatchMsgStaticBinSearch(TId&& id, TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    static_assert(details::allMessagesHaveStaticNumId<TAllMessages>(), 
        "All messages in the provided tuple must statically define their numeric ID");

    return 
        details::DispatchMsgStaticBinSearchHelper<TAllMessages>::dispatch(
            std::forward<TId>(id),
            msg,
            handler);
}

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using static binary search behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page.
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TMsg,
    typename THandler>
auto dispatchMsgStaticBinSearch(TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    static_assert(details::allMessagesHaveStaticNumId<TAllMessages>(), 
        "All messages in the provided tuple must statically define their numeric ID");
    using MsgType = typename std::decay<decltype(msg)>::type;
    static_assert(MsgType::hasGetId(), 
        "The used message object must provide polymorphic ID retrieval function");

    return 
        details::DispatchMsgStaticBinSearchHelper<TAllMessages>::dispatch(
            msg,
            handler);
}

/// @brief Dispatch message id into appropriate @b handle() function in the
///     provided handler using static binary search behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_type section of the 
///     @ref page_dispatch tutorial page.
/// @return @b true in case the appropriate @b handle() member function of the
///     handler object has been called, @b false otherwise.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename THandler>
bool dispatchMsgTypeStaticBinSearch(TId&& id, THandler& handler) 
{
    static_assert(details::allMessagesHaveStaticNumId<TAllMessages>(), 
        "All messages in the provided tuple must statically define their numeric ID");

    return 
        details::DispatchMsgStaticBinSearchHelper<TAllMessages>::
            dispatchType(std::forward<TId>(id), handler);
}

/// @brief Dispatch message id into appropriate @b handle() function in the
///     provided handler using static binary search behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] index Index (or offset) of the message type among those having the same ID.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_type section of the 
///     @ref page_dispatch tutorial page.
/// @return @b true in case the appropriate @b handle() member function of the
///     handler object has been called, @b false otherwise.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename THandler>
bool dispatchMsgTypeStaticBinSearch(TId&& id, std::size_t index, THandler& handler)
{
    static_assert(details::allMessagesHaveStaticNumId<TAllMessages>(), 
        "All messages in the provided tuple must statically define their numeric ID");

    return 
        details::DispatchMsgStaticBinSearchHelper<TAllMessages>::
            dispatchType(std::forward<TId>(id), index, handler);
}

/// @brief Count number of message types in the provided tuple that
///     have the requested numeric ID.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @note Defined in comms/dispatch.h
template <typename TAllMessages, typename TId>
std::size_t dispatchMsgTypeCountStaticBinSearch(TId&& id) 
{
    static_assert(details::allMessagesHaveStaticNumId<TAllMessages>(), 
        "All messages in the provided tuple must statically define their numeric ID");

    return 
        details::DispatchMsgStaticBinSearchHelper<TAllMessages>::
            dispatchTypeCount(std::forward<TId>(id));
}

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using linear switch behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page.
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename TMsg,
    typename THandler>
auto dispatchMsgLinearSwitch(TId&& id, TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    static_assert(details::allMessagesHaveStaticNumId<TAllMessages>(), 
        "All messages in the provided tuple must statically define their numeric ID");

    return 
        details::DispatchMsgLinearSwitchHelper<TAllMessages>::dispatch(
            std::forward<TId>(id),
            msg,
            handler);
}

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using linear switch behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] index Index (or offset) of the message type among those having the same ID.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page.
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename TMsg,
    typename THandler>
auto dispatchMsgLinearSwitch(TId&& id, std::size_t index, TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    static_assert(details::allMessagesHaveStaticNumId<TAllMessages>(), 
        "All messages in the provided tuple must statically define their numeric ID");


    return 
        details::DispatchMsgLinearSwitchHelper<TAllMessages>::dispatch(
            std::forward<TId>(id),
            index,
            msg,
            handler);
}

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using linear switch behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page.
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TMsg,
    typename THandler>
auto dispatchMsgLinearSwitch(TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    static_assert(details::allMessagesHaveStaticNumId<TAllMessages>(), 
        "All messages in the provided tuple must statically define their numeric ID");
    using MsgType = typename std::decay<decltype(msg)>::type;
    static_assert(MsgType::hasGetId(), 
        "The used message object must provide polymorphic ID retrieval function");

    return 
        details::DispatchMsgLinearSwitchHelper<TAllMessages>::dispatch(
            msg,
            handler);
}

/// @brief Dispatch message id into appropriate @b handle() function in the
///     provided handler using linear switch behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_type section of the 
///     @ref page_dispatch tutorial page.
/// @return @b true in case the appropriate @b handle() member function of the
///     handler object has been called, @b false otherwise.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename THandler>
bool dispatchMsgTypeLinearSwitch(TId&& id, THandler& handler) 
{
    static_assert(details::allMessagesHaveStaticNumId<TAllMessages>(), 
        "All messages in the provided tuple must statically define their numeric ID");

    return 
        details::DispatchMsgLinearSwitchHelper<TAllMessages>::
            dispatchType(std::forward<TId>(id), handler);
}

/// @brief Dispatch message id into appropriate @b handle() function in the
///     provided handler using linear switch behavior.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] index Index (or offset) of the message type among those having the same ID.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_type section of the 
///     @ref page_dispatch tutorial page.
/// @return @b true in case the appropriate @b handle() member function of the
///     handler object has been called, @b false otherwise.
/// @note Defined in comms/dispatch.h
template <
    typename TAllMessages,
    typename TId,
    typename THandler>
bool dispatchMsgTypeLinearSwitch(TId&& id, std::size_t index, THandler& handler)
{
    static_assert(details::allMessagesHaveStaticNumId<TAllMessages>(), 
        "All messages in the provided tuple must statically define their numeric ID");

    return 
        details::DispatchMsgLinearSwitchHelper<TAllMessages>::
            dispatchType(std::forward<TId>(id), index, handler);
}

/// @brief Compile time check whether the message object can use its own
///     polymorphic @b dispatch() (see @ref page_use_prot_interface_handle)
///     when @ref dispatchMsg() is invoked.
/// @tparam TMsg Type of the message interface.
/// @tparam THandler Type of the message handler.
/// @note Defined in comms/dispatch.h
template <typename TMsg, typename THandler>
constexpr bool dispatchMsgIsDirect()
{
    return details::dispatchMsgPolymorphicIsCompatibleHandler<typename std::decay<TMsg>::type, typename std::decay<THandler>::type>();
}

/// @brief Similar to other @ref dispatchMsgIsDirect(), but
///     can help in deducing template arguments.
/// @param msg Reference to the message object via its interface class.
/// @param handler Reference to the handler object.
/// @note The result can be evaluated at compile time, but compilers
///     don't support usage of this form in static_assert.
/// @note Defined in comms/dispatch.h
template <typename TMsg, typename THandler>
constexpr bool dispatchMsgIsDirect(TMsg&& msg, THandler&& handler)
{
    return dispatchMsgIsDirect<typename std::decay<decltype(msg)>::type, typename std::decay<decltype(handler)>::type>();
}


namespace details
{
template <typename TAllMessages>
class DispatchMsgHelper
{
public:
    template <typename TMsg, typename THandler>
    static auto dispatchMsg(TMsg& msg, THandler& handler) ->
        MessageInterfaceDispatchRetType<
            typename std::decay<decltype(handler)>::type>
    {
        return dispatchMsgInternal(msg, handler, HandlerAdjustedTag<TMsg, THandler>());
    }

    template <typename TId, typename TMsg, typename THandler>
    static auto dispatchMsg(TId&& id, TMsg& msg, THandler& handler) ->
        MessageInterfaceDispatchRetType<
            typename std::decay<decltype(handler)>::type>
    {
        return dispatchMsgInternal(std::forward<TId>(id), msg, handler, HandlerAdjustedTag<TMsg, THandler>());
    }

    template <typename TId, typename TMsg, typename THandler>
    static auto dispatchMsg(TId&& id, std::size_t index, TMsg& msg, THandler& handler) ->
        MessageInterfaceDispatchRetType<
            typename std::decay<decltype(handler)>::type>
    {
        return dispatchMsgInternal(std::forward<TId>(id), index, msg, handler, HandlerAdjustedTag<TMsg, THandler>());
    }

    template <typename TId, typename THandler>
    static bool dispatchMsgType(TId&& id, THandler& handler) 
    {
        return dispatchMsgTypeInternal(std::forward<TId>(id), handler, Tag());
    }

    template <typename TId, typename THandler>
    static bool dispatchMsgType(TId&& id, std::size_t index, THandler& handler)
    {
        return dispatchMsgTypeInternal(std::forward<TId>(id), index, handler, Tag());
    }

    static constexpr bool isPolymorphic()
    {
        return std::is_same<Tag, PolymorphicTag>::value;
    }

    static constexpr bool isStaticBinSearch()
    {
        return std::is_same<Tag, StaticBinSearchTag>::value;
    }

    template <typename TMsg, typename THandler>
    static constexpr bool isDirect()
    {
        return dispatchMsgIsDirect<TMsg, THandler>();
    }

    template <typename TMsg, typename THandler>
    static constexpr bool isDirect(TMsg&& msg, THandler&& handler)
    {
        return isDirect<typename std::decay<decltype(msg)>::type, typename std::decay<decltype(handler)>::type>();
    }

private:
    struct PolymorphicTag {};
    struct StaticBinSearchTag {};

    using Tag = 
        typename std::conditional<
            dispatchMsgPolymorphicIsDirectSuitable<TAllMessages>() || (!allMessagesHaveStaticNumId<TAllMessages>()),
            PolymorphicTag,
            StaticBinSearchTag
        >::type;

    template <typename TMsgBase, typename THandler>
    using HandlerAdjustedTag =
        typename std::conditional<
            dispatchMsgIsDirect<TMsgBase, THandler>(),
            PolymorphicTag,
            Tag
        >::type;

    template <typename TMsg, typename THandler>
    static auto dispatchMsgInternal(TMsg& msg, THandler& handler, PolymorphicTag) ->
        MessageInterfaceDispatchRetType<
            typename std::decay<decltype(handler)>::type>
    {
        return comms::dispatchMsgPolymorphic<TAllMessages>(msg, handler);
    }

    template <typename TMsg, typename THandler>
    static auto dispatchMsgInternal(TMsg& msg, THandler& handler, StaticBinSearchTag) ->
        MessageInterfaceDispatchRetType<
            typename std::decay<decltype(handler)>::type>
    {
        return comms::dispatchMsgStaticBinSearch<TAllMessages>(msg, handler);
    }

    template <typename TId, typename TMsg, typename THandler>
    static auto dispatchMsgInternal(TId&& id, TMsg& msg, THandler& handler, PolymorphicTag) ->
        MessageInterfaceDispatchRetType<
            typename std::decay<decltype(handler)>::type>
    {
        return comms::dispatchMsgPolymorphic<TAllMessages>(std::forward<TId>(id), msg, handler);
    }

    template <typename TId, typename TMsg, typename THandler>
    static auto dispatchMsgInternal(TId&& id, TMsg& msg, THandler& handler, StaticBinSearchTag) ->
        MessageInterfaceDispatchRetType<
            typename std::decay<decltype(handler)>::type>
    {
        return comms::dispatchMsgStaticBinSearch<TAllMessages>(std::forward<TId>(id), msg, handler);
    }

    template <typename TId, typename TMsg, typename THandler>
    static auto dispatchMsgInternal(TId&& id, std::size_t index, TMsg& msg, THandler& handler, PolymorphicTag) ->
        MessageInterfaceDispatchRetType<
            typename std::decay<decltype(handler)>::type>
    {
        return comms::dispatchMsgPolymorphic<TAllMessages>(std::forward<TId>(id), index, msg, handler);
    }

    template <typename TId, typename TMsg, typename THandler>
    static auto dispatchMsgInternal(TId&& id, std::size_t index, TMsg& msg, THandler& handler, StaticBinSearchTag) ->
        MessageInterfaceDispatchRetType<
            typename std::decay<decltype(handler)>::type>
    {
        return comms::dispatchMsgStaticBinSearch<TAllMessages>(std::forward<TId>(id), index, msg, handler);
    }

    template <typename TId, typename THandler>
    static bool dispatchMsgTypeInternal(TId&& id, THandler& handler, PolymorphicTag) 
    {
        return comms::dispatchMsgTypePolymorphic<TAllMessages>(std::forward<TId>(id), handler);
    }

    template <typename TId, typename THandler>
    static bool dispatchMsgTypeInternal(TId&& id, THandler& handler, StaticBinSearchTag) 
    {
        return comms::dispatchMsgTypeStaticBinSearch<TAllMessages>(std::forward<TId>(id), handler);
    }

    template <typename TId, typename THandler>
    static bool dispatchMsgTypeInternal(TId&& id, std::size_t index, THandler& handler, PolymorphicTag)
    {
        return comms::dispatchMsgTypePolymorphic<TAllMessages>(std::forward<TId>(id), index, handler);
    }

    template <typename TId, typename THandler>
    static bool dispatchMsgTypeInternal(TId&& id, std::size_t index, THandler& handler, StaticBinSearchTag)
    {
        return comms::dispatchMsgTypeStaticBinSearch<TAllMessages>(std::forward<TId>(id), index, handler);
    }
};

} // namespace details

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using either "polymorphic" or "static binary search" behavior.
/// @details The function performs compile time evaluation of the provided @b TAllMessages
///     tuple and uses logic described in @ref page_dispatch_message_object_default
///     to choose the way to dispatch.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page.
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
/// @see @ref dispatchMsgIsPolymorphic()
/// @see @ref dispatchMsgIsStaticBinSearch()
/// @see @ref dispatchMsgIsDirect();
template <
    typename TAllMessages,
    typename TId,
    typename TMsg,
    typename THandler>
auto dispatchMsg(TId&& id, TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    return details::DispatchMsgHelper<TAllMessages>::dispatchMsg(std::forward<TId>(id), msg, handler); 
}

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using either "polymorphic" or "static binary search" behavior.
/// @details The function performs compile time evaluation of the provided @b TAllMessages
///     tuple and uses logic described in @ref page_dispatch_message_object_default
///     to choose the way to dispatch.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] index Index (or offset) of the message type among those having the same ID.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page.
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
/// @see @ref dispatchMsgIsPolymorphic()
/// @see @ref dispatchMsgIsStaticBinSearch()
/// @see @ref dispatchMsgIsDirect();
template <
    typename TAllMessages,
    typename TId,
    typename TMsg,
    typename THandler>
auto dispatchMsg(TId&& id, std::size_t index, TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    return details::DispatchMsgHelper<TAllMessages>::dispatchMsg(std::forward<TId>(id), index, msg, handler);
}

/// @brief Dispatch message object into appropriate @b handle() function in the
///     provided handler using either "polymorphic" or "static binary search" behavior.
/// @details The function performs compile time evaluation of the provided @b TAllMessages
///     tuple and uses logic described in @ref page_dispatch_message_object_default
///     to choose the way to dispatch.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] msg Message object held by reference to its interface class.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_object section of the 
///     @ref page_dispatch tutorial page.
/// @return What the called @b handle() member function of handler object returns.
/// @note Defined in comms/dispatch.h
/// @see @ref dispatchMsgIsPolymorphic()
/// @see @ref dispatchMsgIsStaticBinSearch()
/// @see @ref dispatchMsgIsDirect();
template <
    typename TAllMessages,
    typename TMsg,
    typename THandler>
auto dispatchMsg(TMsg& msg, THandler& handler) ->
    details::MessageInterfaceDispatchRetType<
        typename std::decay<decltype(handler)>::type>
{
    return details::DispatchMsgHelper<TAllMessages>::dispatchMsg(msg, handler); 
}

/// @brief Dispatch message id into appropriate @b handle() function in the
///     provided handler using either "polymorphic" or "static binary search" behavior.
/// @details The function performs compile time evaluation of the provided @b TAllMessages
///     tuple and uses logic described in @ref page_dispatch_message_object_default
///     to choose the way to dispatch.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_type section of the 
///     @ref page_dispatch tutorial page.
/// @return @b true in case the appropriate @b handle() member function of the
///     handler object has been called, @b false otherwise.
/// @note Defined in comms/dispatch.h
/// @see @ref dispatchMsgTypeIsPolymorphic()
/// @see @ref dispatchMsgTypeIsStaticBinSearch()
template <typename TAllMessages, typename TId, typename THandler>
bool dispatchMsgType(TId&& id, THandler& handler)
{
    return details::DispatchMsgHelper<TAllMessages>::dispatchMsgType(std::forward<TId>(id), handler); 
}

/// @brief Dispatch message id into appropriate @b handle() function in the
///     provided handler using either "polymorphic" or "static binary search" behavior.
/// @details The function performs compile time evaluation of the provided @b TAllMessages
///     tuple and uses logic described in @ref page_dispatch_message_object_default
///     to choose the way to dispatch.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @param[in] id ID of the message known at runtime.
/// @param[in] index Index (or offset) of the message type among those having the same ID.
/// @param[in] handler Handler object, it's required public interface
///     is explained in @ref page_dispatch_message_type section of the 
///     @ref page_dispatch tutorial page
/// @return @b true in case the appropriate @b handle() member function of the
///     handler object has been called, @b false otherwise.
/// @note Defined in comms/dispatch.h
/// @see @ref dispatchMsgTypeIsPolymorphic()
/// @see @ref dispatchMsgTypeIsStaticBinSearch()
template <typename TAllMessages, typename TId, typename THandler>
bool dispatchMsgType(TId&& id, std::size_t index, THandler& handler)
{
    return details::DispatchMsgHelper<TAllMessages>::dispatchMsgType(std::forward<TId>(id), index, handler);
}

/// @brief Compile time check whether the @ref dispatchMsgType()
///     will use "polymorphic" dispatch for provided
///     tuple of messages.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @note Defined in comms/dispatch.h
template <typename TAllMessages>
constexpr bool dispatchMsgTypeIsPolymorphic()
{
    return details::DispatchMsgHelper<TAllMessages>::isPolymorphic(); 
}

/// @brief Compile time check whether the @ref dispatchMsg()
///     will use "polymorphic" (including "direct") dispatch for provided
///     tuple of messages and handler.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @tparam TMsg Type of the common message interface.
/// @tparam THandler Type of the handler being used for dispatch.
/// @note Will return @b true in case @ref dispatchMsgIsDirect() returns true;
/// @note Defined in comms/dispatch.h
template <typename TAllMessages, typename TMsg, typename THandler>
constexpr bool dispatchMsgIsPolymorphic()
{
    return dispatchMsgIsDirect<TMsg, THandler>() || dispatchMsgTypeIsPolymorphic<TAllMessages>();
}

/// @brief Similar to other @ref dispatchMsgIsPolymorphic(), but
///     can help in deducing template arguments.
/// @param msg Reference to the message object via its interface class.
/// @param handler Reference to the handler object.
/// @note The result can be evaluated at compile time, but compilers
///     don't support usage of this form in static_assert.
/// @note Defined in comms/dispatch.h
template <typename TAllMessages, typename TMsg, typename THandler>
constexpr bool dispatchMsgIsPolymorphic(TMsg&& msg, THandler&& handler)
{
    return dispatchMsgIsPolymorphic<TAllMessages, typename std::decay<decltype(msg)>::type, typename std::decay<decltype(handler)>::type>();
}

/// @brief Compile time check whether the @ref dispatchMsgType() or
///     @ref dispatchMsgType() will use "static binary search" dispatch for provided
///     tuple of messages.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @note Defined in comms/dispatch.h
template <typename TAllMessages>
constexpr bool dispatchMsgTypeIsStaticBinSearch()
{
    return details::DispatchMsgHelper<TAllMessages>::isStaticBinSearch(); 
}

/// @brief Compile time check whether the @ref dispatchMsg()
///     will use "static binary search" dispatch for provided
///     tuple of messages and handler.
/// @tparam TAllMessages @b std::tuple of supported message classes, sorted in
///     ascending order by their numeric IDs.
/// @tparam TMsg Type of the common message interface.
/// @tparam THandler Type of the handler being used for dispatch.
/// @note Will return @b false in case @ref dispatchMsgIsDirect() returns true;
/// @note Defined in comms/dispatch.h
template <typename TAllMessages, typename TMsg, typename THandler>
constexpr bool dispatchMsgIsStaticBinSearch()
{
    return (!dispatchMsgIsDirect<TMsg, THandler>()) &&
            dispatchMsgTypeIsStaticBinSearch<TAllMessages>();
}

/// @brief Similar to other @ref dispatchMsgIsStaticBinSearch(), but
///     can help in deducing template arguments.
/// @param msg Reference to the message object via its interface class.
/// @param handler Reference to the handler object.
/// @note The result can be evaluated at compile time, but compilers
///     don't support usage of this form in static_assert.
/// @note Defined in comms/dispatch.h
template <typename TAllMessages, typename TMsg, typename THandler>
constexpr bool dispatchMsgIsStaticBinSearch(TMsg&& msg, THandler&& handler)
{
    return dispatchMsgIsStaticBinSearch<TAllMessages, typename std::decay<decltype(msg)>::type, typename std::decay<decltype(handler)>::type>();
}

} // namespace comms
