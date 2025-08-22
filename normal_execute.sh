set -euo pipefail

mkdir -p tmp
cp /dev/null tmp/gridDataBase.csv
cmake --build build/develop --target main create -j4
trap 'cat tmp/gridDataBase.csv | \
  build/develop/grid_slider/create > \
  tmp/gridData.js' EXIT
echo "start execution"
time timeout 10s ./build/develop/main < sample/in.txt \
  > sample/out.txt \
  2> sample/debug.txt
if [ -e ./tools/target/release/vis ]; then
SCORE=$(./tools/target/release/vis sample/in.txt sample/out.txt)
echo "execution succeed (${SCORE})"
else
echo "execution succeed"
fi

if command -v clip.exe >/dev/null 2>&1; then
  cat sample/out.txt | clip.exe
  echo "copied output to clipboard (for web visualizer)"
fi
