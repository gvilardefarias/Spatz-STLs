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

inline void tp_load_64(uint64_t *a, uint64_t *b){
  vle64_v0(a);
  vle64_v8(b);
}
inline void tp_load_32(uint32_t *a, uint32_t *b){
  vle32_v0(a);
  vle32_v8(b);
}
inline void tp_load_16(uint16_t *a, uint16_t *b){
  vle16_v0(a);
  vle16_v8(b);
}
inline void tp_load_8(uint8_t *a, uint8_t *b){
  vle8_v0(a);
  vle8_v8(b);
}

inline void test_procedure_64(uint8_t **a, uint8_t **b){
  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down64_v0(*((uint64_t*) (*a) )); // i is equal to the number of strides performed
      vslide1down64_v8(*((uint64_t*) (*b) ));

      *a += 8;
      *b += 8;

      test_op();
    }
  #endif
}
inline void test_procedure_32(uint8_t **a, uint8_t **b){
  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down32_v0(*((uint32_t*) (*a) )); // i is equal to the number of strides performed
      vslide1down32_v8(*((uint32_t*) (*b) ));

      *a += 4;
      *b += 4;

      test_op();
    }
  #endif
}
inline void test_procedure_16(uint8_t **a, uint8_t **b){
  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down16_v0(*((uint16_t*) (*a) )); // i is equal to the number of strides performed
      vslide1down16_v8(*((uint16_t*) (*b) ));

      *a += 2;
      *b += 2;

      test_op();
    }
  #endif
}
inline void test_procedure_8(uint8_t **a, uint8_t **b){
  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down8_v0(**(a)); // i is equal to the number of strides performed
      vslide1down8_v8(**(b));

      *a += 1;
      *b += 1;

      test_op();
    }

//    #if SNRT_NFPU_PER_CORE % 2 == 0
//      vslide1down8_v0(**(a));
//      vslide1down8_v0(**(b));
//
//      *a += 1;
//      *b += 1;
//    #endif
  #endif
}
inline void test_narrow_procedure_64(uint8_t **a, uint8_t **b){
  test_narrow_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down64_v0(*((uint64_t*) (*a) )); // i is equal to the number of strides performed
      vslide1down64_v8(*((uint64_t*) (*b) ));

      *a += 8;
      *b += 8;

      test_narrow_op();
    }
  #endif
}
inline void test_narrow_procedure_32(uint8_t **a, uint8_t **b){
  test_narrow_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down32_v0(*((uint32_t*) (*a) )); // i is equal to the number of strides performed
      vslide1down32_v8(*((uint32_t*) (*b) ));

      *a += 4;
      *b += 4;

      test_narrow_op();
    }
  #endif
}
inline void test_narrow_procedure_16(uint8_t **a, uint8_t **b){
  test_narrow_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down16_v0(*((uint16_t*) (*a) )); // i is equal to the number of strides performed
      vslide1down16_v8(*((uint16_t*) (*b) ));

      *a += 2;
      *b += 2;

      test_narrow_op();
    }
  #endif
}
inline void test_narrow_procedure_8(uint8_t **a, uint8_t **b){
  test_narrow_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down8_v0(**(a)); // i is equal to the number of strides performed
      vslide1down8_v8(**(b));

      *a += 1;
      *b += 1;

      test_narrow_op();
    }

//    #if SNRT_NFPU_PER_CORE % 2 == 0
//      vslide1down8_v0(**(a));
//      vslide1down8_v0(**(b));
//
//      *a += 1;
//      *b += 1;
//    #endif
  #endif
}
inline void test_widen_procedure_64(uint8_t **a, uint8_t **b){
  test_widen_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down64_v0(*((uint64_t*) (*a) )); // i is equal to the number of strides performed
      vslide1down64_v8(*((uint64_t*) (*b) ));

      *a += 8;
      *b += 8;

      test_widen_op();
    }
  #endif
}
inline void test_widen_procedure_32(uint8_t **a, uint8_t **b){
  test_widen_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down32_v0(*((uint32_t*) (*a) )); // i is equal to the number of strides performed
      vslide1down32_v8(*((uint32_t*) (*b) ));

      *a += 4;
      *b += 4;

      test_widen_op();
    }
  #endif
}
inline void test_widen_procedure_16(uint8_t **a, uint8_t **b){
  test_widen_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down16_v0(*((uint16_t*) (*a) )); // i is equal to the number of strides performed
      vslide1down16_v8(*((uint16_t*) (*b) ));

      *a += 2;
      *b += 2;

      test_widen_op();
    }
  #endif
}
inline void test_widen_procedure_8(uint8_t **a, uint8_t **b){
  test_widen_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down8_v0(**(a)); // i is equal to the number of strides performed
      vslide1down8_v8(**(b));

      *a += 1;
      *b += 1;

      test_widen_op();
    }

