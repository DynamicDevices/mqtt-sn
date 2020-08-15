// Generated by commsdsl2comms v3.4.0

/// @file
/// @brief Contains definition of <b>"TopicId"</b> field.

#pragma once

#include <cstdint>
#include "comms/field/IntValue.h"
#include "comms/options.h"
#include "mqttsn/field/FieldBase.h"
#include "mqttsn/field/TopicIdCommon.h"
#include "mqttsn/options/DefaultOptions.h"

namespace mqttsn
{

namespace field
{

/// @brief Definition of <b>"TopicId"</b> field.
/// @tparam TOpt Protocol options.
/// @tparam TExtraOpts Extra options.
template <typename TOpt = mqttsn::options::DefaultOptions, typename... TExtraOpts>
struct TopicId : public
    comms::field::IntValue<
        mqttsn::field::FieldBase<>,
        std::uint16_t,
        TExtraOpts...,
        comms::option::def::ValidNumValueRange<0, 65534L>
    >
{
    /// @brief Name of the field.
    static const char* name()
    {
        return mqttsn::field::TopicIdCommon::name();
    }
    
};

} // namespace field

} // namespace mqttsn


