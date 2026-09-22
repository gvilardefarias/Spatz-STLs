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

#include "riscv_float_defs.c"

#if TEST_TARGET == TEST_MIX || TEST_TARGET == TEST_ALL
inline void tp_load_mix(uint32_t *a, uint32_t *b){
  #if TEST_START_SEW == E64
    tp_load_64((uint64_t*) a,(uint64_t*) b);
  #elif TEST_START_SEW == E32
    tp_load_32(a, b);
  #elif TEST_START_SEW == E16
    tp_load_16((uint16_t* ) a,(uint16_t* ) b);
  #elif TEST_START_SEW == E8
    tp_load_8((uint8_t*) a,(uint8_t*) b);
  #endif
}

int test_mix(uint32_t *a, uint32_t *b){
  unsigned int avl;
  uint8_t *a_strd = (uint8_t *)a;
  uint8_t *b_strd = (uint8_t *)b;

  vtype_t vtype = {.sew = TEST_START_SEW, .lmul = M8};
  vsetvtype(&vtype);

  // TODO: use safe procedures when FPU count is bigger than 2 ?
  for(int i = 0;i < TP_MUL;i++){
    if(i == 2){
      printf("original: \n");
      for(int j = 0;j < 64;j++){
        printf("%llx ", ((uint64_t*) b)[j]);
      }
      printf("\n");
    }

    tp_load_mix(a, b);
    fsrm((i + 1)%5);  // Starting with 1 so with 4 TP_MUL we arrive until 4 and exercise all the bits
    csrw_fmode((i)%4);

    #if TEST_SEW_64 == 1
      test_procedure_64(&a_strd, &b_strd);

      vslide1down64_v0(*( (uint64_t*) (a_strd)));   // Creating offset for performing different operations
      a_strd += 8;
      test_narrow_procedure_64(&a_strd, &b_strd);

      test_red_op();
    #endif

    #if TEST_SEW_32 == 1
      #if TEST_SEW_64 == 1
        vtype.sew = e32;
        vsetvtype(&vtype);
      #endif
      test_procedure_32(&a_strd, &b_strd);

      vslide1down32_v0(*( (uint32_t*) (a_strd)));   // Creating offset for performing different operations
      a_strd += 4;
      test_narrow_procedure_32(&a_strd, &b_strd);

      test_red_op();
    #endif

    #if TEST_SEW_16 == 1
      #if TEST_SEW_64 + TEST_SEW_32 > 0
        vtype.sew = e16;
        vsetvtype(&vtype);
      #endif
      test_procedure_16(&a_strd, &b_strd);

      vslide1down16_v0(*( (uint16_t*) (a_strd)));   // Creating offset for performing different operations
      a_strd += 2;
      test_narrow_procedure_16(&a_strd, &b_strd);

      test_red_op();
    #endif

    #if TEST_SEW_8 == 1
      #if TEST_SEW_64 + TEST_SEW_32 + TEST_SEW_16 > 0
        vtype.sew = e8;
        vsetvtype(&vtype);
      #endif
      test_procedure_8(&a_strd, &b_strd);

      test_red_op();
    #endif

    vtype.lmul = M4;
    vsetvtype(&vtype);

    #if TEST_SEW_8 == 1
      test_widen_procedure_8(&a_strd, &b_strd);
    #endif

    #if TEST_SEW_16 == 1
      #if TEST_SEW_8 == 1
        vtype.sew = e16;
        vsetvtype(&vtype);
      #endif
      test_widen_procedure_16(&a_strd, &b_strd);
    #endif

    
    #if TEST_SEW_32 == 1
      #if TEST_SEW_8 + TEST_SEW_16 > 0
        vslide1down16_v8(*( (uint16_t*) (a_strd)));   // Deal with missaligment
        a_strd += 2;

        vtype.sew = e32;
        vsetvtype(&vtype);
      #endif
      test_widen_procedure_32(&a_strd, &b_strd);
    #endif

    if(i == 2){
      uint64_t test[64];
      vse64_v8(test);
      printf("after slides: \n");
      for(int j = 0;j < 64;j++){
        printf("%llx ", test[j]);
      }
      printf("\n");
    }

    #if TP_MUL > 1 
      a += VLMAX_32;
      b += VLMAX_32;
      a_strd = (uint8_t *)a;
      b_strd = (uint8_t *)b;

      vtype.lmul = M8;

      #if TEST_START_SEW == E64
        vtype.sew = e64;
      #endif

      vsetvtype(&vtype);
    #endif
  }

  return 1;
}
#endif