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

#include "STLs.h"

inline void tp_load(uint32_t *a, uint32_t *b){
  #if TEST_START_SEW == E64
    vle64_v0(a);
    vle64_v8(b);
  #elif TEST_START_SEW == E32
    vle32_v0(a);
    vle32_v8(b);
  #elif TEST_START_SEW == E16
    vle16_v0(a);
    vle16_v8(b);
  #elif TEST_START_SEW == E8
    vle8_v0(a);
    vle8_v8(b);
  #endif
}

inline void test_procedure(uint8_t *a, uint8_t *b, const unsigned int SEW){
  #if SNRT_NFPU_PER_CORE > 1
    const unsigned int strd_mult = SEW / 8; // Number of bytes per element
  #endif

  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down_v0(*a); // i is equal to the number of strides performed
      vslide1down_v8(*b);

      a += strd_mult;
      b += strd_mult;

      test_op();
    }
  #endif
}

inline void safe_test_procedure(uint8_t* a, uint8_t* b, uint8_t* a_start, uint8_t* b_start, uint8_t* a_end, uint8_t* b_end, const unsigned int SEW){
  #if SNRT_NFPU_PER_CORE > 1
    const unsigned int strd_mult = SEW / 8; // Number of bytes per element
  #endif

  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down_v0(*a); // i is equal to the number of strides performed
      vslide1down_v8(*b);

      a += strd_mult;
      b += strd_mult;

      if(a >= a_end) a = a_start;
      if(b >= b_end) b = b_start;

      test_op();
    }
  #endif
}

inline void safe_test_procedure_f_v8(uint8_t* b, uint8_t* b_start, uint8_t* b_end, double d, const unsigned int sew){
  #if SNRT_NFPU_PER_CORE > 1
    const unsigned int strd_mult = sew / 8; // number of bytes per element
  #endif

  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down_v8(*b);

      b += strd_mult;

      if(b >= b_end) b = b_start;

      test_op_f_v8(d);
    }
  #endif
}

inline void test_procedure_f_v8(uint8_t* b, double d, const unsigned int sew){
  #if SNRT_NFPU_PER_CORE > 1
    const unsigned int strd_mult = sew / 8; // number of bytes per element
  #endif

  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down_v8(*b);

      b += strd_mult;

      test_op_f_v8(d);
    }
  #endif
}

#if TEST_TARGET == TEST_MIX || TEST_TARGET == TEST_ALL
int test_mix(uint32_t *a, uint32_t *b){
  int test_count;
  unsigned int avl;
  uint8_t *a_strd = (uint8_t *)a; //  TODO: change to declare it only if NFPU > 1
  uint8_t *b_strd = (uint8_t *)b;

  vtype_t vtype = {.sew = TEST_START_SEW, .lmul = M8};
  vsetvtype(&vtype);

  for(int i = 0; i < TP_MUL; i++){
    tp_load(a, b);
    test_count = 0;

    #if TEST_SEW_64 == 1
      test_procedure(a_strd, b_strd, 64);
    #endif

    #if TEST_SEW_32 == 1
      #if TEST_SEW_64 == 1
        vtype.sew = e32;
        vsetvtype(&vtype);
      #endif
      test_procedure(a_strd, b_strd, 32);
    #endif

    #if TEST_SEW_16 == 1
      #if TEST_SEW_64 + TEST_SEW_32 > 0
        vtype.sew = e16;
        vsetvtype(&vtype);
      #endif
      test_procedure(a_strd, b_strd, 16);
    #endif

    #if TEST_SEW_8 == 1
      #if TEST_SEW_64 + TEST_SEW_32 + TEST_SEW_16 > 0
        vtype.sew = e8;
        vsetvtype(&vtype);
      #endif
      test_procedure(a_strd, b_strd, 8);
    #endif

    #if TP_MUL > 1 
      a += VLMAX_32;
      b += VLMAX_32;
      a_strd = (uint8_t *)a;
      b_strd = (uint8_t *)b;

      #if (TEST_SEW_64 + TEST_SEW_32 + TEST_SEW_16 + TEST_SEW_8) > 1
        vtype.sew = TEST_START_SEW;
        vsetvtype(&vtype);
      #endif
    #endif
  }

  return 1;
}
#endif

