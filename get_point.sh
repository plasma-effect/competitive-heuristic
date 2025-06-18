#!/bin/bash
set -uo pipefail
TARGET_LESSER=0
make main_measure.o check.o
if [ $? -ne 0 ]; then
  exit 1
fi
mkdir -p tmp
SUM=0
ls samples/*.txt | parallel -k --bar --joblog parallel.log './main_measure.o < {} > samples_out/$(basename {}) 2>/dev/null'
if [ $? -ne 0 ]; then
  awk 'NR > 1 && $7 != 0 { print $9 }' parallel.log
  exit 1
fi
for file in $(ls samples/*.txt); do
  NOW=$(cat ${file} samples_out/$(basename ${file}) | ./check.o)
  SUM=$((SUM+NOW)) 
done

if [ -e best.txt ]; then
  BEST=$(cat best.txt)
elif [ ${TARGET_LESSER} -ge "1" ]; then
  BEST=1000000000
else
  BEST=0
fi

if [ ${TARGET_LESSER} -ge "1" ]; then
  if [ ${BEST} -gt ${SUM} ]; then
    echo "updated the best score by $((BEST-SUM))"
    echo "new best score is ${SUM}"
    echo ${SUM} > best.txt
  else
    echo "${SUM} (best score = ${BEST}, diff = $((SUM-BEST)))"
  fi
else
  if [ ${BEST} -lt ${SUM} ]; then
    echo "updated the best score by $((SUM-BEST))"
    echo "new best score is ${SUM}"
    echo ${SUM} > best.txt
  else
    echo "${SUM} (best score = ${BEST}, diff = $((BEST-SUM)))"
  fi
fi
