set -euo pipefail

mkdir -p tmp
cp /dev/null tmp/gridDataBase.csv
make main_debug.o main_measure.o grid_slider/create.o -j4
trap 'cat tmp/gridDataBase.csv | \
  grid_slider/create.o > \
  tmp/gridData.js' EXIT
cp samples/$1.txt sample/in.txt
time cat sample/in.txt | ./main.o \
  > sample/out.txt \
  2> sample/debug.txt
