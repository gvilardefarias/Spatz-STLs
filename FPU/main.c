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

#include <debug.h>
#include <snrt.h>
#include <stdio.h>

#include DATAHEADER
#include "kernel/STLs.c"

#if TEST_TARGET == TEST_GIZO || TEST_TARGET == TEST_ALL
#include "kernel/test_gizo.c"

#if TEST_SEW_64 == 1
  uint64_t *a_64;
  uint64_t *b_64;
  uint64_t *c_64;
  uint64_t *d_64;
#endif
#if TEST_SEW_32 == 1
  uint32_t *a_32;
  uint32_t *b_32;
  uint32_t *c_32;
  uint32_t *d_32;
#endif
#if TEST_SEW_16 == 1
  uint16_t *a_16;
  uint16_t *b_16;
  uint16_t *c_16;
  uint16_t *d_16;
#endif
// TODO: fix gizo with SEW8
//#if TEST_SEW_8 == 1
//  uint8_t *a_8;
//  uint8_t *b_8;
//  uint8_t *c_8;
//  uint8_t *d_8;
//  uint8_t *cst_0_8;
//  uint8_t *cst_1_8;
//#endif
#endif
#if TEST_TARGET == TEST_MIX || TEST_TARGET == TEST_ALL
#include "kernel/test_mix.c"

uint32_t *e;
uint32_t *f;
#endif

size_t benchmark_get_cycle() { return read_csr(mcycle); }

