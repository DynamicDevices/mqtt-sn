// Generated by commsdsl2comms v3.4.0

/// @file
/// @brief Contains definition of <b>"ReturnCode"</b> field.

#pragma once

#include <cstdint>
#include <type_traits>
#include "comms/field/EnumValue.h"
#include "comms/options.h"
#include "mqttsn/field/FieldBase.h"
#include "mqttsn/field/ReturnCodeCommon.h"
#include "mqttsn/options/DefaultOptions.h"

namespace mqttsn
{

namespace field
{

/// @brief Definition of <b>"ReturnCode"</b> field.
/// @see @ref mqttsn::field::ReturnCodeVal
/// @tparam TOpt Protocol options.
/// @tparam TExtraOpts Extra options.
template <typename TOpt = mqttsn::options::DefaultOptions, typename... TExtraOpts>
class ReturnCode : public
    comms::field::EnumValue<
        mqttsn::field::FieldBase<>,
        mqttsn::field::ReturnCodeVal,
        TExtraOpts...,
        comms::option::def::ValidNumValueRange<0, 3>
    >
{
    using Base = 
        comms::field::EnumValue<
            mqttsn::field::FieldBase<>,
            mqttsn::field::ReturnCodeVal,
            TExtraOpts...,
            comms::option::def::ValidNumValueRange<0, 3>
        >;
public:
    /// @brief Re-definition of the value type.
    using ValueType = typename Base::ValueType;

    /// @brief Name of the field.
    static const char* name()
    {
        return mqttsn::field::ReturnCodeCommon::name();
    }
    
    /// @brief Retrieve name of the enum value
    static const char* valueName(ValueType val)
    {
        return mqttsn::field::ReturnCodeCommon::valueName(val);
    }
    
    /// @brief Retrieve name of the @b current value
    const char* valueName() const
    {
        return valueName(Base::value());
    }
    
};

} // namespace field

} // namespace mqttsn


