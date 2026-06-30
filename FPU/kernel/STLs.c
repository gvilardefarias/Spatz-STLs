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


void test_setup(double *a, double *b, vconfig_t *vconfig){
  vsetvl(vconfig);

  vle64_v0(a);
  vle64_v8(b);
}

inline void test_procedure(double *a, double *b, const int strd_mult){
  for(int i = SNRT_NFPU_PER_CORE * strd_mult; i < SNRT_NFPU_PER_CORE * (strd_mult + 1); i++){
    test_op();
    vslide1down_v0(a[i]); // i is equal to the number of strides performed
    vslide1down_v8(b[i]);
  }
}

int test(double *a, double *b){
  int test_count = 0;
  vconfig_t vconfig = {
      .AVL   = -1,
      .vtype = {.sew = e64, .lmul = m8},
  };

#ifdef TEST_SEW_64
  test_setup(a, b, &vconfig);
  test_procedure(a, b, test_count++);
#endif

#ifdef TEST_SEW_32
  vconfig.vtype.sew = e32;
  vsetvl(&vconfig);
  test_procedure(a, b, test_count++);
#endif

#ifdef TEST_SEW_16
  vconfig.vtype.sew = e16;
  vsetvl(&vconfig);
  test_procedure(a, b, test_count++);
#endif

#ifdef TEST_SEW_8
  vconfig.vtype.sew = e8;
  vsetvl(&vconfig);
  test_procedure(a, b, test_count++);
#endif

  return 1;
}
