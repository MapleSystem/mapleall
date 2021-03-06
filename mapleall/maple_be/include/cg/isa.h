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

#ifndef MAPLEBE_INCLUDE_CG_ISA_H
#define MAPLEBE_INCLUDE_CG_ISA_H

#include <cstdint>

namespace maplebe {

enum RegType {
  kRegTyUndef,
  kRegTyInt,
  kRegTyFloat,
  kRegTyCc,
  kRegTyX87,
  kRegTyVary,
  kRegTyLast,
};

enum MopProperty {
  kPropIsMove = 0,
  kPropIsBranch,
  kPropIsLoad,
  kPropIsLoadPair,
  kPropIsStore,
  kPropIsStorePair,
  kPropIsLoadAddress,
  kPropIsAtomic,
  kPropIsCall,
  kPropIsConversion,
  kPropIsConditionalSet,
  kPropUseSpecReg,
  kPropIsX86style,
  kPropIsCondDef,
  kPropHasAcqure,
  kPropHasAcqureRCpc,
  kPropHasLOAcqure,
  kPropHasRelease,
  kPropHasLORelease,
  kPropCanThrow,
  kPropIsPartDefine,
  kPropIsDMB,
};

#define ISMOVE (1ULL << kPropIsMove)
#define ISBRANCH (1ULL << kPropIsBranch)
#define ISLOAD (1ULL << kPropIsLoad)
#define ISLOADPAIR (1ULL << kPropIsLoadPair)
#define ISSTORE (1ULL << kPropIsStore)
#define ISSTOREPAIR (1ULL << kPropIsStorePair)
#define ISLOADADDR (1ULL << kPropIsLoadAddress)
#define ISATOMIC (1ULL << kPropIsAtomic)
#define ISCALL (1ULL << kPropIsCall)
#define ISCONVERSION (1ULL << kPropIsConversion)
#define ISCONDSET (1ULL << kPropIsConditionalSet)
#define USESPECREG (1ULL << kPropUseSpecReg)
#define ISX86STYLE (1ULL << kPropIsX86style)
#define ISCONDDEF (1ULL << kPropIsCondDef)
#define HASACQUIRE (1ULL << kPropHasAcqure)
#define HASACQUIRERCPC (1ULL << kPropHasAcqureRCpc)
#define HASLOACQUIRE (1ULL << kPropHasLOAcqure)
#define HASRELEASE (1ULL << kPropHasRelease)
#define HASLORELEASE (1ULL << kPropHasLORelease)
#define CANTHROW (1ULL << kPropCanThrow)
#define ISPARTDEF (1ULL << kPropIsPartDefine)
#define ISDMB (1ULL << kPropIsDMB)

typedef uint32_t regno_t;

#define INVALID_REGNO (0)

/* ARM64 has 32 int registes and 32 FP registers.
 * AMD64/X86_64 has 16 int registes, and 16 FP registers.
 * In either case, the corresponding calling conventions use
 * the smaller number of caller-saved registers.
 * 64 bit is not large enough?
 */
typedef uint64_t CSR_BITSET;

// empty class; just for parameter passing
class OpndProp {};

}  // namespace maplebe

#endif /* MAPLEBE_INCLUDE_CG_ISA_H */
