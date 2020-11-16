/*
 * Copyright (c) [2020] Huawei Technologies Co.,Ltd.All rights reserved.
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

#ifndef MAPLEALL_MAPLEBE_INCLUDE_CG_AARCH64_AARCH64OPTIMIZECOMMON_H
#define MAPLEALL_MAPLEBE_INCLUDE_CG_AARCH64_AARCH64OPTIMIZECOMMON_H

#include "riscv64_isa.h"
#include "optimize_common.h"

namespace maplebe {

using namespace maple;
using namespace std;

class Riscv64InsnVisitor : public InsnVisitor {
 public:
  Riscv64InsnVisitor(CGFunc *func) : InsnVisitor(func) {}

  ~Riscv64InsnVisitor() {}

  void ModifyJumpTarget(maple::LabelIdx targetLabel, BB *&bb) override;
  void ModifyJumpTarget(Operand *targetOperand, BB *&bb) override;
  void ModifyJumpTarget(BB *newTarget, BB *&bb) override;
  // return true if successfully modified
  bool ModifyBrInsn(maple::LabelIdx targetLabel, BB *&curbb) override;
  // Check if it requires to add extra gotos when relocate bb
  MOperator FlipConditionOp(MOperator flippedOp, int &targetIdx) override;

  bool IsConditionAlwaysHold(Insn *cmpInsn, Insn *conditionBrInsn, Operand *operands[]) override;
  Insn *CreateGotoInsn(Insn *condBrInsn) override;
  Insn *CloneInsn(Insn *originalInsn) override;
  LabelIdx GetJumpLabel(Insn *insn) override;
  Operand *GetStrTarget(Insn *insn) override;
  Operand *GetStrSource(Insn *insn) override;
  Operand *GetLdrTarget(Insn *insn) override;
  Operand *GetLdrSource(Insn *insn) override;
  bool EqualMemOperands(Operand *op1, Operand *op2) override;
  bool IsCompareInsn(Insn *insn) override;
  bool IsCompareAndBranchInsn(Insn *insn) override;
  Insn *CreateLdrInsn(MOperator mop, RegOperand *reg, MemOperand *mem) override;
  Insn *CreateMoveInsn(RegOperand *reg, RegOperand *mem) override;
  Insn *CreateCmpInsn(Insn *condbr) override;
  RegOperand *CreateVregFromReg(RegOperand *pReg) override;
  bool ModifyInsnOpnds(Insn *insn, Operand *src, Operand *tar) override;

 private:
  bool IsConditionAlwaysHold(MOperator mop, int64 cmpRet);
};

}  // namespace maplebe

#endif /* MAPLEALL_MAPLEBE_INCLUDE_CG_AARCH64_AARCH64OPTIMIZECOMMON_H */
