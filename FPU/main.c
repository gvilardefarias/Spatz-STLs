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

#if TEST_TARGET == TEST_MIX || TEST_TARGET == TEST_ALL
uint32_t *e;
uint32_t *f;
#endif
#if TEST_TARGET == TEST_GIZO || TEST_TARGET == TEST_ALL
uint64_t *a;
uint64_t *b;
uint64_t *c;
uint64_t *d;
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
    a = (uint64_t *)snrt_l1alloc(53 * 8); // Byte size
    b = (uint64_t *)snrt_l1alloc(53 * 8);
    c = (uint64_t *)snrt_l1alloc(53 * 8);
    d = (uint64_t *)snrt_l1alloc(53 * 8);
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
    snrt_dma_start_1d(a, tp_0, 53 * 8);
    snrt_dma_start_1d(b, tp_1, 53 * 8);
    snrt_dma_start_1d(c, tp_2, 53 * 8);
    snrt_dma_start_1d(d, tp_3, 53 * 8);
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
    test_gizo(cst_0, cst_1, a, b, c, d);
#endif

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
