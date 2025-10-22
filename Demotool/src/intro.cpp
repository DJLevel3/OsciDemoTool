//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#include "intro.h"

//---------------------------------------------------------------------
#define NOFF -128

#define CUBE_CROSS_LINES

#ifdef CUBE_CROSS_LINES
#define CUBE_NUM_STROKES 6
#else
#define CUBE_NUM_STROKES 4
#endif
const static char cubeIndices[] = {
    3, 2, 5, 4, 3,
    7, 6, 1, 0, 7,
#ifdef CUBE_CROSS_LINES
    0, 6, 4, 2, 0,
#endif

    3, 4, 7, 0, 3,
    1, 6, 5, 2, 1,

#ifdef CUBE_CROSS_LINES
    0, 6, 4, 2, 0
#endif
    //0, 1, 2, 3, 0,
    //4, 5, 6, 7, 4,
};

static float verts[] = { 
 -1.0f, -1.0f,  1.0f, // 0
 -1.0f,  1.0f,  1.0f, // 1
 -1.0f,  1.0f, -1.0f, // 2
 -1.0f, -1.0f, -1.0f, // 3

  1.0f, -1.0f, -1.0f, // 4
  1.0f,  1.0f, -1.0f, // 5
  1.0f,  1.0f,  1.0f, // 6
  1.0f, -1.0f,  1.0f, // 7

//  0.0f,  0.0f,  0.0f, // 8
};

static const float pitchChangeTimes[] = {
    0.25,
    0.5,
    0.25,
    0.375,
    0.625,
    0.5,
    0.25,
    0.25,
    0.25,
    0.25,
    0.25,
    0.25,
};

static const char notes[] = {
   33,
   45,
   33,
   31,
   43,
   29,
   41,
   29,
   31,
   43,
   40,
   36,
   33,
   45,
   33,
   31,
   43,
   29,
   41,
   29,
   31,
   48,
   47,
   43
};

static const char pulseNotes[] = {
    NOFF,
    0,
    3,
    NOFF,
    NOFF,
    -2,
    2,
    -2,
    NOFF,
    -4,
    0,
    -4,
    -2,
    3,
    2,
    -2,
};

#define OPENHAT_SAMPLES (16000)
#define OPENHAT_RATE (96000)

#define PULSE_WIDTH 0.1f
#define PULSE_FREQ_BASE 27.5f
#define PULSE_SAMPLES 192000

#define HAT_PERIOD0 392
#define HAT_PERIOD1 314
#define HAT_PERIOD2 263
#define HAT_PERIOD3 231
#define HAT_PERIOD4 220
#define HAT_PERIOD5 155

short* kickBuf;
short* snareBuf;
short* openHatBuf;
short* pulseBuf;

float rotMatrix[16];

