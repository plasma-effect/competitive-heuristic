set -euo pipefail

mkdir -p tmp
cp /dev/null tmp/gridDataBase.csv
make main_debug.o main_measure.o grid_slider/create.o -j4
trap 'cat tmp/gridDataBase.csv | \
  grid_slider/create.o > \
  tmp/gridData.js' EXIT
time cat sample/in.txt | ./main_debug.o \
  > sample/out.txt \
  2> sample/debug.txt
