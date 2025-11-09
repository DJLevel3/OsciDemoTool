//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//
#pragma once

#include "config.h"
#include "system.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
extern "C" {
#include <windows.h>
}

#define MAX_CHAR_VERTS 32
#define MAX_CHAR_STROKES 8

void lineToSamples(float* vec3Start, float* vec3End, short* buffer, int samples);
//bool strokeToCycle3D(float* points, int nPoints, short* buffer, int samples, float* viewMatrix);
bool strokeToCycle2D(float* points, int nPoints, short* buffer, int samples);
void addSamples(short* buffer, short* toAdd, int samples, float stretch);
float normalizeBuffer(short* buffer, int samples, float volume);
void fadeBuffer(short* buffer, int samples, float start, float end);
//static float vec3Distance(float* vec3A, float* vec3B);
void wobbleBufferEnv(short* buffer, int samples, float periodT, int phase, float scaleX, float scaleY, float intensityX, float intensityY, float curve);
//void wobbleBuffer(short* buffer, int samples, float periodT, int phase, float intensityX, float intensityY);
//void filterBuffer(short* buffer, int samples, float resonance, float start, float end);
//void circleFill(short* buffer, int samples, int period, float level);
float squareWaveNoDC(int t, int period, float pulseWidth);
//float squareWaveCentered(int t, int period, float pulseWidth);
//float squareWaveZeroed(int t, int period, float pulseWidth);
float mn2f(float n);
//void rotX(float* src, float* dest, float theta);
void rotY(float* src, float* dest, float theta);
//void rotZ(float* src, float* dest, float theta);
void drawChar(short* buffer, int samples, char character, float x1, float y1, float x2, float y2);

// gen should return in the range {0, 255}
//bool hilligoss(short* buffer, int samples, int randomSeed, float tInitial, float tRate, unsigned char(*gen)(short x, short y, float t), int stretch);
//bool determinePath(short* pixelsOriginal, int targetCount);

#define MAX_HILLIGOSS_ITERATIONS (1000 * 1000 * 10)