//
// Copyright 2016 - 2020 (C). Alex Robenko. All rights reserved.
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


#pragma once

#include <tuple>
#include <type_traits>

#include "macro_common.h"
#include "gen_enum.h"
#include "base_detection.h"

#ifdef COMMS_MUST_DEFINE_BASE
#define COMMS_FIELD_VALUE_ACCESS_FUNC typename Base::ValueType& value()
#define COMMS_FIELD_VALUE_ACCESS_CONST_FUNC const typename Base::ValueType& value() const
#define COMMS_ACCESS_MEMBER_FIELD_FUNC(T_, t_, n_) \
    typename std::tuple_element<COMMS_CONCATENATE(FieldIdx_, n_), typename Base::T_>::type& COMMS_CONCATENATE(field_, n_)()
#define COMMS_ACCESS_MEMBER_FIELD_CONST_FUNC(T_, t_, n_) \
    const typename std::tuple_element<COMMS_CONCATENATE(FieldIdx_, n_), typename Base::T_>::type& COMMS_CONCATENATE(field_, n_)() const
#define COMMS_MSG_FIELDS_ACCESS_FUNC \
    typename Base::AllFields& fields()
#define COMMS_MSG_FIELDS_ACCESS_CONST_FUNC \
    const typename Base::AllFields& fields() const

#else // #ifdef COMMS_MUST_DEFINE_BASE
#define COMMS_FIELD_VALUE_ACCESS_FUNC FUNC_AUTO_REF_RETURN(value, decltype(comms::field::toFieldBase(*this).value()))
#define COMMS_FIELD_VALUE_ACCESS_CONST_FUNC FUNC_AUTO_REF_RETURN_CONST(value, decltype(comms::field::toFieldBase(*this).value()))
#define COMMS_ACCESS_MEMBER_FIELD_FUNC(T_, t_, n_) FUNC_AUTO_REF_RETURN(COMMS_CONCATENATE(field_, n_), decltype(std::get<COMMS_CONCATENATE(FieldIdx_, n_)>(t_)))
#define COMMS_ACCESS_MEMBER_FIELD_CONST_FUNC(T_, t_, n_) FUNC_AUTO_REF_RETURN_CONST(COMMS_CONCATENATE(field_, n_), decltype(std::get<COMMS_CONCATENATE(FieldIdx_, n_)>(t_)))
#define COMMS_MSG_FIELDS_ACCESS_FUNC FUNC_AUTO_REF_RETURN(fields, decltype(comms::toMessageBase(*this).fields()))
#define COMMS_MSG_FIELDS_ACCESS_CONST_FUNC FUNC_AUTO_REF_RETURN_CONST(fields, decltype(comms::toMessageBase(*this).fields()))
#endif // #ifdef COMMS_MUST_DEFINE_BASE

#define COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_ACCESS_MEMBER_FIELD_FUNC(T_, t_, n_) {\
        return std::get<COMMS_CONCATENATE(FieldIdx_, n_)>(t_); \
    } \
    COMMS_ACCESS_MEMBER_FIELD_CONST_FUNC(T_, t_, n_) {\
        return std::get<COMMS_CONCATENATE(FieldIdx_, n_)>(t_); \
    }

#define COMMS_FIELD_ACC_FUNC_1(T_, t_, n_) COMMS_FIELD_ACC_FUNC(T_, t_, n_)
#define COMMS_FIELD_ACC_FUNC_2(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_1(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_3(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_2(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_4(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_3(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_5(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_4(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_6(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_5(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_7(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_6(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_8(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_7(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_9(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_8(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_10(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_9(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_11(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_10(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_12(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_11(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_13(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_12(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_14(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_13(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_15(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_14(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_16(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_15(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_17(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_16(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_18(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_17(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_19(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_18(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_20(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_19(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_21(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_20(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_22(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_21(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_23(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_22(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_24(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_23(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_25(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_24(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_26(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_25(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_27(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_26(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_28(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_27(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_29(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_28(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_30(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_29(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_31(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_30(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_32(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_31(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_33(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_32(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_34(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_33(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_35(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_34(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_36(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_35(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_37(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_36(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_38(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_37(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_39(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_38(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_40(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_39(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_41(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_40(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_42(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_41(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_43(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_42(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_44(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_43(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_45(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_44(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_46(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_45(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_47(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_46(T_, t_, __VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_48(T_, t_, n_, ...) \
    COMMS_FIELD_ACC_FUNC(T_, t_, n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_47(T_, t_, __VA_ARGS__))

#define COMMS_CHOOSE_FIELD_ACC_FUNC_(N, T_, t_, ...) COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_ ## N(T_, t_, __VA_ARGS__))
#define COMMS_CHOOSE_FIELD_ACC_FUNC(N, T_, t_, ...) COMMS_EXPAND(COMMS_CHOOSE_FIELD_ACC_FUNC_(N, T_, t_, __VA_ARGS__))
#define COMMS_DO_FIELD_ACC_FUNC(T_, t_, ...) \
    COMMS_EXPAND(COMMS_CHOOSE_FIELD_ACC_FUNC(COMMS_NUM_ARGS(__VA_ARGS__), T_, t_, __VA_ARGS__))

