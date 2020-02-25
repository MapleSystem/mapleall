/*
 * Copyright (c) [2020] Huawei Technologies Co.,Ltd.All rights reserved.
 *
 * OpenArkCompiler is licensed under the Mulan PSL v1.
 * You can use this software according to the terms and conditions of the Mulan PSL v1.
 * You may obtain a copy of Mulan PSL v1 at:
 *
 *     http://license.coscl.org.cn/MulanPSL
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v1 for more details.
 */

#ifndef MAPLEBE_INCLUDE_CG_ARMCG_H_
#define MAPLEBE_INCLUDE_CG_ARMCG_H_

#include "cg.h"
#include "arm_cg_func.h"
#include "arm_reg_alloc.h"

namespace maplebe {

using namespace std;

class ArmCG : public CG {
 public:
  explicit ArmCG(option_flag_t opts, bool run_cg, const char *output) : CG(opts, run_cg, output) {}

  CGFunc *CreateCGFunc(MIRFunction *mirFunc, BECommon &bec, MemPool *mp, MapleAllocator *mallocator) {
    return mp->New<ArmCGFunc>(this, mirFunc, &bec, mp, mallocator);
  }

  MemLayout *CreateMemLayout(BECommon &becommon, MIRFunction *mirFunc, MemPool *mp, MapleAllocator *allocator) {
    return mp->New<ArmMemLayout>(&becommon, mirFunc, allocator);
  }

  RegAllocator *NewRegAllocator(CGFunc *cgfunc, MemPool *mp, MapleAllocator *mallocator) {
    return mp->New<ArmRegAllocator>(cgfunc, mallocator);
  }

  Insn *NewInsn0(MOperator opcode);
  Insn *NewInsn1(MOperator opcode, Operand *opnd0);
  Insn *NewInsn2(MOperator opcode, Operand *opnd0, Operand *opnd1);
  Insn *NewInsn3(MOperator opcode, Operand *opnd0, Operand *opnd1, Operand *opnd2);
  Insn *NewInsn4(MOperator opcode, Operand *opnd0, Operand *opnd1, Operand *opnd2, Operand *opnd3);
  Insn *NewInsn5(MOperator opcode, Operand *opnd0, Operand *opnd1, Operand *opnd2, Operand *opnd3, Operand *opnd4);

 public:
  static const ARMMD thearmmachine[MOP_last];
  static const char *int_reg_name1[R15 + 1];
  static const char *int_reg_name2[R15 + 1];
  static const char *int_reg_name4[MAXREG];
  static const char *int_reg_name8[MAXREG];
};

}  // namespace maplebe

#endif  // MAPLEBE_INCLUDE_CG_ARMCG_H_