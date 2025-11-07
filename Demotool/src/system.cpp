
#include "system.h"

void mvp43(float* matrix, float* src, float* dest)
{
    float result[4] = { 0, 0, 0, 0 };
    for (int i = 0; i < 4; i++) {
        result[i] = src[0] * matrix[i * 4 + 0] + src[1] * matrix[i * 4 + 1] + src[2] * matrix[i * 4 + 2] + matrix[i * 4 + 3];
    }
    dest[0] = result[0] / result[3];
    dest[1] = result[1] / result[3];
    dest[2] = result[2] / result[3];
}

/*
void eulerMat(float* matrix, float x, float y, float z) {
    float cX = cosf(x);
    float sX = sinf(x);
    float cY = cosf(y);
    float sY = sinf(y);
    float cZ = cosf(z);
    float sZ = sinf(z);
    matrix[0] = cY * cZ;
    matrix[1] = -cY * sZ;
    matrix[2] = sY;
    matrix[3] = 0;
    matrix[4] = cX * sZ + sX * sY * cZ;
    matrix[5] = cX * cZ - sX * sY * sZ;
    matrix[6] = -sX * cY;
    matrix[7] = 0;
    matrix[8] = sX * sZ - cX * sY * cZ;
    matrix[9] = sX * cZ + cX * sY * sZ;
    matrix[10] = cX * cY;
    matrix[11] = 0;
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}*/

extern "C"
{
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