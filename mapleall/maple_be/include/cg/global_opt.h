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

#ifndef MAPLEBE_INCLUDE_CG_GLOBALOPT_H
#define MAPLEBE_INCLUDE_CG_GLOBALOPT_H

#include "cg_func.h"
#include "insn.h"
#include "cg_phase.h"
#include "store_load_opt.h"

namespace maplebe {

class LiveAnalysis;

class GlobalOpt {
 public:
  CGFunc *cgfunc;
  MemPool *gomp;

 public:
  GlobalOpt(CGFunc *func, MemPool *mp) : cgfunc(func), gomp(mp) {}

  virtual ~GlobalOpt() {}

  virtual void Run() {}

  const char *PhaseName() const {
    return "globalopt";
  }
};

CGFUNCPHASE_CANSKIP(CgDoGlobalOpt, "globalopt")

}  // namespace maplebe

#endif  // MAPLEBE_INCLUDE_CG_GLOBALOPT_H