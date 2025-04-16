set -euo pipefail

mkdir -p tmp
cp /dev/null tmp/gridDataBase.csv
make main_debug.o main_measure.o grid_slider/create.o -j4
trap 'cat tmp/gridDataBase.csv | \
  grid_slider/create.o > \
  tmp/gridData.js' EXIT
time timeout 10s ./main_debug.o < sample/in.txt \
  > sample/out.txt \
  2> sample/debug.txt
SCORE=$(./tools/target/release/vis sample/in.txt sample/out.txt)
echo "execution succeed (score: ${SCORE})"
