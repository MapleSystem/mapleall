#
# Copyright (c) [2020] Huawei Technologies Co., Ltd. All rights reserved.
#
# Licensed under the Mulan Permissive Software License v2.
# You can use this software according to the terms and conditions of the MulanPSL - 2.0.
# You may obtain a copy of MulanPSL - 2.0 at:
#
#     https://opensource.org/licenses/MulanPSL-2.0
#
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
# FIT FOR A PARTICULAR PURPOSE.
# See the MulanPSL - 2.0 for more details.
#

import os, sys
import time

def check(line):
  n = len(line)
  if line[0] != '{':
    raise Exception("Should begin with '{'");
  if line[n-1] != ',':
    raise Exception("Record delimitter is missing");

  s = 0
  for i in range(0,n):
    c = line[i]
    if c == '{':
       s += 1
    elif c == '}':
       s -= 1

    if s < 0:
       raise Exception("Unmatched '}'")
  if s > 0:
    raise Exception("Unmatched '{'")

def process_line(line):
  line = line.strip()
  if len(line) == 0:
    return None

  if line[0] == '/' and line[1] == '/':
    # comment
    return None

  try:
    check(line)
  except:
    print ("'"+line+"' is invalid")
    return None

  k = line.find(',',0)
  key = line[1:k]
  return key.strip() + ",\n"


def process(mdfilename, newmdfilename):
  if(os.path.exists(newmdfilename) and (os.stat(mdfilename).st_mtime < os.stat(newmdfilename).st_mtime)):
    pass
  else:
    with open( mdfilename, "r" ) as infile:
      lines = []
      for l in infile:
        if(process_line(l) != None):
          lines.append(process_line(l))
    with open(newmdfilename, "w") as outfile:
        for line in lines:
          outfile.write(line)
#   print lines

def help():
  print ("Usage: " + sys.argv[0] + " md-file")

def main():
    if len(sys.argv) != 3:
        help();
        return
    process(sys.argv[1], sys.argv[2])
if __name__ == "__main__":
  main()
