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

#ifndef MAPLE_ME_INCLUDE_ME_SYM_RENAME_H
#define MAPLE_ME_INCLUDE_ME_SYM_RENAME_H
#include "me_option.h"
#include "me_irmap.h"

namespace maple {

class SymRename {
 public:
  explicit SymRename(MeFunction *f) : func(f), ssaTab(f->meSSATab),
      irMap(f->irMap), mp(mempoolctrler.NewMemPool(PhaseName().c_str())), alloc(mp) {}
  ~SymRename() { mempoolctrler.DeleteMemPool(mp); }

  void PerformSymRename();

 private:
  MeFunction *func;
  SSATab *ssaTab;
  IRMap *irMap;
  MemPool *mp;
  MapleAllocator alloc;
  uint32 numNewOriginalSts = 0;         // count of newly created OriginalSt's

  bool IsSymRenameCand(OriginalSt *ost) {
    if (ost->indirectLev != 0 || !ost->isLocal)
      return false;
    ASSERT(ost->IsSymbol(), "IsSymRenameCand: unexpected ost_type");
    if (// !func->placementRCOn &&
        GlobalTables::GetTypeTable().GetTypeFromTyIdx(ost->tyIdx)->GetPrimType() == PTY_ref) {
      // cannot rename localrefvars because it conflicts with analyzerc
      return false;
    }
    return true;
  }

  std::string PhaseName() const {
    return "symrename";
  }
};

class MeDoSymRename : public MeFuncPhase {
 public:
  MeDoSymRename(MePhaseID id) : MeFuncPhase(id) {}

  AnalysisResult *Run(MeFunction *ir, MeFuncResultMgr *m) override;
  std::string PhaseName() const override {
    return "symrename";
  }
};
}  // namespace maple
#endif  // MAPLE_ME_INCLUDE_ME_SYM_RENAME_H
