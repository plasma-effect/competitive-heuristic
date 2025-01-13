#!/bin/bash
set -euo pipefail

if [ ! -e tester.zip ]; then
  curl -o tester.zip $1
fi

if [ ! -e tools ]; then
  unzip tester.zip
fi

cd tools
cargo build -r
cd -

rm -rf samples
cp -r tools/in samples
