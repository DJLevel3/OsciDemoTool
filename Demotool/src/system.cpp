
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

void eulerMat(float* matrix, float x, float y, float z) {
    float cX = cosf(x);
    float sX = sinf(x);
    float cY = cosf(y);
    float sY = sinf(y);
    float cZ = cosf(z);
    float sZ = sinf(z);
    matrix[0 * 4 + 0] = cY * cZ;
    matrix[0 * 4 + 1] = -cY * sZ;
    matrix[0 * 4 + 2] = sY;
    matrix[0 * 4 + 3] = 0;
    matrix[1 * 4 + 0] = cX * sZ + sX * sY * cZ;
    matrix[1 * 4 + 1] = cX * cZ - sX * sY * sZ;
    matrix[1 * 4 + 2] = -sX * cY;
    matrix[1 * 4 + 3] = 0;
    matrix[2 * 4 + 0] = sX * sZ - cX * sY * cZ;
    matrix[2 * 4 + 1] = sX * cZ + cX * sY * sZ;
    matrix[2 * 4 + 2] = cX * cY;
    matrix[2 * 4 + 3] = 0;
    matrix[3 * 4 + 0] = 0;
    matrix[3 * 4 + 1] = 0;
    matrix[3 * 4 + 2] = 0;
    matrix[3 * 4 + 3] = 1;
}