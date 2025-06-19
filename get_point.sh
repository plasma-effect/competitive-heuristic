#!/bin/bash
set -uo pipefail
cmake --build build/release --target main score_manager -j4
if [ $? -ne 0 ]; then
  exit 1
fi
mkdir -p samples_out
SUM=0
ls samples/*.txt | parallel -k --bar --joblog parallel.log './build/release/main < {} > samples_out/$(basename {}) 2>/dev/null'
if [ $? -ne 0 ]; then
  awk 'NR > 1 && $7 != 0 { print $9 }' parallel.log
  exit 1
fi
for file in $(ls samples/*.txt); do
  NOW=$(./tools/target/release/vis ${file} samples_out/$(basename ${file}))
  SUM=$((SUM+NOW)) 
done

./build/release/score_manager score_setting.json $((SUM))
