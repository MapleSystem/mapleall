/*
 * Copyright (c) [2020] Huawei Technologies Co., Ltd. All rights reserved.
 *
 * OpenArkCompiler is licensed under the Mulan Permissive Software License v2.
 * You can use this software according to the terms and conditions of the MulanPSL - 2.0.
 * You may obtain a copy of MulanPSL - 2.0 at:
 *
 *   https://opensource.org/licenses/MulanPSL-2.0
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the MulanPSL - 2.0 for more details.
 */

#ifndef MAPLE_IR_INCLUDE_CFG_PRIMITIVE_TYPES_H
#define MAPLE_IR_INCLUDE_CFG_PRIMITIVE_TYPES_H

namespace maple {
// Declaration of enum PrimType
#define LOAD_ALGO_PRIMARY_TYPE
enum PrimType {
  PTY_begin,            // PrimType begin
#define PRIMTYPE(P) PTY_##P,
#include "prim_types.def"
  PTY_end,              // PrimType end
#undef PRIMTYPE
};

constexpr PrimType kPtyInvalid = PTY_begin;
// just for test, no primitive type for derived SIMD types to be defined
constexpr PrimType kPtyDerived = PTY_end;

struct PrimitiveTypeProperty {
  PrimType type;
  bool isInteger;
  bool isUnsigned;
  bool isAddress;
  bool isFloat;
  bool isPointer;
  bool isSimple;
  bool isDynamic;
  bool isDynamicAny;
  bool isDynamicNone;
};

const PrimitiveTypeProperty &GetPrimitiveTypeProperty(PrimType pType);
} // namespace maple
#endif  // MAPLE_IR_INCLUDE_CFG_PRIMITIVE_TYPES_H
