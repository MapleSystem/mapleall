```
# Copyright (C) [2020] Futurewei Technologies, Inc. All rights reverved.
#
# Licensed under the Mulan Permissive Software License v2
# You can use this software according to the terms and conditions of the MulanPSL - 2.0.
# You may obtain a copy of MulanPSL - 2.0 at:
#
#   https://opensource.org/licenses/MulanPSL-2.0
#
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
# FIT FOR A PARTICULAR PURPOSE.
# See the MulanPSL - 2.0 for more details.
#

Need Huawei Java Frontend and third-party packages

Please refer to maple_engine at https://gitee.com/openarkcompiler-incubator/maple_engine

basic steps are as following:

```

### Compile Java program to assembly code: .java -> .s

```
  $MAPLE_BUILD_TOOLS/java2asm.sh HelloWorld.java
```

### Compile assembly code to shared library: .s -> .so
```
  $MAPLE_BUILD_TOOLS/asm2so.sh HelloWorld.s
```

### Run program with Maple engine
```
$MAPLE_BUILD_TOOLS/run-app.sh -classpath ./HelloWorld.so HelloWorld
```