int main() {
  const unsigned int num_cores = snrt_cluster_core_num();
  const unsigned int cid = snrt_cluster_core_idx();

  const unsigned int measure_iterations = 1;

  unsigned int timer_start, timer_end, timer;

  unsigned int kernel_size;

  // Allocate the test patterns
  if (cid == 0) {
#if TEST_TARGET == TEST_MIX || TEST_TARGET == TEST_ALL
    e = (uint32_t *)snrt_l1alloc(TP_MUL * SNRT_VLEN * LMUL / 8);
    f = (uint32_t *)snrt_l1alloc(TP_MUL * SNRT_VLEN * LMUL / 8);
#endif
#if TEST_TARGET == TEST_GIZO || TEST_TARGET == TEST_ALL
#if TEST_SEW_64 == 1
    a_64 = (uint64_t *)snrt_l1alloc(53 * 8); // Byte size
    b_64 = (uint64_t *)snrt_l1alloc(53 * 8);
    c_64 = (uint64_t *)snrt_l1alloc(53 * 8);
    d_64 = (uint64_t *)snrt_l1alloc(53 * 8);
#endif
#if TEST_SEW_32 == 1
    a_32 = (uint32_t *)snrt_l1alloc(24 * 4); // Byte size
    b_32 = (uint32_t *)snrt_l1alloc(24 * 4);
    c_32 = (uint32_t *)snrt_l1alloc(24 * 4);
    d_32 = (uint32_t *)snrt_l1alloc(24 * 4);
#endif
#if TEST_SEW_16 == 1
    a_16 = (uint16_t *)snrt_l1alloc(11 * 2); // Byte size
    b_16 = (uint16_t *)snrt_l1alloc(11 * 2);
    c_16 = (uint16_t *)snrt_l1alloc(11 * 2);
    d_16 = (uint16_t *)snrt_l1alloc(11 * 2);
#endif
//#if TEST_SEW_8 == 1
//    a_8 = (uint8_t *)snrt_l1alloc(5); // Byte size
//    b_8 = (uint8_t *)snrt_l1alloc(5);
//    c_8 = (uint8_t *)snrt_l1alloc(5);
//    d_8 = (uint8_t *)snrt_l1alloc(5);
//    cst_0_8 = (uint8_t *)snrt_l1alloc(5);
//    cst_1_8 = (uint8_t *)snrt_l1alloc(5);
//#endif
#endif
  }

  // Reset timer
  timer = (unsigned int)-1;

  // Wait for all cores to finish
  snrt_cluster_hw_barrier();

  // Initialize matrices
  // TODO: load only partially the patterns and load more as the test advances
  if (cid == 0) {
#if TEST_TARGET == TEST_MIX || TEST_TARGET == TEST_ALL
    snrt_dma_start_1d(e, &tp[0], TP_MUL * SNRT_VLEN * LMUL / 8);
    snrt_dma_start_1d(f, &tp[1], TP_MUL * SNRT_VLEN * LMUL / 8);
#endif
#if TEST_TARGET == TEST_GIZO || TEST_TARGET == TEST_ALL
#if TEST_SEW_64 == 1
    snrt_dma_start_1d(a_64, tp_0_s64, 53 * 8);
    snrt_dma_start_1d(b_64, tp_1_s64, 53 * 8);
    snrt_dma_start_1d(c_64, tp_2_s64, 53 * 8);
    snrt_dma_start_1d(d_64, tp_3_s64, 53 * 8);
#endif
#if TEST_SEW_32 == 1
    snrt_dma_start_1d(a_32, tp_0_s32, 24 * 4);
    snrt_dma_start_1d(b_32, tp_1_s32, 24 * 4);
    snrt_dma_start_1d(c_32, tp_2_s32, 24 * 4);
    snrt_dma_start_1d(d_32, tp_3_s32, 24 * 4);
#endif
#if TEST_SEW_16 == 1
    snrt_dma_start_1d(a_16, tp_0_s16, 11 * 2);
    snrt_dma_start_1d(b_16, tp_1_s16, 11 * 2);
    snrt_dma_start_1d(c_16, tp_2_s16, 11 * 2);
    snrt_dma_start_1d(d_16, tp_3_s16, 11 * 2);
#endif
//#if TEST_SEW_8 == 1
//    snrt_dma_start_1d(a_8, tp_0_s8, 5);
//    snrt_dma_start_1d(b_8, tp_1_s8, 5);
//    snrt_dma_start_1d(c_8, tp_2_s8, 5);
//    snrt_dma_start_1d(d_8, tp_3_s8, 5);
//    snrt_dma_start_1d(cst_0_8, cst_0_s8, 5);
//    snrt_dma_start_1d(cst_1_8, cst_1_s8, 5);
//#endif
#endif
    snrt_dma_wait_all();
  }

  // Wait for all cores to finish
  snrt_cluster_hw_barrier();

  // Calculate matmul
  for (unsigned int i = 0; i < measure_iterations; ++i) {
    // Start timer
    timer_start = benchmark_get_cycle();

#if TEST_TARGET == TEST_MIX || TEST_TARGET == TEST_ALL
    test_mix(e, f);
#endif
#if TEST_TARGET == TEST_GIZO || TEST_TARGET == TEST_ALL
#if TEST_SEW_64 == 1
    test_gizo_64(cst_0_s64, cst_1_s64, a_64, b_64, c_64, d_64);
#endif
#if TEST_SEW_32 == 1
    test_gizo_32(cst_0_s32, cst_1_s32, a_32, b_32, c_32, d_32);
#endif
#if TEST_SEW_16 == 1
    test_gizo_16(cst_0_s16, cst_1_s16, a_16, b_16, c_16, d_16);
#endif
//#if TEST_SEW_8 == 1
//    test_gizo_8(cst_0_8, cst_1_8, a_8, b_8, c_8, d_8);
//#endif
#endif

// TODO: add test with vstart and thing on the tag to have bits with 1111...

    // Wait for all cores to finish
    snrt_cluster_hw_barrier();

    // End timer and check if new best runtime
    timer_end = benchmark_get_cycle();
    unsigned int timer_temp = timer_end - timer_start;
    if (cid == 0) {
      if (timer_temp < timer) {
        timer = timer_temp;
      }
    }
  }

  // Check and display results
  if (cid == 0) {
    PRINTF("The execution took %u cycles.\n", timer);
  }


  // Wait for all cores to finish
  snrt_cluster_hw_barrier();

  return 0;
}
