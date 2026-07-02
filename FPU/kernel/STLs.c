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
#include <stddef.h>


void tp_load(double *a, double *b){
  vle64_v0(a);
  vle64_v8(b);
}

inline void test_procedure(double *a, double *b, const int strd_mult){
  for(int i = SNRT_NFPU_PER_CORE * strd_mult; i < SNRT_NFPU_PER_CORE * (strd_mult + 1); i++){
    test_op();

    #if SNRT_NFPU_PER_CORE > 1
    vslide1down_v0(a[i]); // i is equal to the number of strides performed
    vslide1down_v8(b[i]);
    #endif
  }
}

int test(double *a, double *b){
  int test_count = 0;
  unsigned int avl;
  vtype_t vtype = {.sew = TEST_START_SEW, .lmul = m8};

  for(int i = 0; i < TP_MUL; i++){
    vsetvtype(&vtype);
    tp_load(a, b); // TODO: load the other part of the array as i advances

    #if TEST_SEW_64 == 1
      test_procedure(a, b, test_count++);
    #endif

    #if TEST_SEW_32 == 1
      #if TEST_SEW_64 == 1
        vtype.sew = e32;
        vsetvtype(&vtype);
      #endif
      test_procedure(a, b, test_count++);
    #endif

    #if TEST_SEW_16 == 1
      #if TEST_SEW_64 + TEST_SEW_32 > 1
        vtype.sew = e16;
        vsetvtype(&vtype);
      #endif
      test_procedure(a, b, test_count++);
    #endif

    #if TEST_SEW_8 == 1
      #if TEST_SEW_64 + TEST_SEW_32 + TEST_SEW_16 > 1
        vtype.sew = e8;
        vsetvtype(&vtype);
      #endif
      test_procedure(a, b, test_count++);
    #endif

    #if TP_MUL > 1 && (TEST_SEW_64 + TEST_SEW_32 + TEST_SEW_16 + TEST_SEW_8) > 1
      vtype.sew = TEST_START_SEW;
      vsetvtype(&vtype);
    #endif
  }


  return 1;
}
