// Generated by commsdsl2comms v3.4.0

/// @file
/// @brief Contains common template parameters independent functionality of
///    @ref mqttsn::message::Willtopic message and its fields.

#pragma once

#include "mqttsn/field/FlagsCommon.h"
#include "mqttsn/field/WillTopicCommon.h"

namespace mqttsn
{

namespace message
{

/// @brief Common types and functions for fields of 
///     @ref mqttsn::message::Willtopic message.
/// @see mqttsn::message::WilltopicFields
struct WilltopicFieldsCommon
{
    /// @brief Scope for all the common definitions of the member fields of
    ///     @ref mqttsn::message::WilltopicFields::Flags field.
    struct FlagsMembersCommon
    {
        /// @brief Common types and functions for members of
        ///     @ref mqttsn::message::WilltopicFields::FlagsMembers::FlagsField field.
        using FlagsFieldMembersCommon = mqttsn::field::FlagsMembersCommon;
        
        /// @brief Common types and functions for
        ///     @ref mqttsn::message::WilltopicFields::FlagsMembers::FlagsField field.
        struct FlagsFieldCommon : public mqttsn::field::FlagsCommon
        {
            /// @brief Name of the @ref mqttsn::message::WilltopicFields::FlagsMembers::FlagsField field.
            static const char* name()
            {
                return "FlagsField";
            }
            
        };
        
    };
    
    /// @brief Scope for all the common definitions of the
    ///     @ref mqttsn::message::WilltopicFields::Flags field.
    struct FlagsCommon
    {
        /// @brief Name of the @ref mqttsn::message::WilltopicFields::Flags field.
        static const char* name()
        {
            return "Flags";
        }
        
    };
    
    
    /// @brief Common types and functions for
    ///     @ref mqttsn::message::WilltopicFields::WillTopic field.
    using WillTopicCommon = mqttsn::field::WillTopicCommon;
    
};

/// @brief Common types and functions of 
///     @ref mqttsn::message::Willtopic message.
/// @see mqttsn::message::Willtopic
struct WilltopicCommon
{
    /// @brief Name of the @ref mqttsn::message::Willtopic message.
    static const char* name()
    {
        return "WILLTOPIC";
    }
    
};

} // namespace message

} // namespace mqttsn

