// Generated by commsdsl2comms v3.4.0

/// @file
/// @brief Contains definition of <b>"CONNACK"</b> message and its fields.

#pragma once

#include <tuple>
#include "comms/MessageBase.h"
#include "comms/options.h"
#include "mqttsn/MsgId.h"
#include "mqttsn/field/FieldBase.h"
#include "mqttsn/field/ReturnCode.h"
#include "mqttsn/message/ConnackCommon.h"
#include "mqttsn/options/DefaultOptions.h"

namespace mqttsn
{

namespace message
{

/// @brief Fields of @ref Connack.
/// @tparam TOpt Extra options
/// @see @ref Connack
/// @headerfile "mqttsn/message/Connack.h"
template <typename TOpt = mqttsn::options::DefaultOptions>
struct ConnackFields
{
    /// @brief Definition of <b>"ReturnCode"</b> field.
    using ReturnCode =
        mqttsn::field::ReturnCode<
            TOpt
        >;
    
    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        ReturnCode
    >;
};

/// @brief Definition of <b>"CONNACK"</b> message class.
/// @details
///     See @ref ConnackFields for definition of the fields this message contains.
/// @tparam TMsgBase Base (interface) class.
/// @tparam TOpt Extra options
/// @headerfile "mqttsn/message/Connack.h"
template <typename TMsgBase, typename TOpt = mqttsn::options::DefaultOptions>
class Connack : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::Connack,
        comms::option::def::StaticNumIdImpl<mqttsn::MsgId_Connack>,
        comms::option::def::FieldsImpl<typename ConnackFields<TOpt>::All>,
        comms::option::def::MsgType<Connack<TMsgBase, TOpt> >,
        comms::option::def::HasName
    >
{
    // Redefinition of the base class type
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::Connack,
            comms::option::def::StaticNumIdImpl<mqttsn::MsgId_Connack>,
            comms::option::def::FieldsImpl<typename ConnackFields<TOpt>::All>,
            comms::option::def::MsgType<Connack<TMsgBase, TOpt> >,
            comms::option::def::HasName
        >;

public:
    /// @brief Provide names and allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    ///     The generated types and functions are:
    ///     @li @b Field_returnCode type and @b field_returnCode() access fuction
    ///         for @ref ConnackFields::ReturnCode field.
    COMMS_MSG_FIELDS_NAMES(
        returnCode
    );
    
    // Compile time check for serialisation length.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static const std::size_t MsgMaxLen = Base::doMaxLength();
    static_assert(MsgMinLen == 1U, "Unexpected min serialisation length");
    static_assert(MsgMaxLen == 1U, "Unexpected max serialisation length");
    
    /// @brief Name of the message.
    static const char* doName()
    {
        return mqttsn::message::ConnackCommon::name();
    }
    
    
};

} // namespace message

} // namespace mqttsn


