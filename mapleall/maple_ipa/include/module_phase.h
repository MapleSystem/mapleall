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

#ifndef MAPLE_IPA_INCLUDE_MODULE_PHASE_H
#define MAPLE_IPA_INCLUDE_MODULE_PHASE_H
#include "mir_module.h"
#include "phase.h"

namespace maple {

enum ModulePhaseID {
  kMoPhaseDoNothing,
#define MODAPHASE(MODPHASEID, CLASSNAME) MODPHASEID,
#define MODTPHASE(MODPHASEID, CLASSNAME) MODPHASEID,
#include "module_phases.def"
#undef MODAPHASE
#undef MODTPHASE
  kMoPhaseMax
};

class ModulePhase;
using ModuleResultMgr = AnalysisResultManager<MIRModule, ModulePhaseID, ModulePhase>;

class ModulePhase : public Phase {
 private:
  ModulePhaseID phaseID;

 public:
  explicit ModulePhase(ModulePhaseID id) : Phase(), phaseID(id) {}

  virtual ~ModulePhase() = default;

  virtual AnalysisResult *Run(MIRModule *module, ModuleResultMgr *)  = 0;

  ModulePhaseID GetPhaseId() const {
    return phaseID;
  }

  virtual std::string PhaseName() const = 0;
};

}  // namespace maple
#endif  // MAPLE_IPA_INCLUDE_MODULE_PHASE_H
