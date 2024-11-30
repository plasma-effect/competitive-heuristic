set -euo pipefail

make
time cat sample/in.txt | ./main.o \
  > sample/out.txt \
  2> sample/debug.txt
