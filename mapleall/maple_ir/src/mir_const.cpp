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

#include "mir_const.h"
#include "mir_nodes.h"
#include "mir_function.h"
#include "printing.h"
#include "name_mangler.h"

#if MIR_FEATURE_FULL
#include <iostream>
#include <iomanip>

namespace maple {

using std::isnan;

void MIRConst::Dump(const MIRSymbolTable *localsymtab) const {
  if (fieldID) {
    LogInfo::MapleLogger() << fieldID << "= ";
  }
}

void MIRIntConst::Dump(const MIRSymbolTable *localsymtab) const {
  MIRConst::Dump(localsymtab);
  if (value <= 1024) {
    LogInfo::MapleLogger() << value;
  } else {
    LogInfo::MapleLogger() << std::hex << "0x" << value << std::dec;
  }
}

bool MIRIntConst::operator==(MIRConst &rhs) const {
  if (&rhs == this) {
    return true;
  }
  MIRIntConst *intconst = dynamic_cast<MIRIntConst *>(&rhs);
  return (intconst && (intconst->type == type) && (intconst->value == value));
}

void MIRVectorIntConst::Dump(const MIRSymbolTable *localsymtab) const {
  LogInfo::MapleLogger()<<"[";
  CHECK_FATAL(this->vecSize > 0, "");
  for (uint32 i = 0; i < this->vecSize; i++) {
    int64 val = this->vecElems[i];
    if (val <= 1024) {
      LogInfo::MapleLogger() << val;
    } else {
      LogInfo::MapleLogger()<<std::hex<<"0x"<<val<<std::dec;
    }
    if (i != (uint32)(this->vecSize - 1))
      LogInfo::MapleLogger()<<",";
  }
  LogInfo::MapleLogger()<<"]";
}

void MIRAddrofConst::Dump(const MIRSymbolTable *localsymtab) const {
  MIRConst::Dump(localsymtab);
  LogInfo::MapleLogger() << "addrof " << GetPrimTypeName(PTY_ptr);
  MIRSymbol *sym = stIdx.IsGlobal() ? GlobalTables::GetGsymTable().GetSymbolFromStIdx(stIdx.Idx())
                        : localsymtab->GetSymbolFromStIdx(stIdx.Idx());
  CHECK_FATAL(stIdx.IsGlobal() || sym->storageClass == kScPstatic || sym->storageClass == kScFstatic,
              "MIRAddrofConst can only point to a global symbol");
  LogInfo::MapleLogger() << (stIdx.IsGlobal() ? " $" : " %") << sym->GetName();
  if (fldID > 0) {
    LogInfo::MapleLogger() << fldID;
  }
  if (offset != 0) {
    LogInfo::MapleLogger() << " (" << offset << ")";
  }
}

bool MIRAddrofConst::operator==(MIRConst &rhs) const {
  if (&rhs == this) {
    return true;
  }
  MIRAddrofConst *rhsA = dynamic_cast<MIRAddrofConst *>(&rhs);
  if (!rhsA) {
    return false;
  }
  if (type != rhs.type) {
    return false;
  }
  return (stIdx == rhsA->stIdx && fldID == rhsA->fldID);
}

void MIRAddroffuncConst::Dump(const MIRSymbolTable *localsymtab) const {
  MIRConst::Dump(localsymtab);
  LogInfo::MapleLogger() << "addroffunc " << GetPrimTypeName(PTY_ptr);
  MIRFunction *func = GlobalTables::GetFunctionTable().funcTable.at(puIdx);
  LogInfo::MapleLogger() << " &" << GlobalTables::GetGsymTable().GetSymbolFromStIdx(func->stIdx.Idx())->GetName();
}

bool MIRAddroffuncConst::operator==(MIRConst &rhs) const {
  if (&rhs == this) {
    return true;
  }
  MIRAddroffuncConst *rhsAf = dynamic_cast<MIRAddroffuncConst *>(&rhs);
  if (!rhsAf) {
    return false;
  }
  return (type == rhs.type && puIdx == rhsAf->puIdx);
}

void MIRLblConst::Dump(const MIRSymbolTable *localsymtab) const {
  MIRConst::Dump(localsymtab);
  LogInfo::MapleLogger() << "addroflabel " << GetPrimTypeName(PTY_ptr);
  MIRFunction *func = GlobalTables::GetFunctionTable().funcTable.at(puIdx);
  LogInfo::MapleLogger() << " @" << func->GetLabelName(value);
}

bool MIRLblConst::operator==(MIRConst &rhs) const {
  if (&rhs == this) {
    return true;
  }
  MIRLblConst *lblconst = dynamic_cast<MIRLblConst *>(&rhs);
  return (lblconst && (lblconst->value == value));
}

bool MIRFloatConst::operator==(MIRConst &rhs) const {
  if (&rhs == this) {
    return true;
  }
  MIRFloatConst *fltconst = dynamic_cast<MIRFloatConst *>(&rhs);
  if (!fltconst) {
    return false;
  }
  if (isnan(fltconst->value.floatValue)) {
    return isnan(value.floatValue);
  }
  if (isnan(value.floatValue)) {
    return isnan(fltconst->value.floatValue);
  }
  return (fabs(fltconst->value.floatValue - value.floatValue) <= 1e-6);
}

bool MIRDoubleConst::operator==(MIRConst &rhs) const {
  if (&rhs == this) {
    return true;
  }
  MIRDoubleConst *fltconst = dynamic_cast<MIRDoubleConst *>(&rhs);
  if (!fltconst) {
    return false;
  }
  if (isnan(fltconst->value.dValue)) {
    return isnan(value.dValue);
  }
  if (isnan(value.dValue)) {
    return isnan(fltconst->value.dValue);
  }
  return (fabs(fltconst->value.dValue - value.dValue) <= 1e-15);
}

bool MIRFloat128Const::operator==(MIRConst &rhs) const {
  if (&rhs == this) {
    return true;
  }
  MIRFloat128Const *fltconst = dynamic_cast<MIRFloat128Const *>(&rhs);
  if (!fltconst) {
    return false;
  }
  if ((value[0] == fltconst->value[0]) && (value[1] == fltconst->value[1])) {
    return true;
  }
  return false;
}

bool MIRAggConst::operator==(MIRConst &rhs) const {
  if (&rhs == this) {
    return true;
  }
  MIRAggConst *aggconst = dynamic_cast<MIRAggConst *>(&rhs);
  if (!aggconst) {
    return false;
  }
  if (aggconst->constVec.size() != constVec.size()) {
    return false;
  }
  for (unsigned i = 0; i < constVec.size(); ++i) {
    if (!(*aggconst->constVec[i] == *constVec[i])) {
      return false;
    }
  }
  return true;
}

void MIRFloatConst::Dump(const MIRSymbolTable *localsymtab) const {
  MIRConst::Dump(localsymtab);
  LogInfo::MapleLogger() << std::setprecision(std::numeric_limits<float>::max_digits10) << value.floatValue << "f";
}

void MIRDoubleConst::Dump(const MIRSymbolTable *localsymtab) const {
  MIRConst::Dump(localsymtab);
  LogInfo::MapleLogger() << std::setprecision(std::numeric_limits<double>::max_digits10) << value.dValue;
}

void MIRFloat128Const::Dump(const MIRSymbolTable *localsymtab) const {
  MIRConst::Dump(localsymtab);
  LogInfo::MapleLogger().setf(std::ios::uppercase);
  LogInfo::MapleLogger() << "0xL" << std::hex << std::setfill('0') << std::setw(16) << value[0]
                                 << std::setfill('0') << std::setw(16) << value[1];
}

void MIRAggConst::Dump(const MIRSymbolTable *localsymtab) const {
  MIRConst::Dump(localsymtab);
  LogInfo::MapleLogger() << "[";
  uint32 size = constVec.size();
  for (uint32 i = 0; i < size; i++) {
    constVec[i]->Dump(localsymtab);
    if (i != size - 1) {
      LogInfo::MapleLogger() << ", ";
    }
  }
  LogInfo::MapleLogger() << "]";
}

MIRStrConst::MIRStrConst(const std::string &str, MIRType *type, uint32 fieldID)
    : MIRConst(type, fieldID), value(GlobalTables::GetUStrTable().GetOrCreateStrIdxFromName(str)) {
  kind = kConstStrConst;
}

void MIRStrConst::Dump(const MIRSymbolTable *localsymtab) const {
  MIRConst::Dump(localsymtab);
  LogInfo::MapleLogger() << "conststr " << GetPrimTypeName(type->primType);
  const std::string kStr = GlobalTables::GetUStrTable().GetStringFromStrIdx(value);
  PrintString(kStr);
}

bool MIRStrConst::operator==(MIRConst &rhs) const {
  if (&rhs == this) {
    return true;
  }
  MIRStrConst *rhsCs = dynamic_cast<MIRStrConst *>(&rhs);
  if (!rhsCs) {
    return false;
  }
  return (type == rhs.type && value == rhsCs->value);
}

MIRStr16Const::MIRStr16Const(const std::u16string &str, MIRType *type, uint32 fieldID)
    : MIRConst(type, fieldID), value(GlobalTables::GetU16StrTable().GetOrCreateStrIdxFromName(str)) {
  kind = kConstStr16Const;
}

void MIRStr16Const::Dump(const MIRSymbolTable *localsymtab) const {
  MIRConst::Dump(localsymtab);
  LogInfo::MapleLogger() << "conststr16 " << GetPrimTypeName(type->primType);
  std::u16string str16 = GlobalTables::GetU16StrTable().GetStringFromStrIdx(U16StrIdx(value.GetIdx()));
  // UTF-16 string are dumped as UTF-8 string in mpl to keep the printable chars in ascii form
  std::string str;
  NameMangler::UTF16ToUTF8(str, str16);
  PrintString(str);
}

bool MIRStr16Const::operator==(MIRConst &rhs) const {
  if (&rhs == this) {
    return true;
  }
  MIRStr16Const *rhsCs = dynamic_cast<MIRStr16Const *>(&rhs);
  if (!rhsCs) {
    return false;
  }
  return (type == rhs.type && value == rhsCs->value);
}

}  // namespace maple
#endif  // MIR_FEATURE_FULL
