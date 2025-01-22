#!/bin/bash
set -euo pipefail
TARGET_LESSER=1
make main.o check.o
mkdir -p tmp
SUM=0
for file in $(ls samples/*.txt); do
  echo "start ${file}"
  timeout 10 cat ${file} | ./main.o > tmp/out.txt 2>/dev/null
  NOW=$(cat ${file} tmp/out.txt | ./check.o)
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
