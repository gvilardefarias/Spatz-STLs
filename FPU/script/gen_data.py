#!/usr/bin/env python3
# Copyright 2026 Politecnico di Torino
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

# Author: Gustavo Vilar de Farias <gustavo.vilarde@polito.it>

import pathlib
import hjson
import numpy as np
import argparse

np.random.seed(21)

def mat2str(mat, mat_name):
    n_row = len(mat)
    try:
        n_col = len(mat[0])
    except:
        n_col = 1

    if n_col > 1:
        mat_str = f"unsigned int {mat_name}[{n_row}][{n_col}] = {{"
    else:
        mat_str = f"unsigned int {mat_name}[{n_row}] = {{"

    for i in range(n_row):
        for j in range(n_col):
            if n_col > 1:
                mat_str += f"{mat[i][j]}"
            else:
                mat_str += f"{mat[i]}"
            if j < n_col - 1:
                mat_str += ", "
        if i < n_row - 1:
            mat_str += ", "
    mat_str += "};\n"

    return mat_str

def gen_tp(config_tp):
    tp_type = config_tp["tp_type"]
    SEW = config_tp["SEW"]
    VLMAX = config_tp["VLMAX"]

    tp = []

    if SEW != 32:
        VLMAX = int(VLMAX * (SEW / 32))

    if tp_type == "random":
        tp = np.random.randint(0, 2**32, size=(2, VLMAX), dtype=np.uint32)
    else:
        raise ValueError(f"Unknown tp_type: {tp_type}")

    return tp

def gen_defines(config_tp):
    VLMAX = config_tp["VLMAX"]
    SEW = config_tp["SEW"]

    defines_str = f"#define VLMAX {VLMAX}\n"
    defines_str += f"#define SEW {SEW}\n"

    return defines_str

def main():
    file_path = pathlib.Path(__file__).parent.parent / "data"
    file_path.mkdir(parents=True, exist_ok=True)
    
    parser = argparse.ArgumentParser(description="Generate test pattern data for FPU.")
    parser.add_argument("config_file", type=str, help="Path to the configuration JSON file.")
    args = parser.parse_args()

    with open(args.config_file, 'r') as f:
        config_tp = hjson.loads(f.read())

    config_tp["VLMAX"] = int((config_tp["VLEN"] / config_tp["SEW"]) * config_tp["LMUL"])

    tp = gen_tp(config_tp)
    tp_str = mat2str(tp, "tp")

    output_file = file_path / f"tp_data_{config_tp['tp_type']}.h"
    with open(output_file, 'w') as f:
        f.write(gen_defines(config_tp) + "\n")
        f.write(tp_str)


    print(f"Test pattern data generated and saved to {output_file}.")


if __name__ == "__main__":
    main()