int intro_init( void )
{
    kickBuf = new short[KICK_SAMPLES * 2];
    snareBuf = new short[SNARE_SAMPLES * 2];
    openHatBuf = new short[OPENHAT_SAMPLES * 2];
    pulseBuf = new short[PULSE_SAMPLES * 2];

    int seed = 1;

#ifdef SAMPLED_KICK
    for (int i = 0; i < KICK_SAMPLES; i++) {
        kickBuf[i * 2] = kick[i] * KICK_MULTIPLIER;
        kickBuf[i * 2 + 1] = kickBuf[i * 2];
    }
    normalizeBuffer(kickBuf, KICK_SAMPELS, p0d30);
#else
    float kFreq = KICK_FMAX;
    float kSPC;
    int kSamp;
    for (int i = 0; i < KICK_SAMPLES;) {
        kSPC = MZK_RATE / kFreq;
        for (kSamp = 0; kSamp < kSPC && i < KICK_SAMPLES; kSamp++) {
            kickBuf[i * 2] = f2i(32768.f * squareWave(kSamp, kSPC, p0d40));
            kickBuf[i * 2 + 1] = f2i(32768.f * squareWave(kSamp + f2i(kSPC / 4), kSPC, p0d40));
            i++;
        }
        if (kFreq > KICK_FBASE) kFreq -= KICK_FSTEP;
        else kFreq = KICK_FBASE;
    }
    normalizeBuffer(kickBuf, KICK_FADE_START, KICK_START_V);
    fadeBuffer(kickBuf + (KICK_FADE_START * 2), KICK_FADE_MID - KICK_FADE_START, KICK_START_V, KICK_MID_V);
    fadeBuffer(kickBuf + (KICK_FADE_MID * 2), KICK_FADE_END - KICK_FADE_MID, KICK_MID_V, KICK_END_V);
    normalizeBuffer(kickBuf + (KICK_FADE_END * 2), KICK_SAMPLES - KICK_FADE_END, KICK_END_V);
#endif

#ifdef SAMPLED_SNARE
    for (int i = 0; i < SNARE_SAMPLES; i++) {
        snareBuf[i * 2] = snare[i] * SNARE_MULTIPLIER;
        snareBuf[i * 2 + 1] = snareBuf[i * 2];
    }
    normalizeBuffer(snareBuf, SNARE_SAMPLES, p0d30);
#else
    float sFreq = SNARE_FMAX;
    float sSPC;
    int sSamp;
    float n = 0;
    for (int i = 0; i < SNARE_SAMPLES; ) {
        sSPC = MZK_RATE / sFreq;
        sSamp = 0;
        for (sSamp = 0; sSamp < sSPC && i < SNARE_SAMPLES; sSamp++) {
            if (sSamp % 6 == 0) n = 0.8 * (short(demo_rand(&seed)) * powf(min(max((float)i - SNARE_WIREWAIT, 0), (float)SNARE_WIRETIME) / (SNARE_WIRETIME), 2.0f));
            snareBuf[i * 2] = f2i(32768.f * squareWave(sSamp, sSPC, p0d60));
            snareBuf[i * 2 + 1] = (i < SNARE_WIREWAIT ? 1.8 : 0.8) * snareBuf[i * 2] + f2i((snareBuf[i * 2] * 0.5 + 16384.f) * n / 16384);
            snareBuf[i * 2] = snareBuf[i * 2 + 1];
            i++;
        }
        if (sFreq > SNARE_FBASE) sFreq -= SNARE_FSTEP;
        else sFreq = SNARE_FBASE;
    }
    normalizeBuffer(snareBuf, SNARE_SAMPLES, p0d40);
    fadeBuffer(snareBuf + (SNARE_SAMPLES * 4 / 3), SNARE_SAMPLES / 3, 1.f, 0.f);
#endif

    float state1 = 0;
    float state2 = 0;
    float state3 = 0;
    float retval = 0;
    for (int i = 0; i < OPENHAT_SAMPLES; i++) {
#ifdef NOISE_HIHAT
        retval = short(demo_rand(&seed));
#else
        retval = 10922.f * (
            squareWave(i, HAT_PERIOD0, 0.5f) +
            squareWave(i, HAT_PERIOD1, 0.5f) +
            squareWave(i, HAT_PERIOD2, 0.5f) +
            squareWave(i, HAT_PERIOD3, 0.5f) +
            squareWave(i, HAT_PERIOD4, 0.5f) +
            squareWave(i, HAT_PERIOD5, 0.5f)
            );
#endif
        state1 += p0d45 * (retval - state1 + 3.2 * (state2 - state3));
        state2 += p0d45 * (state1 - state2);
        state3 += p0d45 * (state2 - state3);
        openHatBuf[i * 2] = f2i((retval - state3));
        openHatBuf[i * 2 + 1] = f2i((retval - state3));
    }
    normalizeBuffer(openHatBuf, OPENHAT_SAMPLES, 1.f);
    fadeBuffer(openHatBuf, OPENHAT_SAMPLES, p0d50, p0d00);

    int pw;
    int oneCycle = (int(MZK_RATE / PULSE_FREQ_BASE + 0.5));
    for (pw = 0; pw < (MZK_RATE / PULSE_FREQ_BASE) * PULSE_WIDTH * 2; pw++) {
        pulseBuf[pw] = f2i((PULSE_WIDTH - 1) * 32767);
    }for (; pw < oneCycle * 2; pw++) {
        pulseBuf[pw] = f2i(PULSE_WIDTH * 32767);
    }
    for (; pw < PULSE_SAMPLES * 2; pw++) {
        pulseBuf[pw] = 0;
    }
    for (int i = 1; i * oneCycle < PULSE_SAMPLES; i++) {
        addSamples(pulseBuf + (oneCycle * i), pulseBuf, oneCycle, 1);
    }
    fadeBuffer(pulseBuf, PULSE_SAMPLES, p0d25, p0d00);
    fadeBuffer(pulseBuf, PULSE_SAMPLES / 2, p1d99, p1d00);

    eulerMat(rotMatrix, p0d03, p0d02, p0d05);
    return 1;
}

