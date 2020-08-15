// Generated by commsdsl2comms v3.4.0

/// @file
/// @brief Contains common template parameters independent functionality of
///    @ref mqttsn::message::Willmsgupd message and its fields.

#pragma once

#include "mqttsn/field/WillMsgCommon.h"

namespace mqttsn
{

namespace message
{

/// @brief Common types and functions for fields of 
///     @ref mqttsn::message::Willmsgupd message.
/// @see mqttsn::message::WillmsgupdFields
struct WillmsgupdFieldsCommon
{
    /// @brief Common types and functions for
    ///     @ref mqttsn::message::WillmsgupdFields::WillMsg field.
    using WillMsgCommon = mqttsn::field::WillMsgCommon;
    
};

/// @brief Common types and functions of 
///     @ref mqttsn::message::Willmsgupd message.
/// @see mqttsn::message::Willmsgupd
struct WillmsgupdCommon
{
    /// @brief Name of the @ref mqttsn::message::Willmsgupd message.
    static const char* name()
    {
        return "WILLMSGUPD";
    }
    
};

} // namespace message

} // namespace mqttsn


