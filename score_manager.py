import subprocess
import os
from typing import Dict
import argparse
from pathlib import Path
import tempfile
import re
import json
import hashlib
import sys
import shutil
import time

DATABASE="data.json"

def get_score(s: str):
    m = re.search(r"Score = (\d+)", s)
    if m:
        return int(m.group(1))
    else:
        return 0

def execute(dir: str):
    ret: Dict[str, int] = {}
    d = Path(dir)
    files = [file.absolute() for file in d.iterdir() if file.is_file()]
    max_elapsed = 0.0
    for file in sorted(files):
        print(f"start {file}")
        with open(file) as infile:
            try:
                start = time.perf_counter()
                exec_result = subprocess.run(["./main_measure.o"], stdin=infile, capture_output=True, text=True, timeout=5, check=True)
                end = time.perf_counter()
                max_elapsed = max(max_elapsed, end - start)
                with tempfile.NamedTemporaryFile(mode="w+") as tmp:
                    tmp.write(exec_result.stdout)
                    tmp.seek(0)
                    result = subprocess.run(["./tools/target/release/vis", file, tmp.name], capture_output=True, text=True)
                    score = get_score(result.stdout)
                    ret[file.name] = score
            except subprocess.CalledProcessError:
                print(f"error occured by \"{file}\"", file=sys.stderr)
    print(f"max execution time = {max_elapsed:.3f}sec")
    return ret

def get_min_max(data: Dict[str, Dict[str, int]], decrease: bool):
    ret: Dict[str, int] = {}
    for d in data.values():
        for name, value in d.items():
            if name not in ret:
                ret[name] = value
            elif decrease:
                if value != 0:
                    ret[name] = min(ret[name], value)
            else:
                ret[name] = max(ret[name], value)
    return ret

def main(dir: str, decrease: bool):
    main_path = "main.cpp"
    sha256 = hashlib.sha256()
    with open(main_path, "rb") as f:
        sha256.update(f.read())
    digest = sha256.hexdigest()
    data: Dict[str, Dict[str, int]] = {}
    if os.path.exists(DATABASE):
        with open(DATABASE, encoding="utf-8") as f:
            data = json.load(f)
    if digest not in data:
        os.makedirs("backup", exist_ok=True)
        shutil.copy2("main.cpp", "backup/" + digest + ".cpp")
        data[digest] = execute(dir)
    minmax = get_min_max(data, decrease)
    this = 0.0
    best = 0.0
    best_digest = ""
    for name, result in data.items():
        s = 0.0
        for file, value in minmax.items():
            if file in data[digest]:
                if decrease:
                    s += value / result[file]
                else:
                    s += result[file] / value
        s /= len(minmax)
        if s > best:
            best = s
            best_digest = name
        if name == digest:
            this = s
    shutil.copy2("backup/" + best_digest + ".cpp", "best.cpp")
    print(f"the result = {100*this:.3f}% (the best = {100*best:.3f}%)")
    with open(DATABASE, "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=4)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("dir", type=str)
    parser.add_argument("-d", "--decrease", action="store_true")
    args = parser.parse_args()
    main(args.dir, args.decrease)
