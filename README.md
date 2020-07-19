```
#
# Copyright (C) [2020] Futurewei Technologies, Inc. All rights reverved.
#
# Licensed under the Mulan Permissive Software License v2.
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
```

#MAPLE - Multiple Architecture and Programming Language Environment

## Steps to build

Let MAPLE_ROOT be the top directory for mapleall repository.

The directory structure as follows:

```
      $MAPLE_ROOT
            README.md    : this file
            Copyright    : Copyright file
            license      : Mulan license file
            doc          : MapleIR documentation
            mapleall     : maple compiler source
            maple_engine : maple engine opcode header file
            tools        : ninja gn tools
            Makefile     : makefile
            build        : environment set up and build files
            BUILD.gn     : gn build file
            bin          : created during make install
            out          : created during make
```

## Set up tools
1. cd $MAPLE_ROOT
2. cd tools
3. ./setup_tools.sh

## Build compiler
1. cd $MAPLE_ROOT
2. choose {TARGET, VERSION} combo from four flavors {arm/ark, release/debug}
3. source envsetup.sh TARGET VERSION
4. make
5. make install

## Output
maple excutables are in $MAPLE_ROOT/bin directory

## Usage
refer to maple_build git repository for java2asm.sh and asm2so.sh scripts
java2asm.sh: .java -> .mpl -> .s
asm2so.sh  : .s -> .so

## Possible issues
You might need to install required packages like:
```
        sudo apt-get -y install clang
        sudo apt-get -y install libelf-dev
        sudo apt-get -y install libssl-dev
```