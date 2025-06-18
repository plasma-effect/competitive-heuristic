set -euo pipefail

mkdir -p tmp
cp /dev/null tmp/gridDataBase.csv
cmake --build build/develop --target main create -j4
trap 'cat tmp/gridDataBase.csv | \
  build/develop/grid_slider/create > \
  tmp/gridData.js' EXIT
cp samples/$1.txt sample/in.txt
echo "start execution"
time timeout 10 ./build/develop/main < sample/in.txt  \
  > sample/out.txt \
  2> sample/debug.txt
if [ -e ./tools/target/release/vis ]; then
SCORE=$(./tools/target/release/vis sample/in.txt sample/out.txt)
echo "execution succeed (${SCORE})"
else
echo "execution succeed"
fi
