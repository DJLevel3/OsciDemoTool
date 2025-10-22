//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//
#pragma once

#include "fp.h"
#include <cmath>

static inline int f2i(float x) // use this to convert float to int, or use /QIfist as additional compiler parameters
{
    int tmp;
    _asm fld dword ptr[x]
    _asm fistp dword ptr[tmp];
    return tmp;
}
/*
static inline int mysinf(float x)
{
    int tmp;
    _asm fld dword ptr[x]
    _asm fsin
    _asm fstp dword ptr[tmp];
    return tmp;
}*/

static inline int demo_rand(int* seed)
{
    seed[0] = seed[0] * 0x343FD + 0x269EC3;
    return((seed[0] >> 3) & 65535);
}

void eulerMat(float* matrix, float x, float y, float z);

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