void intro_do( long itime , short * buffer )
{
    int s;
    float nextTime = 0;
    int p = 0;

    float nSPC = f2i(MZK_RATE / mn2f<const char>(notes[0]));
    int nSPS = f2i(nSPC / CUBE_NUM_STROKES);
    int correction = f2i(nSPC - (CUBE_NUM_STROKES * nSPS));
    nextTime += pitchChangeTimes[p];

    float* curFace = new float[15];
    for (s = 0; s + nSPC < DEMO_NUMSAMPLES;) {
        // generate
        for (int i = 0; i < CUBE_NUM_STROKES; i += 1) {
            for (int v = 0; v < 5; v++) {
                curFace[v * 3 + 0] = verts[cubeIndices[i * 5 + v] * 3 + 0];
                curFace[v * 3 + 1] = verts[cubeIndices[i * 5 + v] * 3 + 1];
                curFace[v * 3 + 2] = verts[cubeIndices[i * 5 + v] * 3 + 2];
            }
            strokeToCycle(curFace, 5, buffer + (2 * s), nSPS);
            s += nSPS;
        }
        for (int i = 0; i < correction; i++) {
            buffer[2 * s - 2] = buffer[2 * s];
            buffer[2 * s - 1] = buffer[2 * s + 1];
            s++;
        }
        for (int i = 0; i < 24; i+=3) {
            mvp43(rotMatrix, verts + i, verts + i);
        }
        while (s > MZK_RATE * nextTime) {
            p++;
            // check pitch and shit
            nSPC = MZK_RATE / mn2f<const char>(notes[p % 24]);
            nSPS = f2i(nSPC / CUBE_NUM_STROKES);
            correction = f2i(nSPC - (CUBE_NUM_STROKES * nSPS));
            nextTime += pitchChangeTimes[p % 12];
        }
    }
    for (; s < DEMO_NUMSAMPLES; s++) {
        buffer[s * 2] = 0;
        buffer[s * 2 + 1] = 0;
    }
    normalizeBuffer(buffer, DEMO_NUMSAMPLES, p0d45);
    delete[] curFace;

    
    // sidechain/kick
    for (int i = 0; i < 256; i += 4) {
        fadeBuffer(buffer + ((MZK_RATE >> 3) * i) * 2, 30000, p0d10, p1d00);
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, MZK_RATE / KICK_RATE);
    }
    for (int i = 368; i < 480; i += 4) {
        fadeBuffer(buffer + ((MZK_RATE >> 3) * i) * 2, 30000, p0d10, p1d00);
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, MZK_RATE / KICK_RATE);
    }/**/

    // snare drums
    for (int i = 68; i < 248; i += 8) {
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, MZK_RATE / SNARE_RATE);
    }
    for (int i = 324; i < 368; i += 8) {
        fadeBuffer(buffer + ((MZK_RATE >> 3) * i) * 2, 30000, p0d30, p1d00);
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, MZK_RATE / SNARE_RATE);
    }
    for (int i = 372; i < 480; i += 8) {
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, MZK_RATE / SNARE_RATE);
    }

    /*
    // closed hihats
    for (int i = 0; i < DEMO_DURATION * 8; i += 1) {
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, openHatBuf + 3 * OPENHAT_SAMPLES / 4, OPENHAT_SAMPLES / 8, MZK_RATE / OPENHAT_RATE);
    }/**/

    // open hihats
    for (int i = 130; i < 248; i += 4) {
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, openHatBuf, OPENHAT_SAMPLES, MZK_RATE / OPENHAT_RATE);
    }
    for (int i = 386; i < 504; i += 4) {
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, openHatBuf, OPENHAT_SAMPLES, MZK_RATE / OPENHAT_RATE);
    }

    // Pulse stabs
    float f;
    for (int i = 0; i < 144; i += 1) {
        if (pulseNotes[i % 16] > -64) {
            f = mn2f<const char>(pulseNotes[i % 16] + 57);
            addSamples(buffer + ((MZK_RATE >> 2) * (i + 96)) * 2, pulseBuf, PULSE_SAMPLES, 27.5f / f);
        }
    }
}

void intro_end() {
    delete[] kickBuf;
    delete[] snareBuf;
    delete[] openHatBuf;
    delete[] pulseBuf;
}