set -euo pipefail

cp /dev/null gridDataBase.csv
make main.o grid_slider/create_grid_data.o
trap 'cat gridDataBase.csv | grid_slider/create_grid_data.o > gridData.js' EXIT
cp samples/$1.txt sample/in.txt
time cat sample/in.txt | ./main.o \
  > sample/out.txt \
  2> sample/debug.txt
