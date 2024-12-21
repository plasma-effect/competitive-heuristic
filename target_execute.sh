set -euo pipefail

make
cp samples/$1.txt sample/in.txt
time cat sample/in.txt | ./main.o \
  > sample/out.txt \
  2> sample/debug.txt
