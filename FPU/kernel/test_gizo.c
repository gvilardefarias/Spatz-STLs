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

#if TEST_TARGET == TEST_GIZO || TEST_TARGET == TEST_ALL
int test_gizo_64(uint64_t cst_0, uint64_t cst_1, uint64_t* tp_0, uint64_t* tp_1, uint64_t* tp_2, uint64_t* tp_3){
  DoubleInt di_cst_0, di_cst_1;
  const uint64_t exp_0 = (uint64_t) ((uint64_t) 0x7F) << 52;
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
    safe_test_procedure_64(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down64_v8(*((uint64_t*) b_strd));
    b_strd += 8;
    if(b_strd >= b_end) b_strd = b_start;
  }
  // Far-path s1 test
  vand_vx_v8(mask_exp_b0_l);
  for(int i = 0;i < 53;i++){
    safe_test_procedure_64(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down64_v8(*((uint64_t*) b_strd));
    b_strd += 8;
    if(b_strd >= b_end) b_strd = b_start;
  }
  vor_vx_v8(mask_exp_b0_h);

  // Close-path s2 test
  safe_test_procedure64_f_v8(&b_strd, b_start, b_end, di_cst_0.d);
  // Far-path s2 test
  di_cst_0.i &= mask_exp_b0_l;
  safe_test_procedure64_f_v8(&b_strd, b_start, b_end, di_cst_0.d);

  // Close-path s4 test
  vle64_v8(tp_3);
  vor_vx_v8(exp_0);
  b_end = (uint8_t*) (tp_3 + 53);
  b_start = (uint8_t*) tp_3;
  b_strd = (uint8_t*) tp_3;
  for(int i = 0;i < 53;i++){
    safe_test_procedure_64(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down64_v8(*((uint64_t*) b_strd));
    b_strd += 8;
    if(b_strd >= b_end) b_strd = b_start;
  }
  // Far-path s4 test
  vand_vx_v8(mask_exp_b0_l);
  for(int i = 0;i < 53;i++){
    safe_test_procedure_64(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down64_v8(*((uint64_t*) b_strd));
    b_strd += 8;
    if(b_strd >= b_end) b_strd = b_start;
  }

  // Close-path s3 test
  vle64_v8(tp_2);
  vor_vx_v8(exp_0);
  b_strd = (uint8_t*) tp_2;
  test_procedure64_f_v8(&b_strd, di_cst_1.d);
  // Far-path s3 test
  di_cst_1.i &= mask_exp_b0_l;
  test_procedure64_f_v8(&b_strd, di_cst_1.d);

  return 1;
}

int test_gizo_32(uint32_t cst_0, uint32_t cst_1, uint32_t* tp_0, uint32_t* tp_1, uint32_t* tp_2, uint32_t* tp_3){
  Float32Int di_cst_0, di_cst_1;
  const uint32_t exp_0 = (uint32_t) 0x7F << 23;
  const uint32_t mask_exp_b0_l = ~((uint32_t) 1 << 23);
  const uint32_t mask_exp_b0_h =  ((uint32_t) 1 << 23);

  cst_0 |= exp_0;
  cst_1 |= exp_0;

  di_cst_0.i = cst_0;
  di_cst_1.i = cst_1;

  vconfig_t vconfig = {.AVL = 24,
                       .vtype = {.sew = E32, .lmul = 8}};

  uint8_t *a_end = (uint8_t*) (tp_0 + 24);
  uint8_t *b_end = (uint8_t*) (tp_1 + 24);
  uint8_t *a_start = (uint8_t*) tp_0;
  uint8_t *b_start = (uint8_t*) tp_1;

  vsetvl(&vconfig);

  tp_load_32(tp_0, tp_1);
  vor_vx_v0(exp_0);
  vor_vx_v8(exp_0);

  uint8_t *a_strd = (uint8_t*) tp_0;
  uint8_t *b_strd = (uint8_t*) tp_1;
  // Close-path s1 test
  for(int i = 0;i < 24;i++){
    safe_test_procedure_32(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down32_v8(*((uint32_t*) b_strd));
    b_strd += (32 / 8);
    if(b_strd >= b_end) b_strd = b_start;
  }
  // Far-path s1 test
  vand_vx_v8(mask_exp_b0_l);
  for(int i = 0;i < 24;i++){
    safe_test_procedure_32(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down32_v8(*((uint32_t*) b_strd));
    b_strd += (32 / 8);
    if(b_strd >= b_end) b_strd = b_start;
  }
  vor_vx_v8(mask_exp_b0_h);

  // Close-path s2 test
  safe_test_procedure32_f_v8(&b_strd, b_start, b_end, di_cst_0.d);
  // Far-path s2 test
  di_cst_0.i &= mask_exp_b0_l;
  safe_test_procedure32_f_v8(&b_strd, b_start, b_end, di_cst_0.d);

  // Close-path s4 test
  vle32_v8(tp_3);
  vor_vx_v8(exp_0);
  b_end = (uint8_t*) (tp_3 + 24);
  b_start = (uint8_t*) tp_3;
  b_strd = (uint8_t*) tp_3;
  for(int i = 0;i < 24;i++){
    safe_test_procedure_32(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down32_v8(*((uint32_t*) b_strd));
    b_strd += (32 / 8);
    if(b_strd >= b_end) b_strd = b_start;
  }
  // Far-path s4 test
  vand_vx_v8(mask_exp_b0_l);
  for(int i = 0;i < 24;i++){
    safe_test_procedure_32(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down32_v8(*((uint32_t*) b_strd));
    b_strd += (32 / 8);
    if(b_strd >= b_end) b_strd = b_start;
  }

  // Close-path s3 test
  vle32_v8(tp_2);
  vor_vx_v8(exp_0);
  b_strd = (uint8_t*) tp_2;
  test_procedure32_f_v8(&b_strd, di_cst_1.d);
  // Far-path s3 test
  di_cst_1.i &= mask_exp_b0_l;
  test_procedure32_f_v8(&b_strd, di_cst_1.d);

  return 1;
}

int test_gizo_16(uint16_t cst_0, uint16_t cst_1, uint16_t* tp_0, uint16_t* tp_1, uint16_t* tp_2, uint16_t* tp_3){
  Float16Int di_cst_0, di_cst_1;
  const uint16_t exp_0 = (uint16_t) ((uint16_t) 0x7F) << 10;
  const uint16_t mask_exp_b0_l = ~((uint16_t) 1 << 10);
  const uint16_t mask_exp_b0_h =  ((uint16_t) 1 << 10);

  cst_0 |= exp_0;
  cst_1 |= exp_0;

  di_cst_0.i = cst_0;
  di_cst_1.i = cst_1;

  vconfig_t vconfig = {.AVL = 11,
                       .vtype = {.sew = E16, .lmul = 8}};

  uint8_t *a_end = (uint8_t*) (tp_0 + 11);
  uint8_t *b_end = (uint8_t*) (tp_1 + 11);
  uint8_t *a_start = (uint8_t*) tp_0;
  uint8_t *b_start = (uint8_t*) tp_1;

  vsetvl(&vconfig);

  tp_load_16(tp_0, tp_1);
  vor_vx_v0(exp_0);
  vor_vx_v8(exp_0);

  uint8_t *a_strd = (uint8_t*) tp_0;
  uint8_t *b_strd = (uint8_t*) tp_1;
  // Close-path s1 test
  for(int i = 0;i < 11;i++){
    safe_test_procedure_16(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down16_v8(*((uint16_t*) b_strd));
    b_strd += (16 / 8);
    if(b_strd >= b_end) b_strd = b_start;
  }
  // Far-path s1 test
  vand_vx_v8(mask_exp_b0_l);
  for(int i = 0;i < 11;i++){
    safe_test_procedure_16(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down16_v8(*((uint16_t*) b_strd));
    b_strd += (16 / 8);
    if(b_strd >= b_end) b_strd = b_start;
  }
  vor_vx_v8(mask_exp_b0_h);

  // Close-path s2 test
  safe_test_procedure16_f_v8(&b_strd, b_start, b_end, di_cst_0.d);
  // Far-path s2 test
  di_cst_0.i &= mask_exp_b0_l;
  safe_test_procedure16_f_v8(&b_strd, b_start, b_end, di_cst_0.d);

  // Close-path s4 test
  vle16_v8(tp_3);
  vor_vx_v8(exp_0);
  b_end = (uint8_t*) (tp_3 + 11);
  b_start = (uint8_t*) tp_3;
  b_strd = (uint8_t*) tp_3;
  for(int i = 0;i < 11;i++){
    safe_test_procedure_16(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down16_v8(*((uint16_t*) b_strd));
    b_strd += (16 / 8);
    if(b_strd >= b_end) b_strd = b_start;
  }
  // Far-path s4 test
  vand_vx_v8(mask_exp_b0_l);
  for(int i = 0;i < 11;i++){
    safe_test_procedure_16(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down16_v8(*((uint16_t*) b_strd));
    b_strd += (16 / 8);
    if(b_strd >= b_end) b_strd = b_start;
  }

  // Close-path s3 test
  vle16_v8(tp_2);
  vor_vx_v8(exp_0);
  b_strd = (uint8_t*) tp_2;
  test_procedure16_f_v8(&b_strd, di_cst_1.d);
  // Far-path s3 test
  di_cst_1.i &= mask_exp_b0_l;
  test_procedure16_f_v8(&b_strd, di_cst_1.d);

  return 1;
}

int test_gizo_8(uint8_t* cst_0, uint8_t* cst_1, uint8_t* tp_0, uint8_t* tp_1, uint8_t* tp_2, uint8_t* tp_3){
  const uint8_t exp_0 = (uint8_t) ((uint8_t) 0xE) << 4;
  const uint8_t mask_exp_b0_l = ~((uint8_t) 1 << 4);
  const uint8_t mask_exp_b0_h =  ((uint8_t) 1 << 4);

  vconfig_t vconfig = {.AVL = 5,
                       .vtype = {.sew = E8, .lmul = 8}};

  uint8_t *a_end = (uint8_t*) (tp_0 + 5);
  uint8_t *b_end = (uint8_t*) (tp_1 + 5);
  uint8_t *a_start = (uint8_t*) tp_0;
  uint8_t *b_start = (uint8_t*) tp_1;

  vsetvl(&vconfig);

  tp_load_8(tp_0, tp_1);
  vor_vx_v0(exp_0);
  vor_vx_v8(exp_0);

  uint8_t *a_strd = tp_0;
  uint8_t *b_strd = tp_1;
  // Close-path s1 test
  for(int i = 0;i < 5;i++){
    safe_test_procedure_8(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down8_v8(*b_strd);
    b_strd += 1;
    if(b_strd >= b_end) b_strd = b_start;
  }
  // Far-path s1 test
  vand_vx_v8(mask_exp_b0_l);
  for(int i = 0;i < 5;i++){
    safe_test_procedure_8(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down8_v8(*b_strd);
    b_strd += 1;
    if(b_strd >= b_end) b_strd = b_start;
  }
  vor_vx_v8(mask_exp_b0_h);

  // Close-path s4 test
  vle8_v8(tp_3);
  vor_vx_v8(exp_0);
  b_end = (uint8_t*) (tp_3 + 5);
  b_start = (uint8_t*) tp_3;
  b_strd = (uint8_t*) tp_3;
  b_strd = tp_3;
  for(int i = 0;i < 5;i++){
    safe_test_procedure_8(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down8_v8(*b_strd);
    b_strd += (8 / 8);
    if(b_strd >= b_end) b_strd = b_start;
  }
  // Far-path s4 test
  vand_vx_v8(mask_exp_b0_l);
  for(int i = 0;i < 5;i++){
    safe_test_procedure_8(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
    vslide1down8_v8(*b_strd);
    b_strd += (8 / 8);
    if(b_strd >= b_end) b_strd = b_start;
  }

  // Close-path s2 test
  tp_load_8(cst_0, tp_1);
  vor_vx_v0(exp_0);
  vor_vx_v8(exp_0);
  a_end = (cst_0 + 5);
  b_end = (tp_1 + 5);
  a_start = cst_0;
  b_start = tp_1;
  a_strd = cst_0;
  b_strd = tp_1;
  safe_test_procedure_8(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
  // Far-path s2 test
  vand_vx_v8(mask_exp_b0_l);
  safe_test_procedure_8(&a_strd, &b_strd, a_start, b_start, a_end, b_end);

  // Close-path s3 test
  tp_load_8(cst_1, tp_2);
  vor_vx_v0(exp_0);
  vor_vx_v8(exp_0);
  a_end = (cst_1 + 5);
  b_end = (tp_2 + 5);
  a_start = cst_1;
  b_start = tp_2;
  a_strd = cst_1;
  b_strd = tp_2;
  safe_test_procedure_8(&a_strd, &b_strd, a_start, b_start, a_end, b_end);
  // Far-path s3 test
  vand_vx_v8(mask_exp_b0_l);
  safe_test_procedure_8(&a_strd, &b_strd, a_start, b_start, a_end, b_end);

  return 1;
}
#endif