#!/bin/bash
set -e
cd $(dirname $0)/..

if [ ! -e build/develop ]; then
  cmake -B build/develop \
    -DCMAKE_TOOLCHAIN_FILE=$PWD/competitive-library/toolchains/develop.cmake
fi
cmake -B build/develop
cmake --build build/develop --target vis gen

if [ ! -e tools ]; then
  mkdir -p tools/target/release
  cp build/develop/example_problem/vis tools/target/release
fi

rm -rf samples
./build/develop/example_problem/gen --dir=samples example_problem/seed.txt
