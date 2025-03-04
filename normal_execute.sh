set -euo pipefail

mkdir -p tmp
cp /dev/null tmp/gridDataBase.csv
make main.o grid_slider/create_grid_data.o
trap 'cat tmp/gridDataBase.csv | \
  grid_slider/create_grid_data.o > \
  tmp/gridData.js' EXIT
time cat sample/in.txt | ./main.o \
  > sample/out.txt \
  2> sample/debug.txt
