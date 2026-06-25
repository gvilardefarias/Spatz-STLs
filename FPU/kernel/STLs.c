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

test_t test_ar[STL_NUM_TEST] = {TEST_MUL};


void test_setup(double *a, double *b, vconfig_t *vconfig){
  vsetvl(vconfig);

  vle64_v0(a);
  vle64_v8(b);
}

int test_mul(double *a, double *b){
  vconfig_t vconfig = {
      .AVL   = -1,
      .vtype = {.sew = e64, .lmul = m8},
  };

  test_setup(a, b, &vconfig);

  for(int i = 0; i < FPU_COUNT; i++){
    vfmul_vv_v16_v0_v8();
    vslide1down_v0(a[i]); // i is equal to the number of strides performed
    vslide1down_v8(b[i]);
  }
}

int test_procedure(double *a, double *b, double *c, test_t test){
  switch (test) {
  case TEST_MUL:
    return test_mul(a, b);
  case TEST_ADD:
    return 0;
  default:
    return -1;
  }
}