#if TEST_TARGET == TEST_GIZO || TEST_TARGET == TEST_ALL
int test_gizo(uint64_t cst_0, uint64_t cst_1, uint64_t* tp_0, uint64_t* tp_1, uint64_t* tp_2, uint64_t* tp_3){
  DoubleInt di_cst_0, di_cst_1;
  const uint64_t exp_0 = (uint64_t) 0x7F << 52;
  const uint64_t mask_exp_b0_l = ~((uint64_t) 1 << 52);
  const uint64_t mask_exp_b0_h =  ((uint64_t) 1 << 52);

  cst_0 |= exp_0;
  cst_1 |= exp_0;

  di_cst_0.i = cst_0;
  di_cst_1.i = cst_1;

  vconfig_t vconfig = {.AVL = 53,
                       .vtype = {.sew = E64, .lmul = 8}};

  uint8_t *a_end = (uint8_t*) (tp_0 + 53);
  uint8_t *b_end = (uint8_t*) (tp_1 + 53);
  uint8_t *a_start = (uint8_t*) tp_0;
  uint8_t *b_start = (uint8_t*) tp_1;

  vsetvl(&vconfig);

  vle64_v0(tp_0);
  vle64_v8(tp_1);
  vor_vx_v0(exp_0);
  vor_vx_v8(exp_0);

  uint8_t *a_strd = (uint8_t*) tp_0;
  uint8_t *b_strd = (uint8_t*) tp_1;
  // Close-path s1 test
  for(int i = 0;i < 53;i++){
    safe_test_procedure(a_strd, b_strd, a_start, b_start, a_end, b_end, 64);
    vslide1down_v8(*b_strd);
    b_strd += 8;
    if(b_strd >= b_end) b_strd = b_start;
  }
  // Far-path s1 test
  vand_vx_v8(mask_exp_b0_l);
  for(int i = 0;i < 53;i++){
    safe_test_procedure(a_strd, b_strd, a_start, b_start, a_end, b_end, 64);
    vslide1down_v8(*b_strd);
    b_strd += 8;
    if(b_strd >= b_end) b_strd = b_start;
  }
  vor_vx_v8(mask_exp_b0_h);

  // Close-path s2 test
  safe_test_procedure_f_v8(b_strd, b_start, b_end, di_cst_0.d, 64);
  // Far-path s2 test
  di_cst_0.i &= mask_exp_b0_l;
  safe_test_procedure_f_v8(b_strd, b_start, b_end, di_cst_0.d, 64);

  // Close-path s4 test
  vle64_v8(tp_3);
  vor_vx_v8(exp_0);
  b_end = (uint8_t*) (tp_3 + 53);
  b_start = (uint8_t*) tp_3;
  b_strd = (uint8_t*) tp_3;
  for(int i = 0;i < 53;i++){
    safe_test_procedure(a_strd, b_strd, a_start, b_start, a_end, b_end, 64);
    vslide1down_v8(*b_strd);
    b_strd += 8;
    if(b_strd >= b_end) b_strd = b_start;
  }
  // Far-path s4 test
  vand_vx_v8(mask_exp_b0_l);
  for(int i = 0;i < 53;i++){
    safe_test_procedure(a_strd, b_strd, a_start, b_start, a_end, b_end, 64);
    vslide1down_v8(*b_strd);
    b_strd += 8;
    if(b_strd >= b_end) b_strd = b_start;
  }

  // Close-path s3 test
  vle64_v8(tp_2);
  vor_vx_v8(exp_0);
  b_strd = (uint8_t*) tp_2;
  test_procedure_f_v8(b_strd, di_cst_1.d, 64);
  // Far-path s3 test
  di_cst_1.i &= mask_exp_b0_l;
  test_procedure_f_v8(b_strd, di_cst_1.d, 64);

  return 1;
}
#endif