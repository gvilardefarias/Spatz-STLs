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

#include "riscv_vec_defs.h"

#ifndef STLS_H
#define STLS_H

#define FPU_COUNT 4
#define STL_NUM_TEST 1

typedef enum {
    TEST_MUL,
    TEST_ADD
} test_t;

void test_setup(double *a, double *b, vconfig_t *vconfig);
void test_stride(double *a, double *b);
int test_mul(double *a, double *b);
int test_procedure(double *a, double *b, double *c, test_t test);

#endif
