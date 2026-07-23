
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

#include "riscv_vec_defs.h"
#include <stdint.h>

inline int vtype_to_int(vtype_t *vtype){
    return (vtype->sew << 3) | vtype->lmul | (3 << 6); // ta=1, ma=1
}

inline void vsetvl(vconfig_t *vconfig){
    unsigned int vl;

    if (vconfig->AVL == -1) {   // Set AVL to max
        asm volatile("vsetvl %[vl], x0, %[vtype]"
                     : [vl]    "=r"(vl)
                     : [vtype] "r"(vtype_to_int(&vconfig->vtype)));
    } else {
        asm volatile("vsetvl %[vl], %[AVL], %[vtype]"
                     : [vl]    "=r"(vl)
                     : [AVL]   "r"(vconfig->AVL),
                       [vtype] "r"(vtype_to_int(&vconfig->vtype)));
    }

    vconfig->AVL = vl;
}

inline unsigned int vsetvtype(vtype_t *vtype){
    unsigned int vl;
    asm volatile("vsetvl %[vl], x0, %[vtype]"   // Set AVL to max
                 : [vl]    "=r"(vl)
                 : [vtype] "r"(vtype_to_int(vtype)));
    return vl;
}


__attribute__ ((always_inline)) inline void vle64_v0(void *addr){
	asm volatile("vle64.v v0, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vle64_v8(void *addr){
	asm volatile("vle64.v v8, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vle32_v0(void *addr){
	asm volatile("vle32.v v0, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vle32_v8(void *addr){
	asm volatile("vle32.v v8, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vle16_v0(void *addr){
	asm volatile("vle16.v v0, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vle16_v8(void *addr){
	asm volatile("vle16.v v8, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vle8_v0(void *addr){
    asm volatile("vle8.v v0, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vle8_v8(void *addr){
    asm volatile("vle8.v v8, (%0)" ::"r"(addr));
}

__attribute__ ((always_inline)) inline void vle64_v16(void *addr){
	asm volatile("vle64.v v16, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vle64_v24(void *addr){
	asm volatile("vle64.v v24, (%0)" ::"r"(addr));
}


__attribute__ ((always_inline)) inline void vse64_v0(void *addr){
    asm volatile("vse64.v v0, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vse64_v8(void *addr){
    asm volatile("vse64.v v8, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vse32_v0(void *addr){
    asm volatile("vse32.v v0, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vse32_v8(void *addr){
    asm volatile("vse32.v v8, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vse16_v0(void *addr){
    asm volatile("vse16.v v0, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vse16_v8(void *addr){
    asm volatile("vse16.v v8, (%0)" ::"r"(addr));
}

__attribute__ ((always_inline)) inline void vfmul_vv_v16_v0_v8(){
	asm volatile("vfmul.vv v16, v0, v8");
}
__attribute__ ((always_inline)) inline void vfadd_vv_v16_v0_v8(){
	asm volatile("vfadd.vv v16, v0, v8");
}
__attribute__ ((always_inline)) inline void vfsub_vv_v16_v0_v8(){
	asm volatile("vfsub.vv v16, v0, v8");
}
__attribute__ ((always_inline)) inline void vfmacc_vv_v16_v0_v8(){
	asm volatile("vfmacc.vv v16, v0, v8");
}
__attribute__ ((always_inline)) inline void vfmsac_vv_v16_v0_v8(){
	asm volatile("vfmsac.vv v16, v0, v8");
}
__attribute__ ((always_inline)) inline void vor_vx_v8(uint64_t r){
	asm volatile("vor.vx v8, v8, %0" :: "r"(r));
}
__attribute__ ((always_inline)) inline void vor_vx_v0(uint64_t r){
	asm volatile("vor.vx v0, v0, %0" :: "r"(r));
}
__attribute__ ((always_inline)) inline void vand_vx_v8(uint64_t r){
	asm volatile("vand.vx v8, v8, %0" :: "r"(r));
}

__attribute__ ((always_inline)) inline void vfsub_vf_v16_v8(double d){
	asm volatile("vfsub.vf v16, v8, %0" ::"f"(d));
}
__attribute__ ((always_inline)) inline void vfadd_vf_v16_v8(double d){
	asm volatile("vfadd.vf v16, v8, %0" ::"f"(d));
}
__attribute__ ((always_inline)) inline void vfmul_vf_v16_v8(double d){
	asm volatile("vfmul.vf v16, v8, %0" ::"f"(d));
}

__attribute__ ((always_inline)) inline void vslide1down_v0(uint64_t value){
	asm volatile("vslide1down.vx v0, v0, %0" ::"r"(value));
}
__attribute__ ((always_inline)) inline void vslide1down_v8(uint64_t value){
	asm volatile("vslide1down.vx v8, v8, %0" ::"r"(value));
}