// Generated by commsdsl2comms v3.4.0

/// @file
/// @brief Contains definition of <b>"SEARCHGW"</b> message and its fields.

#pragma once

#include <tuple>
#include "comms/MessageBase.h"
#include "comms/options.h"
#include "mqttsn/MsgId.h"
#include "mqttsn/field/FieldBase.h"
#include "mqttsn/field/Radius.h"
#include "mqttsn/message/SearchgwCommon.h"
#include "mqttsn/options/DefaultOptions.h"

namespace mqttsn
{

namespace message
{

/// @brief Fields of @ref Searchgw.
/// @tparam TOpt Extra options
/// @see @ref Searchgw
/// @headerfile "mqttsn/message/Searchgw.h"
template <typename TOpt = mqttsn::options::DefaultOptions>
struct SearchgwFields
{
    /// @brief Definition of <b>"Radius"</b> field.
    using Radius =
        mqttsn::field::Radius<
            TOpt
        >;
    
    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        Radius
    >;
};

/// @brief Definition of <b>"SEARCHGW"</b> message class.
/// @details
///     See @ref SearchgwFields for definition of the fields this message contains.
/// @tparam TMsgBase Base (interface) class.
/// @tparam TOpt Extra options
/// @headerfile "mqttsn/message/Searchgw.h"
template <typename TMsgBase, typename TOpt = mqttsn::options::DefaultOptions>
class Searchgw : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::Searchgw,
        comms::option::def::StaticNumIdImpl<mqttsn::MsgId_Searchgw>,
        comms::option::def::FieldsImpl<typename SearchgwFields<TOpt>::All>,
        comms::option::def::MsgType<Searchgw<TMsgBase, TOpt> >,
        comms::option::def::HasName
    >
{
    // Redefinition of the base class type
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::Searchgw,
            comms::option::def::StaticNumIdImpl<mqttsn::MsgId_Searchgw>,
            comms::option::def::FieldsImpl<typename SearchgwFields<TOpt>::All>,
            comms::option::def::MsgType<Searchgw<TMsgBase, TOpt> >,
            comms::option::def::HasName
        >;

public:
    /// @brief Provide names and allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    ///     The generated types and functions are:
    ///     @li @b Field_radius type and @b field_radius() access fuction
    ///         for @ref SearchgwFields::Radius field.
    COMMS_MSG_FIELDS_NAMES(
        radius
    );
    
    // Compile time check for serialisation length.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static const std::size_t MsgMaxLen = Base::doMaxLength();
    static_assert(MsgMinLen == 1U, "Unexpected min serialisation length");
    static_assert(MsgMaxLen == 1U, "Unexpected max serialisation length");
    
    /// @brief Name of the message.
    static const char* doName()
    {
        return mqttsn::message::SearchgwCommon::name();
    }
    
    
};

} // namespace message

} // namespace mqttsn

