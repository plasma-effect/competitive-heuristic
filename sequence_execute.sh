#!/bin/bash
set -euo pipefail
mkdir -p tmp samples_out
cmake --build build/develop --target main create -j4
for file in $(ls samples/*.txt); do
  cp /dev/null tmp/gridDataBase.csv
  echo "start execution $(basename ${file})"
  timeout 10 ./build/develop/main < ${file} \
    > samples_out/$(basename ${file}) \
    2> samples_out/debug_$(basename ${file})
done
echo "execution succeed"
