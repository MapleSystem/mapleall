/*
 * Copyright (c) [2019-2020] Huawei Technologies Co., Ltd. All rights reserved.
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
#include "compiler_factory.h"
#include "error_code.h"
#include "mpl_options.h"
#include "mpl_logging.h"

using namespace maple;

void PrintErrorMessage(int ret) {
  switch (ret) {
    case kErrorNoError:
    case kErrorExitHelp:
      break;
    case kErrorExit:
      ERR(kLncErr, "Error Exit!");
      break;
    case kErrorInvalidParameter:
      ERR(kLncErr, "Invalid Parameter!");
      break;
    case kErrorInitFail:
      ERR(kLncErr, "Init Fail!");
      break;
    case kErrorFileNotFound:
      ERR(kLncErr, "File Not Found!");
      break;
    case kErrorToolNotFound:
      ERR(kLncErr, "Tool Not Found!");
      break;
    case kErrorCompileFail:
      ERR(kLncErr, "Compile Fail!");
      break;
    case kErrorNotImplement:
      ERR(kLncErr, "Not Implement!");
      break;
    default:
      break;
  }
}

int main(int argc, char **argv) {
  MemPool *optMp = mempoolctrler.NewMemPool("maple option mempool");
  MplOptions mplOptions(optMp);
  int ret = mplOptions.Parse(argc, argv);
  if (ret == kErrorNoError) {
    ret = CompilerFactory::GetInstance().Compile(mplOptions);
  }
  PrintErrorMessage(ret);
  mempoolctrler.DeleteMemPool(optMp);
  return ret;
}