#define COMMS_DEFINE_FIELD_ENUM(...) COMMS_EXPAND(COMMS_DEFINE_ENUM(FieldIdx, __VA_ARGS__));

// ----------------------------------------------

#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    typename std::tuple_element<COMMS_CONCATENATE(FieldIdx_, n_), ValueType>::type& COMMS_CONCATENATE(field_, n_)() {\
        return std::get<COMMS_CONCATENATE(FieldIdx_, n_)>(value()); \
    } \
    const typename std::tuple_element<COMMS_CONCATENATE(FieldIdx_, n_), ValueType>::type& COMMS_CONCATENATE(field_, n_)() const {\
        return std::get<COMMS_CONCATENATE(FieldIdx_, n_)>(value()); \
    }

#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_1(n_) COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_)
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_2(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_1(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_3(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_2(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_4(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_3(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_5(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_4(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_6(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_5(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_7(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_6(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_8(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_7(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_9(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_8(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_10(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_9(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_11(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_10(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_12(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_11(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_13(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_12(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_14(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_13(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_15(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_14(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_16(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_15(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_17(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_16(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_18(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_17(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_19(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_18(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_20(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_19(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_21(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_20(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_22(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_21(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_23(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_22(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_24(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_23(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_25(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_24(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_26(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_25(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_27(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_26(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_28(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_27(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_29(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_28(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_30(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_29(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_31(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_30(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_32(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_31(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_33(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_32(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_34(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_33(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_35(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_34(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_36(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_35(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_37(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_36(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_38(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_37(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_39(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_38(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_40(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_39(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_41(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_40(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_42(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_41(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_43(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_42(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_44(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_43(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_45(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_44(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_46(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_45(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_47(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_46(__VA_ARGS__))
#define COMMS_FIELD_ACC_FUNC_NOTEMPLATE_48(n_, ...) \
    COMMS_FIELD_ACC_FUNC_NOTEMPLATE(n_) \
    COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_47(__VA_ARGS__))

#define COMMS_CHOOSE_FIELD_ACC_FUNC_NOTEMPLATE_(N, ...) COMMS_EXPAND(COMMS_FIELD_ACC_FUNC_NOTEMPLATE_ ## N(__VA_ARGS__))
#define COMMS_CHOOSE_FIELD_ACC_FUNC_NOTEMPLATE(N, ...) COMMS_EXPAND(COMMS_CHOOSE_FIELD_ACC_FUNC_NOTEMPLATE_(N, __VA_ARGS__))
#define COMMS_DO_FIELD_ACC_FUNC_NOTEMPLATE(...) \
    COMMS_EXPAND(COMMS_CHOOSE_FIELD_ACC_FUNC_NOTEMPLATE(COMMS_NUM_ARGS(__VA_ARGS__), __VA_ARGS__))


// ----------------------------------------------

#define COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    using COMMS_CONCATENATE(P_, n_) = typename std::tuple_element<COMMS_CONCATENATE(pref_, n_), T_>::type;

#define COMMS_FIELD_TYPEDEF_1(T_, P_, pref_, n_) COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_)
#define COMMS_FIELD_TYPEDEF_2(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_1(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_3(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_2(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_4(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_3(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_5(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_4(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_6(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_5(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_7(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_6(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_8(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_7(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_9(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_8(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_10(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_9(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_11(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_10(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_12(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_11(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_13(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_12(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_14(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_13(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_15(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_14(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_16(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_15(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_17(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_16(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_18(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_17(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_19(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_18(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_20(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_19(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_21(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_20(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_22(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_21(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_23(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_22(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_24(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_23(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_25(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_24(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_26(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_25(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_27(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_26(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_28(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_27(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_29(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_28(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_30(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_29(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_31(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_30(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_32(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_31(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_33(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_32(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_34(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_33(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_35(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_34(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_36(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_35(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_37(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_36(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_38(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_37(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_39(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_38(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_40(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_39(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_41(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_40(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_42(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_41(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_43(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_42(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_44(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_43(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_45(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_44(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_46(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_45(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_47(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_46(T_, P_, pref_, __VA_ARGS__))
#define COMMS_FIELD_TYPEDEF_48(T_, P_, pref_, n_, ...) \
    COMMS_FIELD_TYPEDEF(T_, P_, pref_, n_) \
    COMMS_EXPAND(COMMS_FIELD_TYPEDEF_47(T_, P_, pref_, __VA_ARGS__))

#define COMMS_CHOOSE_FIELD_TYPEDEF_(N, T_, P_, pref_, ...) COMMS_EXPAND(COMMS_FIELD_TYPEDEF_ ## N(T_, P_, pref_, __VA_ARGS__))
#define COMMS_CHOOSE_FIELD_TYPEDEF(N, T_, P_, pref_, ...) COMMS_EXPAND(COMMS_CHOOSE_FIELD_TYPEDEF_(N, T_, P_, pref_, __VA_ARGS__))
#define COMMS_DO_FIELD_TYPEDEF(T_, P_, pref_, ...) \
    COMMS_EXPAND(COMMS_CHOOSE_FIELD_TYPEDEF(COMMS_NUM_ARGS(__VA_ARGS__), T_, P_, pref_, __VA_ARGS__))

