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

#ifndef MAPLE_ME_INCLUDE_ME_MAY_TO_DASSIGN_H
#define MAPLE_ME_INCLUDE_ME_MAY_TO_DASSIGN_H
#include "me_function.h"
#include "me_phase.h"
#include "me_irmap.h"

namespace maple {
class May2Dassign {
 public:
  May2Dassign(MeFunction *f) : func(f), irMap(f->irMap) {}

  bool Doit();  // return true if there is change
  std::string PhaseName() const {
    return "may2dassign";
  }

 private:
  MeFunction *func;
  IRMap *irMap;
};

class MeDoMay2Dassign : public MeFuncPhase {
 public:
  MeDoMay2Dassign(MePhaseID id) : MeFuncPhase(id) {}

  AnalysisResult *Run(MeFunction *func, MeFuncResultMgr *m) override;
  std::string PhaseName() const override {
    return "may2dassign";
  }
};
}  // namespace maple
#endif  // MAPLE_ME_INCLUDE_ME_MAY_TO_DASSIGN_H
