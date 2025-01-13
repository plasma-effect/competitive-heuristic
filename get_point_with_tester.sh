#!/bin/bash
set -euo pipefail
make
mkdir -p tmp
SUM=0
for file in $(ls samples/*.txt); do
  echo "start ${file}"
  timeout 10 cat ${file} | ./main.o > tmp/out.txt 2>/dev/null
  NOW=$(./tools/target/release/vis ${file} tmp/out.txt)
  SUM=$((SUM+NOW)) 
done

if [ -e best.txt ]; then
  BEST=$(cat best.txt)
else
  BEST=0
fi

if [ ${BEST} -lt ${SUM} ]; then
  echo "updated the best score by $((SUM-BEST))"
  echo "new best score is ${SUM}"
  echo ${SUM} > best.txt
else
  echo "${SUM} (best score = ${BEST}, diff = $((BEST-SUM)))"
fi
