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

/// Copyright [year] <Copyright Owner>

#ifndef MAPLE_IR_INCLUDE_MIR_PRAGMA_H
#define MAPLE_IR_INCLUDE_MIR_PRAGMA_H

#include "types_def.h"
#include "prim_types.h"
#include "mir_module.h"
#include "../../maple_util/include/mpl_logging.h"
#include "../../mempool/include/mempool_allocator.h"

using namespace maple;

namespace maple {

class MIRModule;
class MIRType;
class MIRFunction;
class MIRSymbol;
class MIRSymbolTable;
class MIRTypeNameTable;

enum PragmaKind {
  kPragmaUnknown,
  kPragmaClass,
  kPragmaFunc,
  kPragmaField,
  kPragmaParam,
  kPragmaPkg,
  kPragmaVar,
  kPragmaGlbvar,
  kPragmaFuncExecptioni,
  kPragmaFuncVar,
};

enum PragmaVisibility {
  kVisBuild,
  kVisRuntime,
  kVisSystem,
  kVisMaple,
};

enum PragmaValueType {
  kValueByte = 0x00,          //(none; must be 0)  ubyte[1]
  kValueShort = 0x02,         // size - 1 (0…1)  ubyte[size]
  kValueChar = 0x03,          // size - 1 (0…1)  ubyte[size]
  kValueInt = 0x04,           // size - 1 (0…3)  ubyte[size]
  kValueLong = 0x06,          // size - 1 (0…7)  ubyte[size]
  kValueFloat = 0x10,         // size - 1 (0…3)  ubyte[size]
  kValueDouble = 0x11,        // size - 1 (0…7)  ubyte[size]
  kValueMethodType = 0x15,    // size - 1 (0…3)  ubyte[size]
  kValueMethodHandle = 0x16,  // size - 1 (0…3)  ubyte[size]
  kValueString = 0x17,        // size - 1 (0…3)  ubyte[size]
  kValueType = 0x18,          // size - 1 (0…3)  ubyte[size]
  kValueField = 0x19,         // size - 1 (0…3)  ubyte[size]
  kValueMethod = 0x1a,        // size - 1 (0…3)  ubyte[size]
  kValueEnum = 0x1b,          // size - 1 (0…3)  ubyte[size]
  kValueArray = 0x1c,         //(none; must be 0) encoded_array
  kValueAnnotation = 0x1d,    //(none; must be 0) encoded_annotation
  kValueNull = 0x1e,          //(none; must be 0) (none)
  kValueBoolean = 0x1f        // boolean (0…1)   (none)
};

class MIRPragmaElement {
 public:
  MIRModule *mod;
  GStrIdx namestridx_;
  GStrIdx typestridx_;
  PragmaValueType type_;
  union {
    int32 i;
    int64 j;
    uint64 u;
    float f;
    double d;
  } val_;
  MapleVector<MIRPragmaElement *> subelemvec_;
  MIRPragmaElement(MIRModule *m)
    : mod(m), namestridx_(0), typestridx_(0), type_(kValueNull), subelemvec_(m->memPoolAllocator.Adapter()) {
    val_.u = 0;
    subelemvec_.clear();
  }

  void Dump(int indent);
};

class MIRPragma {
 public:
  MIRModule *mod;
  PragmaKind pragmaKind;
  uint8 visibility;
  GStrIdx strIdx;
  TyIdx tyIdx;
  TyIdx tyIdxEx;
  int32 paramNum;  // paramNum th param in function, -1 not for param annotation
  MapleVector<MIRPragmaElement *> elementVec;

 public:
  MIRPragma(MIRModule *m)
    : mod(m),
      pragmaKind(kPragmaUnknown),
      visibility(0),
      strIdx(0),
      tyIdx(0),
      tyIdxEx(0),
      paramNum(-1),
      elementVec(m->memPoolAllocator.Adapter()) {}

  MIRPragmaElement *GetPragmaElemFromSignature(const std::string signature);
  void Dump(int indent);
};

}  // namespace maple

#endif  // MAPLE_IR_INCLUDE_MIR_PRAGMA_H
