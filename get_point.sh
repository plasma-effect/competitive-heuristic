#!/bin/bash
set -uo pipefail
cmake --build build/release --target main score_manager -j4
if [ $? -ne 0 ]; then
  exit 1
fi
mkdir -p samples_out
SUM=0
ls samples/*.txt | 
  parallel -j4 -k --bar --joblog parallel.log \
  './build/release/main < {} > samples_out/$(basename {}) 2> samples_out/debug_$(basename {})'
if [ $? -ne 0 ]; then
  awk 'NR > 1 && $7 != 0 { print $9 }' parallel.log
  exit 1
fi

cp /dev/null results.txt
for file in $(ls samples/*.txt); do
  NOW=$(./tools/target/release/vis ${file} samples_out/$(basename ${file}))
  SUM=$((SUM+NOW)) 
  echo "$(basename ${file}) = $((NOW))" >> results.txt
done

./build/release/score_manager score_setting.json $((SUM))
