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
#include "option_parser.h"
#include <regex>
#include <iostream>
#include <cstring>
#include "mpl_logging.h"

using namespace maple;
using namespace mapleOption;

OptionParser::OptionParser(const Descriptor usage[]) {
  for (size_t i = 0; usage[i].help != nullptr; ++i) {
    rawUsages.push_back(usage[i]);
    if (usage[i].shortOption != nullptr) {
      InsertOption(std::string(usage[i].shortOption), usage[i]);
    }
    if (usage[i].longOption != nullptr) {
      InsertOption(std::string(usage[i].longOption), usage[i]);
    }
#ifdef OPTION_PARSER_EXTRAOPT
    // Insert usage for extra options
    InsertExtraUsage(usage[i]);
#endif
    // Add --no-opt for boolean option
    if (usage[i].checkPolicy == kArgCheckPolicyBool) {
      CreateNoOption(usage[i]);
    }
  }
}

#ifdef OPTION_PARSER_EXTRAOPT
void OptionParser::InsertExtraUsage(const Descriptor &usage) {
  unsigned int index = 0;
  while (index < kMaxExtraOptions && usage.extras[index].exeName != nullptr) {
    Descriptor tempUsage = { usage.index,
                             usage.type,
                             usage.shortOption,
                             usage.longOption,
                             nullptr,
                             false,
                             nullptr,
                             usage.enableBuildType,
                             usage.checkPolicy,
                             usage.help,
                             usage.extras[index].exeName,
                             { { nullptr } } };
    if (usage.shortOption != nullptr) {
      InsertOption(std::string(usage.shortOption), tempUsage);
    }
    if (usage.longOption != nullptr) {
      InsertOption(std::string(usage.longOption), tempUsage);
    }
    rawUsages.push_back(tempUsage);
    index++;
  }
}
#endif

void OptionParser::CreateNoOption(const Descriptor &usage) {
  std::string longOpt = "";
  std::string shortOpt = "";
  std::string newHelpInfo = "";
  if (usage.shortOption != nullptr) {
    std::ostringstream optionString;
    optionString << "no-" << usage.shortOption; // Generate short option: no-opt
    shortOpt = optionString.str();
  }
  if (usage.longOption != nullptr) {
    std::ostringstream optionString;
    optionString << "no-" << usage.longOption; // Generate long option: no-opt
    longOpt = optionString.str();
  }
  Descriptor tempUsage = { usage.index,
                           kDisable, // Change to disable
                           (usage.shortOption != nullptr) ? shortOpt.c_str() : nullptr,
                           (usage.longOption != nullptr) ? longOpt.c_str() : nullptr,
#ifdef OPTION_PARSER_EXTRAOPT
                           nullptr,
                           false,
                           nullptr,
#endif
                           usage.enableBuildType,
                           usage.checkPolicy,
                           usage.help,
#ifdef OPTION_PARSER_EXTRAOPT
                           usage.exeName,
                           usage.extras
#endif
  };
  if (usage.shortOption != nullptr) {
    InsertOption(shortOpt, tempUsage);
  }
  if (usage.longOption != nullptr) {
    InsertOption(longOpt, tempUsage);
  }
#ifdef OPTION_PARSER_EXTRAOPT
  // Insert usage for extra options
  InsertExtraUsage(tempUsage);
#endif
}

void OptionParser::PrintUsage() const {
  for (size_t i = 0; i < rawUsages.size(); ++i) {
    if (rawUsages[i].help != nullptr && rawUsages[i].IsEnabledForCurrentBuild()) {
      LogInfo::MapleLogger() << rawUsages[i].help << '\n';
    }
  }
}

#ifdef OPTION_PARSER_EXTRAOPT
void OptionParser::PrintUsage(const std::string &helpType) const {
  for (size_t i = 0; i < rawUsages.size(); ++i) {
    if (rawUsages[i].help != nullptr && rawUsages[i].IsEnabledForCurrentBuild() && rawUsages[i].exeName == helpType) {
      LogInfo::MapleLogger() << rawUsages[i].help << '\n';
    }
  }
}
#endif


bool OptionParser::HandleKeyValue(const std::string &key, const std::string &value, bool isValueEmpty,
                                  std::vector<mapleOption::Option> &inputOption, const std::string &exeName,
                                  bool isAllOption) {
  if (key.empty()) {
    if (!isAllOption) {
      LogInfo::MapleLogger(kLlErr) << "Cannot recognize " << value << '\n';
      return false;
    }
    nonOptionsArgs.push_back(value);
    return true;
  }
  size_t count = usages.count(key);
  auto item = usages.find(key);
#ifdef OPTION_PARSER_EXTRAOPT
  while (count > 0 && item->second.exeName != exeName) {
    ++item;
    --count;
  }
#endif
  if (count == 0) {
    LogInfo::MapleLogger(kLlErr) << ("Unknown Option: " + key) << '\n';
    return false;
  }
  switch (item->second.checkPolicy) {
    case kArgCheckPolicyUnknown:
      LogInfo::MapleLogger(kLlErr) << ("Unknown option " + key) << '\n';
      return false;
    case kArgCheckPolicyNone:
    case kArgCheckPolicyOptional:
      break;
    case kArgCheckPolicyRequired:
      if (value.empty() && !isValueEmpty) {
        LogInfo::MapleLogger(kLlErr) << ("Option " + key + " requires an argument.") << '\n';
        return false;
      }
      break;
    case kArgCheckPolicyNumeric:
      if (value.empty()) {
        LogInfo::MapleLogger(kLlErr) << ("Option " + key + " requires an argument.") << '\n';
        return false;
      } else {
        std::regex rx("^(-|)[0-9]+\\b");
        bool isNumeric = std::regex_match(value.begin(), value.end(), rx);
        if (!isNumeric) {
          LogInfo::MapleLogger(kLlErr) << ("Option " + key + " requires a numeric argument.") << '\n';
          return false;
        }
      }
      break;
    default:
      break;
  }
  inputOption.push_back(Option(item->second, key, value));
  return true;
}

