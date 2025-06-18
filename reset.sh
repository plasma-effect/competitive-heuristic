#!/bin/bash
set -euo pipefail

git submodule update --remote

if [ ! -e build/competitive-library ]; then
  cmake -S competitive-library/ \
    -B build/competitive-library \
    -DCMAKE_TOOLCHAIN_FILE=$PWD/competitive-library/toolchains/develop.cmake
fi
if [ ! -e build/develop ]; then
  cmake -B build/develop \
    -DCMAKE_TOOLCHAIN_FILE=$PWD/competitive-library/toolchains/develop.cmake
fi
if [ ! -e build/release ]; then
  cmake -B build/release \
    -DCMAKE_TOOLCHAIN_FILE=$PWD/competitive-library/toolchains/release.cmake
fi
cmake -S competitive-library/ -B build/competitive-library
cmake -B build/develop
cmake -B build/release

cmake --build build/competitive-library --target expander
./build/competitive-library/expander/expander templates/main.cpp -o main.cpp
cp templates/experiment.cpp experiment.cpp
