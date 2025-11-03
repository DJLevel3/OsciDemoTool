//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//
#pragma once

extern "C" {
#include <stdlib.h>
#include <math.h>
}
#include "fp.h"

#define M_PI 3.14159265358
#define M_E 2.71828182845904523536

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
    return seed[0] >> 3;
}


#ifndef _DEBUG
extern "C" {
    void* memset(void* dst, int val, size_t size)
    {
        char* realdst = (char*)dst;
        for (size_t i = 0; i < size; i++)
            realdst[i] = (char)val;
        return dst;
    }

    void* memcpy(void* dst, const void* src, size_t size)
    {
        char* _dst = (char*)dst;
        const char* _src = (char*)src;
        for (size_t i = 0; i < size; i++)
            _dst[i] = _src[i];
        return dst;
    }
}
#endif

void eulerMat(float* matrix, float x, float y, float z);

void mvp43(float* matrix, float* src, float* dest);

static void rotY(float* src, float* dest, float theta) {
    float sT = sinf(theta);
    float cT = cosf(theta);
    
    float x, y;
    x = cT * src[0] - sT * src[2];
    y = sT * src[0] + cT * src[2];

    dest[0] = x;
    dest[1] = src[1];
    dest[2] = y;
}

static void rotZ(float* src, float* dest, float theta) {
    float sT = sinf(theta);
    float cT = cosf(theta);

    float x, y;

    x = cT * src[0] - sT * src[1];
    y = sT * src[0] + cT * src[1];

    dest[0] = x;
    dest[1] = y;
    dest[2] = src[2];
}

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