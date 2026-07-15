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

#ifndef STLS_H
#define STLS_H

#define VLMAX_32 (SNRT_VLEN / 32) * LMUL

// Define test targets
#define TEST_ALL 0
#define TEST_MUL 1
#define TEST_ADD 2

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

int test(uint32_t *a, uint32_t *b);

#if TEST_TARGET == TEST_ALL
#define test_op() vfmul_vv_v16_v0_v8(); \
                  vfadd_vv_v16_v0_v8(); \
                  vfsub_vv_v16_v0_v8(); 
// TODO add the other functions and mode
#endif

#endif