#!/bin/bash
set -euo pipefail
make $1
mkdir -p tmp
SUM=0
for file in $(ls samples/*.txt); do
  echo "start ${file}"
  timeout 10 cat ${file} | ./codes/$1.o > tmp/out.txt 2>/dev/null
  NOW=$(cat ${file} tmp/out.txt | ./check.o)
  SUM=$((SUM+NOW))
done

echo "codes/$1 result is ${SUM}"