//    #if SNRT_NFPU_PER_CORE % 2 == 0
//      vslide1down8_v0(**(a));
//      vslide1down8_v0(**(b));
//
//      *a += 1;
//      *b += 1;
//    #endif
  #endif
}

inline void safe_test_procedure_64(uint8_t** a, uint8_t** b, uint8_t* a_start, uint8_t* b_start, uint8_t* a_end, uint8_t* b_end){
  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down64_v0(*( (uint64_t*) (*a))); // i is equal to the number of strides performed
      vslide1down64_v8(*( (uint64_t*) (*b)));

      *a += 8;
      *b += 8;

      if(*a >= a_end) *a = a_start;
      if(*b >= b_end) *b = b_start;

      test_op();
    }
  #endif
}
inline void safe_test_procedure_32(uint8_t** a, uint8_t** b, uint8_t* a_start, uint8_t* b_start, uint8_t* a_end, uint8_t* b_end){
  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down32_v0(*( (uint32_t*) (*a) )); // i is equal to the number of strides performed
      vslide1down32_v8(*( (uint32_t*) (*b) ));

      *a += 4;
      *b += 4;

      if(*a >= a_end) *a = a_start;
      if(*b >= b_end) *b = b_start;

      test_op();
    }
  #endif
}
inline void safe_test_procedure_16(uint8_t** a, uint8_t** b, uint8_t* a_start, uint8_t* b_start, uint8_t* a_end, uint8_t* b_end){
  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down16_v0(*( (uint16_t*) (*a) )); // i is equal to the number of strides performed
      vslide1down16_v8(*( (uint16_t*) (*b) ));

      *a += 2;
      *b += 2;

      if(*a >= a_end) *a = a_start;
      if(*b >= b_end) *b = b_start;

      test_op();
    }
  #endif
}
inline void safe_test_procedure_8(uint8_t** a, uint8_t** b, uint8_t* a_start, uint8_t* b_start, uint8_t* a_end, uint8_t* b_end){
  test_op();
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down8_v0(**a); // i is equal to the number of strides performed
      vslide1down8_v8(**b);

      *a += 1;
      *b += 1;

      if(*a >= a_end) *a = a_start;
      if(*b >= b_end) *b = b_start;

      test_op();
    }
  #endif
}

inline void safe_test_procedure64_f_v8(uint8_t** b, uint8_t* b_start, uint8_t* b_end, double d){
  test_op_f_v8(d);
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down64_v8(*( (uint64_t*) (*b) ));

      *b += 8;

      if(*b >= b_end) *b = b_start;

      test_op_f_v8(d);
    }
  #endif
}
// TODO: fix it to be float and so on
inline void safe_test_procedure32_f_v8(uint8_t** b, uint8_t* b_start, uint8_t* b_end, double d){
  test_op_f_v8(d);
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down32_v8(*( (uint32_t*) (*b) ));

      *b += 4;

      if(*b >= b_end) *b = b_start;

      test_op_f_v8(d);
    }
  #endif
}
inline void safe_test_procedure16_f_v8(uint8_t** b, uint8_t* b_start, uint8_t* b_end, double d){
  test_op_f_v8(d);
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down16_v8(*( (uint16_t*) (*b) ));

      *b += 2;

      if(*b >= b_end) *b = b_start;

      test_op_f_v8(d);
    }
  #endif
}

inline void test_procedure64_f_v8(uint8_t** b, double d){
  test_op_f_v8(d);
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down64_v8(*( (uint64_t*) (*b) ));

      *b += 8;

      test_op_f_v8(d);
    }
  #endif
}
inline void test_procedure32_f_v8(uint8_t** b, double d){
  test_op_f_v8(d);
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down64_v8(*( (uint32_t*) (*b) ));

      *b += 4;

      test_op_f_v8(d);
    }
  #endif
}
inline void test_procedure16_f_v8(uint8_t** b, double d){
  test_op_f_v8(d);
  #if SNRT_NFPU_PER_CORE > 1
    for(int i = 1; i < SNRT_NFPU_PER_CORE; i++){
      vslide1down16_v8(*( (uint16_t*) (*b) ));

      *b += 2;

      test_op_f_v8(d);
    }
  #endif
}