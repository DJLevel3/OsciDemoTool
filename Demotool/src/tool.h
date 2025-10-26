//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#ifndef _TOOL_H_
#define _TOOL_H_

#include "config.h"

#ifndef SAVE_FILE
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#endif
#include "system.h"

#ifdef LISSAJOUS_INTRO
#define INTROMUSIC_DURATION 8
#define INTROMUSIC_NUMSAMPLES  ((int)(INTROMUSIC_DURATION*SAMPLE_RATE))
#define INTROMUSIC_NUMSAMPLESC ((int)(INTROMUSIC_NUMSAMPLES*2))
void mzk_init( short *buffer );
#endif
void vec3ToSample(float* vec3, short* dest);
void lineToSamples(float* vec3Start, float* vec3End, short* buffer, int samples);
bool strokeToCycle(float* points, int nPoints, short* buffer, int samples);
void addSamples(short* buffer, short* toAdd, int samples, float stretch);
float normalizeBuffer(short* buffer, int samples, float volume);
void fadeBuffer(short* buffer, int samples, float start, float end);
static float squareWave(int t, int period, float pulseWidth) {
    return ((t % period) <= pulseWidth * period) ? (pulseWidth - 1) : (pulseWidth);
}
static float mn2f(const char n) {
    return 261.63 * powf(2.0f, (n - 60) / 12.f);
}
static float vec3Distance(float* vec3A, float* vec3B);

#define SCRATCH_SIZE 1024
static float renderer_scratch[SCRATCH_SIZE];
static int scSP = 0;

#endif