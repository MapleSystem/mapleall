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

#ifndef MAPLEBE_INCLUDE_CG_AARCH64IMMEDIATE_H
#define MAPLEBE_INCLUDE_CG_AARCH64IMMEDIATE_H

#include "types_def.h"  // mapleir/include/typedef.h

namespace maplebe {

bool IsBitmaskImmediate(maple::uint64 val, maple::uint32 bitlen);
bool IsMoveWidableImmediate(maple::uint64 val, maple::uint32 bitlen);
bool BetterUseMOVZ(maple::uint64 val);

}  // namespace maplebe

#endif /* MAPLEBE_INCLUDE_CG_AARCH64IMMEDIATE_H */
