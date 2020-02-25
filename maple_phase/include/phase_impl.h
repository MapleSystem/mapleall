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

#ifndef MAPLE_PHASE_INCLUDE_PHASE_IMPL_H
#define MAPLE_PHASE_INCLUDE_PHASE_IMPL_H

#include "class_hierarchy.h"
#include "mir_builder.h"
#include "mpl_scheduler.h"

namespace maple {

class FuncOptimizeImpl {
 public:
  MIRModule *module = nullptr;

 protected:
  KlassHierarchy *klassHierarchy = nullptr;
  MIRFunction *currFunc = nullptr;
  MIRBuilderExt *builder = nullptr;
  bool trace = false;

 public:
  explicit FuncOptimizeImpl(MIRModule *mod, KlassHierarchy *kh = nullptr, bool trace = false);
  virtual ~FuncOptimizeImpl();
  // Each phase needs to implement its own Clone
  virtual FuncOptimizeImpl *Clone() = 0;
  inline MIRModule *GetModule() {
    return module;
  }

  virtual void CreateLocalBuilder(pthread_mutex_t *mutex);
  virtual void ProcessFunc(MIRFunction *func);
  virtual void Finish() {}

 protected:
  void SetCurrentFunction(MIRFunction *func) {
    currFunc = func;
    ASSERT(builder, "builder is null in FuncOptimizeImpl::SetCurrentFunction");
    builder->SetCurrentFunction(func);
    module->SetCurFunction(func);
  }
  virtual void ProcessBlock(StmtNode *stmt);
  // Each phase needs to implement its own ProcessStmt
  virtual void ProcessStmt(StmtNode *stmt) {}
};

class FuncOptimizeIterator : public MplScheduler {
 public:
  class Task : public MplTask {
   private:
    MIRFunction *mFunction;

   public:
    Task(MIRFunction *func) : mFunction(func) {}

    ~Task() {}

    virtual int Run(void *param = nullptr) {
      static_cast<FuncOptimizeImpl *>(param)->ProcessFunc(mFunction);
      return 0;
    }

    virtual int Finish(void *param = nullptr) {
      return 0;
    }
  };

 protected:
  bool dumpTime = false;
  FuncOptimizeImpl *phaseImpl;
  thread_local static FuncOptimizeImpl *phaseImplLocal;

 public:
  explicit FuncOptimizeIterator(std::string phaseName, FuncOptimizeImpl *phaseImpl);
  virtual ~FuncOptimizeIterator();
  virtual void Run(uint32_t nThread = 1, bool isSeq = false);

 protected:
  void RunSerial();
  void RunParallel(uint32_t nThread, bool isSeq = false);

  virtual void *CallbackGetTaskRunParam() {
    return phaseImplLocal;
  }

  virtual void *CallbackGetTaskFinishParam() {
    return phaseImplLocal;
  }

  virtual void CallbackThreadMainStart() {
    phaseImplLocal = phaseImpl->Clone();
    phaseImplLocal->CreateLocalBuilder(&mutexGlobal);
  }

  virtual void CallbackThreadMainEnd() {
    delete phaseImplLocal;
    phaseImplLocal = nullptr;
  }
};

#define OPT_TEMPLATE(OPT_NAME)                                                                 \
  auto *kh = static_cast<KlassHierarchy*>(mrm->GetAnalysisResult(MoPhase_CHA, mod));           \
  ASSERT_NOT_NULL(kh);                                                                         \
  FuncOptimizeIterator opt(PhaseName(), new OPT_NAME(mod, kh, TRACE_PHASE));                   \
  opt.Run();

}  // namespace maple

#endif  // MAPLE_PHASE_INCLUDE_PHASE_IMPL_H
