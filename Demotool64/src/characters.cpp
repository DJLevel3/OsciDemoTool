#include "characters.h"

#define IH_COORDS 2
const static float coordsIH[] = {
    0.f,  1.f,
    0.f, -1.f
};
int genCharI(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < IH_COORDS; i++) {
        vertArray[i * 3] = coordsIH[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsIH[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = IH_COORDS;
    return 1;
}/*
int genCharH(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < IH_COORDS; i++) {
        vertArray[i * 3] = coordsIH[i * 2 + 1] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsIH[i * 2] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = IH_COORDS;
    return 1;
}*/

#define ZN_COORDS 8
const static float coordsZN[] = {
    -1.f,  1.f,
     1.f,  1.f,
    -1.f, -1.f,
     1.f, -1.f,
    -1.f, -1.f,
     1.f,  1.f,
    -1.f,  1.f,
     1.f,  1.f,
};/*
int genCharZ(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < ZN_COORDS; i++) {
        vertArray[i * 3] = coordsZN[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsZN[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = ZN_COORDS;
    return 1;
}*/
int genCharN(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < ZN_COORDS; i++) {
        vertArray[i * 3] = -coordsZN[i * 2 + 1] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsZN[i * 2] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = ZN_COORDS;
    return 1;
}


#define E_COORDS 11
const static float coordsE[] = {
    -1.f,  1.f,
     1.f,  1.f,
    -1.f,  1.f,
    -1.f,  0.f,
     1.f,  0.f,
    -1.f,  0.f,
    -1.f, -1.f,
     1.f, -1.f,
    -1.f, -1.f,
    -1.f,  1.f,
     1.f,  1.f,
};
int genCharE(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < E_COORDS; i++) {
        vertArray[i * 3] = coordsE[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsE[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = E_COORDS;
    return 1;
}
/*
#define MW_COORDS 11
const static float coordsMW[] = {
    -1.0f, -1.0f,
    -1.0f,  0.6f,
    -0.6f,  1.0f,
    -0.4f,  1.0f,
     0.0f,  0.6f,
     0.0f,  0.0f,
     0.0f,  0.6f,
     0.4f,  1.0f,
     0.6f,  1.0f,
     1.0f,  0.6f,
     1.0f, -1.0f,
};
int genCharM(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < MW_COORDS; i++) {
        vertArray[i * 3] = coordsMW[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsMW[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = MW_COORDS;
    return 1;
}
int genCharW(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < MW_COORDS; i++) {
        vertArray[i * 3] = coordsMW[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = -coordsMW[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = MW_COORDS;
    return 1;
}*/

#define R_COORDS 6
const static float coordsR[] = {
    -1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f,
     1.0f,  0.0f,
    -1.0f,  0.0f,
     1.0f, -1.0f,
};
int genCharR(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < R_COORDS; i++) {
        vertArray[i * 3] = coordsR[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsR[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = R_COORDS;
    return 1;
}

#define CU_COORDS 6
const static float coordsCU[] = {
     1.0f,  1.0f,
    -0.6f,  1.0f,
    -1.0f,  0.6f,
    -1.0f, -0.6f,
    -0.6f, -1.0f,
     1.0f, -1.0f
};
int genCharC(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < CU_COORDS; i++) {
        vertArray[i * 3] = coordsCU[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsCU[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = CU_COORDS;
    return 1;
}
/*
int genCharU(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < CU_COORDS; i++) {
        vertArray[i * 3] = coordsCU[i * 2 + 1] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsCU[i * 2] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = CU_COORDS;
    return 1;
}
#define B_COORDS 12
const static float coordsB[] = {
    -1.0f, -1.0f,
    -1.0f,  1.0f,
     0.6f,  1.0f,
     1.0f,  0.6f,
     1.0f,  0.2f,
     0.8f,  0.0f,
    -1.0f,  0.0f,
     0.8f,  0.0f,
     1.0f, -0.2f,
     1.0f, -0.6f,
     0.6f, -1.0f,
    -1.0f, -1.0f,
};
int genCharB(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < B_COORDS; i++) {
        vertArray[i * 3] = coordsB[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsB[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = B_COORDS;
    return 1;
}*/

#define A_COORDS 6
const static float coordsA[] = {
    -1.0f, -1.0f,
     0.0f,  1.0f,
     1.0f, -1.0f,
     0.5f,  0.0f,
    -0.5f,  0.0f,
    -1.0f, -1.0f,
};
int genCharA(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < A_COORDS; i++) {
        vertArray[i * 3] = coordsA[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsA[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = A_COORDS;
    return 1;
}/*
int genCharV(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < 3; i++) {
        vertArray[i * 3] = coordsA[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsA[i * 2 + 1] * -scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = 3;
    return 1;
}*/

#define S_COORDS 6
const static float coordsS[] = {
     1.0f,  1.0f,
    -1.0f,  1.0f,
    -1.0f,  0.0f,
     1.0f,  0.0f,
     1.0f, -1.0f,
    -1.0f, -1.0f
};
int genCharS(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < S_COORDS; i++) {
        vertArray[i * 3] = coordsS[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsS[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = S_COORDS;
    return 1;
}
int genChar2(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < S_COORDS; i++) {
        vertArray[i * 3] = coordsS[i * 2] * -scaleX + offX;
        vertArray[i * 3 + 1] = coordsS[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = S_COORDS;
    return 1;
}

#define O_COORDS 9
const static float coordsO[] = {
    -1.0f,  0.6f,
    -0.6f,  1.0f,
     0.6f,  1.0f,
     1.0f,  0.6f,
     1.0f, -0.6f,
     0.6f, -1.0f,
    -0.6f, -1.0f,
    -1.0f, -0.6f,
    -1.0f,  0.6f
};
int genCharO(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < O_COORDS; i++) {
        vertArray[i * 3] = coordsO[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsO[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = O_COORDS;
    return 1;
}
/*
#define L_COORDS 3
const static float coordsL[] = {
    -1.0f,  1.0f,
    -1.0f, -1.0f,
     1.0f, -1.0f
};
int genCharL(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < L_COORDS; i++) {
        vertArray[i * 3] = coordsL[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsL[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = L_COORDS;
    return 1;
}

#define T_COORDS 4
const static float coordsT[] = {
    -1.0f,  1.0f,
     1.0f,  1.0f,
     0.0f,  1.0f,
     0.0f, -1.0f
};
int genCharT(float* vertArray, short* indArray, float scaleX, float scaleY, float offX, float offY)
{
    for (int i = 0; i < T_COORDS; i++) {
        vertArray[i * 3] = coordsT[i * 2] * scaleX + offX;
        vertArray[i * 3 + 1] = coordsT[i * 2 + 1] * scaleY + offY;
        vertArray[i * 3 + 2] = 0;
    }
    indArray[0] = 0;
    indArray[1] = T_COORDS;
    return 1;
}*/

void drawChar(short* buffer, int samples, char character, float x1, float y1, float x2, float y2)
{
    float offX = (x1 + x2) / 2;
    float scaleX = (x2 - x1) / 2;

    float offY = (y1 + y2) / 2;
    float scaleY = (y2 - y1) / 2;

    float* strokeVerts = (float*)malloc(3 * MAX_CHAR_VERTS * sizeof(float));
    if (strokeVerts == 0) {
        return;
    }
    short* strokeIndices = (short*)malloc(2 * MAX_CHAR_STROKES * sizeof(short));
    if (strokeIndices == 0) {
        free(strokeVerts);
        return;
    }
    int nStrokes = 0;
    if (character >= 0x61 && character <= 0x7A) character -= 0x20;

    if (character == 'A') {
        nStrokes = genCharA(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }
    /*
    if (character == 'B') {
        nStrokes = genCharB(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }*/
    if (character == 'C') {
        nStrokes = genCharC(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }
    if (character == 'E') {
        nStrokes = genCharE(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    } /*
    case 'H':
        nStrokes = genCharH(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
        break;*/
    if (character == 'I') {
        nStrokes = genCharI(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }/*
    if (character == 'L') {
        nStrokes = genCharL(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }
    case 'M':
        nStrokes = genCharM(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
        break;*/
    if (character == 'N') {
        nStrokes = genCharN(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }
    if (character == '0') {
        nStrokes = genCharO(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }
    if (character == 'R') {
        nStrokes = genCharR(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }
    if (character == '5') {
        nStrokes = genCharS(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }
    if (character == '2') {
        nStrokes = genChar2(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }/*
    if (character == 'T') {
        nStrokes = genCharT(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }
    if (character == 'U') {
        nStrokes = genCharU(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }
    if (character == 'V') {
        nStrokes = genCharV(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
    }/*
    case 'W':
        nStrokes = genCharW(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
        break;
    case 'Z':
        nStrokes = genCharZ(strokeVerts, strokeIndices, scaleX, scaleY, offX, offY);
        break;*/

    if (nStrokes > 0) {
        float sampsPerStroke = samples / nStrokes;
        int sampsBase = 0;
        float sampsTarget = 0;
        for (int i = 0; i < nStrokes; i++) {
            sampsTarget += sampsPerStroke;
            strokeToCycle2D(strokeVerts + 3 * strokeIndices[i * 2], strokeIndices[i * 2 + 1], buffer + sampsBase * 2, f2i(sampsTarget - sampsBase));
            sampsBase += f2i(sampsTarget - sampsBase);
        }
    }

    free(strokeVerts);
    free(strokeIndices);
    return;
}