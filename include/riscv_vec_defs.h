
// Copyright 2026 Politecnico di Torino
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Author: Gustavo Vilar de Farias, Politecnico di Torino, Italy

#ifndef RISCV_VEC_DEFS_H
#define RISCV_VEC_DEFS_H

typedef enum {
    e8  = 0,
    e16 = 1,
    e32 = 2,
    e64 = 3
} sew_t;

typedef enum {
    m1  = 0,
    m2  = 1,
    m4  = 2,
    m8  = 3, // registers available: v0, v8, v16, and v24
    mf2 = 5,
    mf4 = 6,
    mf8 = 7
} lmul_t;

typedef struct {
    sew_t  sew;
    lmul_t lmul;
} vtype_t;

typedef struct {
    int     AVL; // AVL = -1 -> rs1 = x0 = 0 and rd1 != x0 for VLMAX
    vtype_t vtype;
} vconfig_t;

int vtype_to_int(vtype_t *vtype);

void vsetvl(vconfig_t *vconfig);

#endif