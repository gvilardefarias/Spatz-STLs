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
#include <stdint.h>


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

// TODO change to inline after debug
inline void test_procedure(uint8_t *a, uint8_t *b, const unsigned int SEW){
//  PRINTF("Starting test procedure for SEW = %d\n", SEW);
  #if SNRT_NFPU_PER_CORE > 1
    const unsigned int strd_mult = SEW / 8; // Number of bytes per element
  #endif
      /*
  const unsigned int cid = snrt_cluster_core_idx();
  uint32_t *a_dbg;
  uint32_t *b_dbg;
  if (cid == 0) {
    a_dbg = (uint32_t *)snrt_l1alloc(SNRT_VLEN * LMUL / 8);
    b_dbg = (uint32_t *)snrt_l1alloc(SNRT_VLEN * LMUL / 8);
  }
    */

//  PRINTF("strd_mult = %d\n", strd_mult);
  test_op();
  for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
//    PRINTF("test 0\n");
    #if SNRT_NFPU_PER_CORE > 1
      vslide1down_v0(*a); // i is equal to the number of strides performed
      vslide1down_v8(*b);
      /*
      if(SEW == 64 && snrt_cluster_core_idx() == 0){
        vse64_v0(a_dbg);
        vse64_v8(b_dbg);
        for(int j = 0; j < 4; j++){
          PRINTF("a_dbg[%d] = %d | ", j, *((uint64_t*)a_dbg + j));
        }
      PRINTF("\n");
      }
      */
      a += strd_mult;
      b += strd_mult;
    #endif

    test_op();
  }
}

int test(uint32_t *a, uint32_t *b){
  int test_count;
  unsigned int avl;
  uint8_t *a_strd = (uint8_t *)a; //  TODO: change to declare it only if NFPU > 1
  uint8_t *b_strd = (uint8_t *)b;

  vtype_t vtype = {.sew = TEST_START_SEW, .lmul = M8};
  vsetvtype(&vtype);
//  PRINTF("vtype.sew = %d | vtype.lmul = %d\n", vtype.sew, vtype.lmul);

  for(int i = 0; i < TP_MUL; i++){
//    PRINTF("Test %d/%d\n", i + 1, TP_MUL);
    tp_load(a, b);
//    PRINTF("data loaded, sew: %d, sew8: %d, sew16: %d, sew32: %d, sew64:%d\n", TEST_START_SEW, e8, e16, e32, e64);
//    PRINTF("%d %d %d %d\n", TEST_START_SEW == e8, TEST_START_SEW == e16, TEST_START_SEW == e32, TEST_START_SEW == e64);
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
