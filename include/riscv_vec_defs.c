
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

int vtype_to_int(vtype_t *vtype){
    return (vtype->sew << 3) | vtype->lmul | (3 << 6); // ta=1, ma=1
}

void vsetvl(vconfig_t *vconfig){
    unsigned int vl;

    if (vconfig->AVL == -1) {
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


__attribute__ ((always_inline)) inline void vle64_v0(void *addr){
	asm volatile("vle64.v v0, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vle64_v8(void *addr){
	asm volatile("vle64.v v8, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vle64_v16(void *addr){
	asm volatile("vle64.v v16, (%0)" ::"r"(addr));
}
__attribute__ ((always_inline)) inline void vle64_v24(void *addr){
	asm volatile("vle64.v v24, (%0)" ::"r"(addr));
}

__attribute__ ((always_inline)) inline void vfmul_vv_v16_v0_v8(){
	asm volatile("vfmul.vv v16, v0, v8");
}

__attribute__ ((always_inline)) inline void vslide1down_v0(int value){
	asm volatile("vslide1down.vx v0, v0, %0" ::"r"(value));
}
__attribute__ ((always_inline)) inline void vslide1down_v8(int value){
	asm volatile("vslide1down.vx v8, v8, %0" ::"r"(value));
}