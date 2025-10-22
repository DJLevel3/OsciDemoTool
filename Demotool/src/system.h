//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "fp.h"

#ifdef WINDOWS
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

void eulerMat(float* matrix, float x, float y, float z) {
    float cX = cosf(x);
    float sX = sinf(x);
    float cY = cosf(y);
    float sY = sinf(y);
    float cZ = cosf(z);
    float sZ = sinf(z);
    matrix[0*4+0] = cY * cZ;
    matrix[0*4+1] = -cY * sZ;
    matrix[0*4+2] = sY;
    matrix[0*4+3] = 0;
    matrix[1*4+0] = cX * sZ + sX * sY * cZ;
    matrix[1*4+1] = cX * cZ - sX * sY * sZ;
    matrix[1*4+2] = -sX * cY;
    matrix[1*4+3] = 0;
    matrix[2*4+0] = sX * sZ - cX * sY * cZ;
    matrix[2*4+1] = sX * cZ + cX * sY * sZ;
    matrix[2*4+2] = cX * cY;
    matrix[2*4+3] = 0;
    matrix[3*4+0] = 0;
    matrix[3*4+1] = 0;
    matrix[3*4+2] = 0;
    matrix[3*4+3] = 1;
}

void mvp43(float* matrix, float* src, float* dest)
{
    float result[4] = { 0, 0, 0, 0 };
    for (int i = 0; i < 4; i++) {
        result[i] = src[0] * matrix[i*4+0] + src[1] * matrix[i*4+1] + src[2] * matrix[i*4+2] + matrix[i*4+3];
    }
    dest[0] = result[0] / result[3];
    dest[1] = result[1] / result[3];
    dest[2] = result[2] / result[3];
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

#endif

#endif