#ifdef OPTION_PARSER_EXTRAOPT
bool OptionParser::SetOption(const std::string &rawKey, const std::string &value, const std::string &exeName,
                             std::vector<mapleOption::Option> &exeOption) {
  constexpr char kOptionMark = '-';
  if (rawKey.empty()) {
    LogInfo::MapleLogger(kLlErr) << "Invalid key" << '\n';
    PrintUsage("all");
    return false;
  }
  int index = 0;
  if (rawKey[0] == kOptionMark) {
    ++index;
    if (rawKey[1] == kOptionMark) {
      ++index;
    }
  }
  std::string key = rawKey.substr(index);
  int count = usages.count(key);
  auto item = usages.find(key);
  while (count > 0) {
    if (item->second.exeName != exeName) {
      ++item;
      --count;
      continue;
    }
    switch (item->second.checkPolicy) {
      case kArgCheckPolicyNone:
      case kArgCheckPolicyOptional:
        break;
      case kArgCheckPolicyRequired:
        if (value.empty()) {
          LogInfo::MapleLogger(kLlErr) << ("Option " + key + " requires an argument.") << '\n';
          return false;
        }
        break;
      default:
        break;
    }
    break;
  }
  exeOption.push_back(Option(item->second, key, value));
  return true;
}
#endif

bool OptionParser::CheckOpt(const std::string option, std::string &lastKey,
                            bool &isLastMatch, std::vector<mapleOption::Option> &inputOption,
                            const std::string &exeName) {
  std::vector<std::string> temps;
  size_t pos = option.find('=');
  if (pos != std::string::npos) {
    ASSERT(pos > 0, "option should not begin with symbol '='");
    isLastMatch = false;
    std::string key = option.substr(0, pos);
    std::string value = option.substr(pos + 1);
    return HandleKeyValue(key, value, value.empty(), inputOption, exeName);
  } else {
    auto item = usages.find(option);
    if (item != usages.end()) {
      if (item->second.checkPolicy == kArgCheckPolicyRequired || item->second.checkPolicy == kArgCheckPolicyNumeric) {
        lastKey = option;
        isLastMatch = true;
      } else {
        return HandleKeyValue(option, "", false, inputOption, exeName);
      }
    } else {
      LogInfo::MapleLogger(kLlErr) << ("Unknown Option: " + option) << '\n';
      return false;
    }
  }
  return true;
}

ErrorCode OptionParser::HandleInputArgs(const std::vector<std::string> &inputArgs, const std::string &exeName,
                                        std::vector<mapleOption::Option> &inputOption) {
  bool isLastMatchOpt = false;
  std::string lastKey = "";
  bool ret = true;
  for (size_t i = 0; i < inputArgs.size(); ++i) {
    if (inputArgs[i] == "") {
      continue;
    }
    bool isMatchLongOpt = false;
    bool isMatchShortOpt = false;
    MatchedIndex index = kMatchNone;
    if (inputArgs[i][0] == '-') {
      index = kMatchShortOpt;
      if (inputArgs[i][1] == '-') {
        index = kMatchLongOpt;
      }
    }
    if (index == kMatchShortOpt) {
      isMatchShortOpt = true;
    } else if (index == kMatchLongOpt) {
      isMatchLongOpt = true;
    }
    std::string arg = inputArgs[i].substr(index);
    bool isAllOption = (exeName == "all") ? true : false;
    bool isOptMatched = isMatchLongOpt || isMatchShortOpt;
    if (!isLastMatchOpt && isOptMatched) {
      ret = CheckOpt(arg, lastKey, isLastMatchOpt, inputOption, exeName);
    } else if (isLastMatchOpt && !isOptMatched) {
      isLastMatchOpt = false;
      ret = HandleKeyValue(lastKey, arg, false, inputOption, exeName, isAllOption);
    } else if (isLastMatchOpt && isOptMatched) {
      LogInfo::MapleLogger(kLlErr) << ("Unknown Option: " + arg) << '\n';
      return kErrorInvalidParameter;
    } else {
      ret = HandleKeyValue("", arg, false, inputOption, exeName, isAllOption);
    }
    if (i == inputArgs.size() - 1 && isLastMatchOpt) {
      LogInfo::MapleLogger(kLlErr) << ("Option " + lastKey + " requires an argument.") << '\n';
      return kErrorInvalidParameter;
    }
    if (!ret) {
      return kErrorInvalidParameter;
    }
  }
  return kErrorNoError;
}

ErrorCode OptionParser::Parse(int argc, char **argv) {
  if (argc > 0) {
    --argc;
    ++argv;  // skip program name argv[0] if present
  }
  if (argc == 0 || *argv == nullptr) {
#ifdef OPTION_PARSER_EXTRAOPT
    PrintUsage("all");
#else
    PrintUsage();
#endif
    LogInfo::MapleLogger(kLlErr) << "No input files!" << '\n';
    return kErrorInitFail;
  }
  // transform char* to string
  std::vector<std::string> inputArgs;
  while (argc != 0 && *argv != nullptr) {
    inputArgs.push_back(*argv);
    ++argv;
    --argc;
  }
  ErrorCode ret = HandleInputArgs(inputArgs, "all", options);
  return ret;
}
