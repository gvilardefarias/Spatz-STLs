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

#include "riscv_vec_defs.c"
#include <stdint.h>

#ifndef STLS_H
#define STLS_H

typedef union {
    double d;
    uint64_t i;
} DoubleInt;

#define VLMAX_32 (SNRT_VLEN / 32) * LMUL

// Define test targets
#define TEST_ALL 0
#define TEST_MIX 1
#define TEST_GIZO 2
//#define TEST_ADD 2

// Define test sew
#if TEST_SEW == 0 // All SEWs
    #define TEST_SEW_8  1
    #define TEST_SEW_16 1
    #define TEST_SEW_32 1
    #define TEST_SEW_64 1

    #define TEST_START_SEW E64
#elif TEST_SEW == 72
    #define TEST_SEW_8  1
    #define TEST_SEW_16 0
    #define TEST_SEW_32 0
    #define TEST_SEW_64 1

    #define TEST_START_SEW E64
#else
    #define TEST_SEW_64 (TEST_SEW == 64)
    #define TEST_SEW_32 (TEST_SEW == 32)
    #define TEST_SEW_16 (TEST_SEW == 16)
    #define TEST_SEW_8  (TEST_SEW ==  8)
    
    #if TEST_SEW_8 == 1
        #define TEST_START_SEW E8
    #endif
    #if TEST_SEW_16 == 1
        #define TEST_START_SEW E16
    #endif
    #if TEST_SEW_32 == 1
        #define TEST_START_SEW E32
    #endif
    #if TEST_SEW_64 == 1
        #define TEST_START_SEW E64
    #endif
#endif

int test_mix(uint32_t *a, uint32_t *b);
int test_gizo(uint64_t cst_0, uint64_t cst_1, uint64_t* tp_0, uint64_t* tp_1, uint64_t* tp_2, uint64_t* tp_3); // Gizoupolos paper

#if TEST_TARGET == TEST_ALL || TEST_TARGET == TEST_MIX
    #define test_op() vfmul_vv_v16_v0_v8(); \
                      vfadd_vv_v16_v0_v8(); \
                      vfsub_vv_v16_v0_v8(); 
    // TODO add the other functions and mode
    #define test_op_f_v8(d) vfsub_vf_v16_v8(d); \
                            vfadd_vf_v16_v8(d); \
                            vfmul_vf_v16_v8(d); // TODO increase it
#elif TEST_TARGET == TEST_GIZO
    #define test_op() vfmul_vv_v16_v0_v8(); \
                      vfadd_vv_v16_v0_v8(); \
                      vfsub_vv_v16_v0_v8(); 
    #define test_op_f_v8(d) vfsub_vf_v16_v8(d); \
                            vfadd_vf_v16_v8(d); \
                            vfmul_vf_v16_v8(d);
#endif

#endif