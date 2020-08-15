/// @file
/// @brief Length calculation layer

#pragma once

#include <type_traits>
#include "comms/options.h"
#include "comms/protocol/MsgSizeLayer.h"

namespace mqttsn
{

namespace frame
{

namespace layer
{

/// @brief Custom length calculation layer
/// @tparam TField Used field type
/// @tparam TNextLayer Next frame layer
/// @tparam TExtraOpt Extra options passed to @b comms::MsgFactory
template <
    typename TField,
    typename TNextLayer,
    typename... TExtraOpts>
class Length : public
    comms::protocol::MsgSizeLayer<
        TField,
        TNextLayer,
        comms::option::ExtendingClass<Length<TField, TNextLayer, TExtraOpts...> >
    >
{
    using Base =
        comms::protocol::MsgSizeLayer<
            TField,
            TNextLayer,
            comms::option::ExtendingClass<Length<TField, TNextLayer, TExtraOpts...> >
        >;

public:
    /// @brief Field type is defined in the base class.
    using Field = typename Base::Field;

    /// @brief Retrieve message ID value from the given field
    static std::size_t getRemainingSizeFromField(const Field& field)
    {
        return field.getLengthValue();
    }

    /// @brief Assemble the field's value before its write.
    template <typename TMsg>
    static void prepareFieldForWrite(std::size_t size, const TMsg* msg, Field& field)
    {
        static_cast<void>(msg);
        field.setLengthValue(size);
    }

};

}  // namespace layer

}  // namespace frame

}  // namespace mqttsn


