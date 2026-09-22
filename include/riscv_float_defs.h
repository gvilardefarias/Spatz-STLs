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
// Based on float_macros.h from the RISC-V Tests repository 

#ifndef RISCV_FLOAT_DEFS_H
#define RISCV_FLOAT_DEFS_H

#define RM_RNE 0x0
#define RM_RTZ 0x1
#define RM_RDN 0x2
#define RM_RUP 0x3
#define RM_RMM 0x4

typedef union {
    double d;
    uint64_t i;
} DoubleInt;

typedef union {
    float d;
    uint32_t i;
} Float32Int;

typedef union {
    __fp16 d;
    uint16_t i;
} Float16Int;

__attribute__ ((always_inline)) inline void fsrm(const unsigned int rm){
	asm volatile("fsrm %0" :: "r"(rm) : "memory");
}

__attribute__ ((always_inline)) inline void csrw_fmode(const unsigned int fmode){
	asm volatile("csrw 0x800, %0" :: "r"(fmode) : "memory");
}

#endif