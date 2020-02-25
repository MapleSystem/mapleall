/*
 * Copyright (c) [2019-2020] Huawei Technologies Co.,Ltd.All rights reserved.
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
#include <iterator>
#include <algorithm>
#include "compiler.h"
#include "usages.h"
#include "string_utils.h"
#include "mpl_logging.h"
#include "driver_runner.h"

namespace maple {
using namespace mapleOption;

std::string MapleCombCompiler::GetInputFileName(const MplOptions &options) const {
  if (!options.GetRunningExes().empty()) {
    if (options.GetRunningExes()[0] == kBinNameMe || options.GetRunningExes()[0] == kBinNameMpl2mpl) {
      return options.GetInputFiles();
    }
  }
  if (options.GetInputFileType() == InputFileType::kVtableImplMpl) {
    return options.GetOutputFolder() + options.GetOutputName() + ".VtableImpl.mpl";
  }
  return options.GetOutputFolder() + options.GetOutputName() + ".mpl";
}


std::unordered_set<std::string> MapleCombCompiler::GetFinalOutputs(const MplOptions &mplOptions) const {
  std::unordered_set<std::string> finalOutputs;
  finalOutputs.insert(mplOptions.GetOutputFolder() + mplOptions.GetOutputName() + ".VtableImpl.mpl");
  return finalOutputs;
}

void MapleCombCompiler::PrintCommand(const MplOptions &options) const {
  std::string runStr = "--run=";
  std::ostringstream optionStr;
  optionStr << "--option=\"";
  std::string connectSym = "";
  bool firstComb = false;
  if (options.GetExeOptions().find(kBinNameMe) != options.GetExeOptions().end()) {
    runStr += "me";
    auto it = options.GetExeOptions().find(kBinNameMe);
    for (const mapleOption::Option &opt : it->second) {
      connectSym = !opt.Args().empty() ? "=" : "";
      optionStr << " --" << opt.OptionKey() << connectSym << opt.Args();
    }
    firstComb = true;
  }
  if (options.GetExeOptions().find(kBinNameMpl2mpl) != options.GetExeOptions().end()) {
    if (firstComb) {
      runStr += ":mpl2mpl";
      optionStr << ":";
    } else {
      runStr += "mpl2mpl";
    }
    auto it = options.GetExeOptions().find(kBinNameMpl2mpl);
    for (const mapleOption::Option &opt : it->second) {
      connectSym = !opt.Args().empty() ? "=" : "";
      optionStr << " --" << opt.OptionKey() << connectSym << opt.Args();
    }
  }
  optionStr << "\"";
  LogInfo::MapleLogger() << "Starting:" << options.GetExeFolder() << "maple " << runStr << " " << optionStr.str() << " "
                         << GetInputFileName(options) << options.GetPrintCommandStr() << '\n';
}

#if 0
MeOption *MapleCombCompiler::MakeMeOptions(const MplOptions &options, MemPool &optMp) {
  MeOption *meOption = new MeOption(optMp);
  auto it = options.GetExeOptions().find(kBinNameMe);
  if (it == options.GetExeOptions().end()) {
    LogInfo::MapleLogger() << "no me input options\n";
    return meOption;
  }
  for (const mapleOption::Option &opt : it->second) {
    if (options.HasSetDebugFlag()) {
      LogInfo::MapleLogger() << "Me options: " << opt.Index() << " " << opt.OptionKey() << " " << opt.Args() << '\n';
    }
    switch (opt.Index()) {
      case kMeSkipPhases:
        meOption->SplitSkipPhases(opt.Args());
        break;
      case kMeRange:
        meOption->useRange = true;
        meOption->GetRange(opt.Args());
        break;
      case kMeDumpAfter:
        meOption->dumpAfter = (opt.Type() == kEnable);
        break;
      case kMeDumpFunc:
        meOption->dumpFunc = opt.Args();
        break;
      case kMeDumpPhases:
        meOption->SplitPhases(opt.Args().c_str(), meOption->dumpPhases);
        break;
      case kMeQuiet:
        meOption->quiet = (opt.Type() == kEnable);
        break;
      case kSetCalleeHasSideEffect:
        meOption->setCalleeHasSideEffect = (opt.Type() == kEnable);
        break;
      case kNoSteensgaard:
        meOption->noSteensgaard = (opt.Type() == kEnable);
        break;
      case kNoTBAA:
        meOption->noTBAA = (opt.Type() == kEnable);
        break;
      case kAliasAnalysisLevel:
        meOption->aliasAnalysisLevel = std::stoul(opt.Args(), nullptr);
        if (meOption->aliasAnalysisLevel > MeOption::kLevelThree) {
          meOption->aliasAnalysisLevel = MeOption::kLevelThree;
        }
        switch (meOption->aliasAnalysisLevel) {
          case MeOption::kLevelThree:
            meOption->setCalleeHasSideEffect = false;
            meOption->noSteensgaard = false;
            meOption->noTBAA = false;
            break;
          case MeOption::kLevelZero:
            meOption->setCalleeHasSideEffect = true;
            meOption->noSteensgaard = true;
            meOption->noTBAA = true;
            break;
          case MeOption::kLevelOne:
            meOption->setCalleeHasSideEffect = false;
            meOption->noSteensgaard = false;
            meOption->noTBAA = true;
            break;
          case MeOption::kLevelTwo:
            meOption->setCalleeHasSideEffect = false;
            meOption->noSteensgaard = true;
            meOption->noTBAA = false;
            break;
          default:
            break;
        }
        if (options.HasSetDebugFlag()) {
          LogInfo::MapleLogger() << "--sub options: setCalleeHasSideEffect "
                                 << meOption->setCalleeHasSideEffect << '\n';
          LogInfo::MapleLogger() << "--sub options: noSteensgaard " << meOption->noSteensgaard << '\n';
          LogInfo::MapleLogger() << "--sub options: noTBAA " << meOption->noTBAA << '\n';
        }
        break;
      case kMeNoDot:
        meOption->noDot = (opt.Type() == kEnable);
        break;
      case kStmtNum:
        meOption->stmtNum = (opt.Type() == kEnable);
        break;
      case kLessThrowAlias:
        meOption->lessThrowAlias = (opt.Type() == kEnable);
        break;
      case kRegReadAtReturn:
        meOption->regreadAtReturn = (opt.Type() == kEnable);
        break;
      default:
        WARN(kLncWarn, "input invalid key for me " + opt.OptionKey());
        break;
    }
  }
  return meOption;
}


Options *MapleCombCompiler::MakeMpl2MplOptions(const MplOptions &options, MemPool &optMp) {
  auto *mpl2mplOption = new Options(optMp);
  auto it = options.GetExeOptions().find(kBinNameMpl2mpl);
  if (it == options.GetExeOptions().end()) {
    LogInfo::MapleLogger() << "no mpl2mpl input options\n";
    return mpl2mplOption;
  }
  for (const mapleOption::Option &opt : it->second) {
    if (options.HasSetDebugFlag()) {
      LogInfo::MapleLogger() << "mpl2mpl options: " << opt.Index() << " " << opt.OptionKey() << " " << opt.Args()
                             << '\n';
    }
    switch (opt.Index()) {
      case kMpl2MplDumpBefore:
        mpl2mplOption->dumpBefore = (opt.Type() == kEnable);
        break;
      case kMpl2MplDumpAfter:
        mpl2mplOption->dumpAfter = (opt.Type() == kEnable);
        break;
      case kMpl2MplDumpFunc:
        mpl2mplOption->dumpFunc = opt.Args();
        break;
      case kMpl2MplQuiet:
        mpl2mplOption->quiet = (opt.Type() == kEnable);
        break;
      case kMpl2MplDumpPhase:
        mpl2mplOption->dumpPhase = opt.Args();
        break;
      case kMpl2MplSkipPhase:
        mpl2mplOption->skipPhase = opt.Args();
        break;
      case kMpl2MplSkipFrom:
        mpl2mplOption->skipFrom = opt.Args();
        break;
      case kMpl2MplSkipAfter:
        mpl2mplOption->skipAfter = opt.Args();
        break;
      case kRegNativeDynamicOnly:
        mpl2mplOption->regNativeDynamicOnly = (opt.Type() == kEnable);
        break;
      case kRegNativeStaticBindingList:
        mpl2mplOption->staticBindingList = opt.Args();
        break;
      case kMpl2MplStubJniFunc:
        mpl2mplOption->regNativeFunc = (opt.Type() == kEnable);
        break;
      case kNativeWrapper:
        mpl2mplOption->nativeWrapper = (opt.Type() == kEnable);
        break;
      case kInlineWithProfile:
        mpl2mplOption->inlineWithProfile = (opt.Type() == kEnable);
        break;
      case kMpl2MplUseInline:
        mpl2mplOption->useInline = (opt.Type() == kEnable);
        break;
      case kMpl2MplNoInlineFuncList:
        mpl2mplOption->noInlineFuncList = opt.Args();
        break;
      case kMpl2MplUseCrossModuleInline:
        mpl2mplOption->useCrossModuleInline = (opt.Type() == kEnable);
        break;
      case kInlineSmallFunctionThreshold:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --inline-small-function-threshold\n";
          exit(1);
        } else {
          mpl2mplOption->inlineSmallFunctionThreshold = std::stoul(opt.Args());
        }
        break;
      case kInlineHotFunctionThreshold:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --inline-hot-function-threshold\n";
          exit(1);
        } else {
          mpl2mplOption->inlineHotFunctionThreshold = std::stoul(opt.Args());
        }
        break;
      case kInlineModuleGrowth:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --inline-module-growth\n";
          exit(1);
        } else {
          mpl2mplOption->inlineModuleGrowth = std::stoul(opt.Args());
        }
        break;
      case kInlineColdFunctionThreshold:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --inline-cold-function-threshold\n";
          exit(1);
        } else {
          mpl2mplOption->inlineColdFunctionThreshold = std::stoul(opt.Args());
        }
        break;
      case kProfileHotCount:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --profile-hot-count\n";
          exit(1);
        } else {
          mpl2mplOption->profileHotCount = std::stoul(opt.Args());
        }
        break;
      case kProfileColdCount:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --profile-cold-count\n";
          exit(1);
        } else {
          mpl2mplOption->profileColdCount = std::stoul(opt.Args());
        }
        break;
      case kProfileHotRate:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --profile-hot-rate\n";
          exit(1);
        } else {
          mpl2mplOption->profileHotRate = std::stoul(opt.Args());
        }
        break;
      case kProfileColdRate:
        if (opt.Args().empty()) {
          LogInfo::MapleLogger(kLlErr) << "expecting not empty for --profile-cold-rate\n";
          exit(1);
        } else {
          mpl2mplOption->profileColdRate = std::stoul(opt.Args());
        }
        break;
      case kMpl2MplMapleLinker:
        mpl2mplOption->mapleLinker = (opt.Type() == kEnable);
        mpl2mplOption->dumpMuidFile = (opt.Type() == kEnable);
        if (options.HasSetDebugFlag()) {
          LogInfo::MapleLogger() << "--sub options: dumpMuidFile " << mpl2mplOption->dumpMuidFile << '\n';
        }
        break;
      case kMplnkDumpMuid:
        mpl2mplOption->dumpMuidFile = (opt.Type() == kEnable);
        break;
      case kEmitVtableImpl:
        mpl2mplOption->emitVtableImpl = (opt.Type() == kEnable);
        break;
#if MIR_JAVA
      case kMpl2MplSkipVirtual:
        mpl2mplOption->skipVirtualMethod = (opt.Type() == kEnable);
        break;
#endif
      default:
        WARN(kLncWarn, "input invalid key for mpl2mpl " + opt.OptionKey());
        break;
    }
  }
  return mpl2mplOption;
}
#endif

ErrorCode MapleCombCompiler::Compile(MplOptions &options, MIRModulePtr &theModule) {
  std::string fileName = GetInputFileName(options);
  theModule = new MIRModule(fileName.c_str());
#if 0
  MemPool *optMp = mempoolctrler.NewMemPool("maplecomb mempool");
  std::unique_ptr<MeOption> meOptions;
  std::unique_ptr<Options> mpl2mplOptions;
  auto it = std::find(options.GetRunningExes().begin(), options.GetRunningExes().end(), kBinNameMe);
  if (it != options.GetRunningExes().end()) {
    meOptions.reset(MakeMeOptions(options, *optMp));
  }
  auto iterMpl2Mpl = std::find(options.GetRunningExes().begin(), options.GetRunningExes().end(), kBinNameMpl2mpl);
  if (iterMpl2Mpl != options.GetRunningExes().end()) {
    mpl2mplOptions.reset(MakeMpl2MplOptions(options, *optMp));
  }

  LogInfo::MapleLogger() << "Starting mpl2mpl&mplme\n";
  PrintCommand(options);
  DriverRunner runner(theModule, options.GetRunningExes(), options.GetInputFileType(), mpl2mplOptions.get(), fileName, meOptions.get(),
                      fileName, fileName, optMp,
                      options.HasSetTimePhases(), options.HasSetGenVtableImpl(), options.HasSetGenMeMpl());
#else
  LogInfo::MapleLogger() << "Starting mpl2mpl&mplme\n";
  //PrintCommand(options);
  const Options *mpl2mplOptions;
  DriverRunner runner(theModule, options.GetRunningExes(), options.GetInputFileType(), options, fileName, options.optMp);
#endif
  ErrorCode nErr = runner.Run();

#if 0
  mempoolctrler.DeleteMemPool(optMp);
#endif
  return nErr;
}
}  // namespace maple
