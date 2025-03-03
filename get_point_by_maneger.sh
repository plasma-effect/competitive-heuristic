#!/bin/bash
set -euo pipefail
make main_measure.o
python3 score_manager.py samples/
