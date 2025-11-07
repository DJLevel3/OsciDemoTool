//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//
#ifndef SYSTEM_H
#define SYSTEM_H

extern "C" {
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
}
#include "fp.h"

static inline int f2i(float x) // use this to convert float to int, or use /QIfist as additional compiler parameters
{
    int tmp;
    _asm fld dword ptr[x]
    _asm fistp dword ptr[tmp];
    return tmp;
}

static inline short demo_rand(int* seed)
{
    seed[0] = seed[0] * 0x343FD + 0x269EC3;
    return seed[0] >> 6;
}

extern "C"
{
    void* memset(void* dst, int val, size_t size);

    void* memcpy(void* dst, const void* src, size_t size);
}
//void eulerMat(float* matrix, float x, float y, float z);

void mvp43(float* matrix, float* src, float* dest);

static float view_matrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, -5,
    0, 0, 0, 1
};
static float proj_matrix[16] = {
    1.0f, 0,     0,      0,
    0,    1.0f,  0,      0,
    0,    0,    -1.0f, -1.0f,
    0,    0,    -p0d40,  0.0f
};
#endif