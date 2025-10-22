//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#include "intro.h"

//---------------------------------------------------------------------
#define NOFF -128

const static char objects[] = {
    0, 6,
};

const static char vertIndices[] = {
     3,  2,  5,  4,  3,
     7,  6,  1,  0,  7,
     0,  6,  4,  2,  0,
     3,  4,  7,  0,  3,
     1,  6,  5,  2,  1,
     0,  6,  4,  2,  0, // 5 - end of cube

#ifdef OCTA
    10,  9, 11,  8, 10,
    12, 11, 13, 10, 12,
     9, 12,  8, 13,  9, // 8 - end of octahedron
    12, 11, 13, 10, 12,
    10,  9, 11,  8, 10,
     8, 13,  9, 12,  8 // 11 - end of octahedron 2
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

#ifdef OCTA
  0.0f,  0.0f,  0.0f, // 8  (placeholder) - back
  0.0f,  0.0f,  0.0f, // 9  (placeholder) - front
  0.0f,  0.0f,  0.0f, // 10 (placeholder) - bottom
  0.0f,  0.0f,  0.0f, // 11 (placeholder) - top
  0.0f,  0.0f,  0.0f, // 12 (placeholder) - left
  0.0f,  0.0f,  0.0f, // 13 (placeholder) - right
#endif
};

#ifdef OCTA
static float sourceVerts[] = {
  0.0f,  0.0f,  -1.5f, // 8  - back
  0.0f,  0.0f,   1.5f, // 9  - front
  0.0f, -1.5f,   0.0f, // 10 - bottom
  0.0f,  1.5f,   0.0f, // 11 - top
 -1.5f,  0.0f,   0.0f, // 12 - left
  1.5f,  0.0f,   0.0f, // 13 - right
};
#endif

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

/*
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
};*/

static const char pulseNotes[] = {
    NOFF,
    57,
    60,
    NOFF,
    NOFF,
    55,
    59,
    55,
    NOFF,
    53,
    57,
    53,
    55,
    60,
    59,
    55,
};

static const char pulseNotes2[] = {
    NOFF,
    NOFF,
    57,
    60,
    64,
    NOFF,
    NOFF,
    NOFF,
    NOFF,
    NOFF,
    55,
    59,
    62,
    NOFF,
    59,
    NOFF,
    NOFF,
    NOFF,
    53,
    57,
    60,
    NOFF,
    57,
    NOFF,
    59,
    NOFF,
    60,
    NOFF,
    59,
    NOFF,
    55,
    NOFF,
};

static const char pulseNotes3[] = {
    52,
    52,
    57,
    52,
    50,
    50,
    55,
    50,
    48,
    48,
    53,
    48,
    50,
    50,
    55,
    50,
};

static const unsigned short kickPattern = 0b1001101010001000;

#define OPENHAT_SAMPLES (16000)
#define OPENHAT_RATE (96000)

#define PULSE_WIDTH p0d10
#define PULSE_FREQ_BASE 27.5f
#define PULSE_SAMPLES (MZK_RATE * 2)

#define PULSE2_WIDTH p0d05
#define PULSE2_SAMPLES (MZK_RATE * 5 / 2)

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
short* pulse2Buf;

float rotMatrix1[16];
#ifdef OCTA
float rotMatrix2[16];
float rotMatrix3[16];
#endif

int intro_init( void )
{
    kickBuf = new short[KICK_SAMPLES * 2];
    snareBuf = new short[SNARE_SAMPLES * 2];
    openHatBuf = new short[OPENHAT_SAMPLES * 2];
    pulseBuf = new short[PULSE_SAMPLES * 2];
    pulse2Buf = new short[PULSE2_SAMPLES * 2];

    int seed = 1;

    // kick
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

    // snare
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

    // hihat
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
    int oneCycle = (f2i(0.5f * MZK_RATE / PULSE_FREQ_BASE));

    // first pulse buffer
    for (pw = 0; pw < PULSE_SAMPLES; pw++) {
        pulseBuf[pw * 2] = SHRT_MAX * squareWave(pw, oneCycle, PULSE_WIDTH);
        pulseBuf[pw * 2 + 1] = pulseBuf[pw * 2];
    }
    fadeBuffer(pulseBuf, PULSE_SAMPLES, p0d25, p0d00);
    fadeBuffer(pulseBuf, PULSE_SAMPLES / 2, p1d99, p1d00);

    // second pulse buffer
    for (pw = 0; pw < PULSE_SAMPLES; pw++) {
        pulse2Buf[pw * 2] = SHRT_MAX * squareWave(pw, oneCycle, PULSE2_WIDTH);
        pulse2Buf[pw * 2 + 1] = pulse2Buf[pw * 2];
    }
    fadeBuffer(pulse2Buf, PULSE2_SAMPLES, p0d25, p0d00);
    fadeBuffer(pulse2Buf, PULSE2_SAMPLES / 2, p1d99, p1d00);

    // cube rotation
    eulerMat(rotMatrix1, p0d03, p0d02, p0d05);
#ifdef OCTA
    eulerMat(rotMatrix2, 0, p0d07, 0);
    eulerMat(rotMatrix3, p0d50, 0, -p0d79);
#endif
    return 1;
}

void intro_do( long itime , short * buffer )
{
    int s,vi;
    float nextTime = 0;
    int p = 0;

    unsigned char object = 0;
    int offset = objects[2 * object];
    int nStrokes = objects[2 * object + 1];
    float nSPC = f2i(MZK_RATE / mn2f<const char>(notes[0]));
    int nSPS = f2i(nSPC / nStrokes);
    int correction = f2i(nSPC - (nStrokes * nSPS));
    nextTime += pitchChangeTimes[p];

    float* curStroke = new float[15];
    for (s = 0; s + nSPC < DEMO_NUMSAMPLES;) {
        // generate the shape
        for (int i = 0; i < nStrokes; i += 1) {
            for (int v = 0; v < 5; v++) {
#ifdef OCTA
                vi = vertIndices[(i + offset) * 5 + v] * 3;
#else
                vi = vertIndices[i * 5 + v] * 3;
#endif
                curStroke[v * 3 + 0] = verts[vi + 0];
                curStroke[v * 3 + 1] = verts[vi + 1];
                curStroke[v * 3 + 2] = verts[vi + 2];
            }
            strokeToCycle(curStroke, 5, buffer + (2 * s), nSPS);
            s += nSPS;
        }
        for (int i = 0; i < correction; i++) {
            buffer[2 * s] = buffer[2 * s - 2];
            buffer[2 * s + 1] = buffer[2 * s - 1];
            s++;
        }
        for (int i = 0; i < 8; i++) {
            mvp43(rotMatrix1, verts + (i * 3), verts + (i * 3));
        }
#ifdef OCTA
        for (int i = 0; i < 6; i++) {
            mvp43(rotMatrix2, sourceVerts + (i * 3), sourceVerts + (i * 3));
            mvp43(rotMatrix3, sourceVerts + (i * 3), verts + ((i + 8) * 3));
        }

        object = (s < DEMO_NUMSAMPLES / 2) ? 1 : ((s * 2 / MZK_RATE) % 2);
        offset = objects[2 * object];
        nStrokes = objects[2 * object + 1];

        nSPC = MZK_RATE / mn2f<const char>(notes[p % 24]);
        nSPS = f2i(nSPC / nStrokes);
        correction = f2i(nSPC - (nStrokes * nSPS));
#endif

        while (s > MZK_RATE * nextTime) {
            p++;
            nextTime += pitchChangeTimes[p % 12];
            // check pitch and shit
            nSPC = MZK_RATE / mn2f<const char>(notes[p % 24]);
            nSPS = f2i(nSPC / nStrokes);
            correction = f2i(nSPC - (nStrokes * nSPS));
        }
    }
    for (; s < DEMO_NUMSAMPLES; s++) {
        buffer[s * 2] = 0;
        buffer[s * 2 + 1] = 0;
    }
    normalizeBuffer(buffer, DEMO_NUMSAMPLES, p0d45);
    delete[] curStroke;

    
    // sidechain
    for (int i = 0; i < 258; i++) {
        if (i % 64 == 62) {
            fadeBuffer(buffer + ((MZK_RATE >> 3) * i) * 2, 20000, p0d30, p1d00);
        } else if (i < 64) {
            if (i % 4 == 0) fadeBuffer(buffer + ((MZK_RATE >> 3) * i) * 2, 20000, p0d30, p1d00);
        } else if ((kickPattern & 1 << (15 - (i % 16)))) {
            fadeBuffer(buffer + ((MZK_RATE >> 3) * i) * 2, 20000, p0d30, p1d00);
        }
    }
    for (int i = 324; i < 368; i += 8) {
        fadeBuffer(buffer + ((MZK_RATE >> 3) * i) * 2, 20000, p0d50, p1d00);
    }
    for (int i = 368; i < 476; i ++) {
        if (i % 64 == 62) {
            fadeBuffer(buffer + ((MZK_RATE >> 3) * i) * 2, 20000, p0d30, p1d00);
        } else if (i < 384) {
            if (i % 4 == 0) fadeBuffer(buffer + ((MZK_RATE >> 3) * i) * 2, 20000, p0d30, p1d00);
        } else if ((kickPattern & 1 << (15 - (i % 16)))) {
            fadeBuffer(buffer + ((MZK_RATE >> 3) * i) * 2, 20000, p0d30, p1d00);
        }
    }

    // kick drums
    for (int i = 0; i < 258; i ++) {
        if (i % 64 == 62) {
            addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, MZK_RATE / KICK_RATE);
        } else if (i < 64) {
            if (i % 4 == 0) addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, MZK_RATE / KICK_RATE);
        } else if ((kickPattern & 1 << (15 - (i % 16)))) {
            addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, MZK_RATE / KICK_RATE);
        }
    }
    for (int i = 368; i < 476; i ++) {
        if (i % 64 == 62) {
            addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, MZK_RATE / KICK_RATE);
        } else if (i < 384) {
            if (i % 4 == 0) addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, MZK_RATE / KICK_RATE);
        } else if ((kickPattern & 1 << (15 - (i % 16)))) {
            addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, MZK_RATE / KICK_RATE);
        }
    }

    // snare drums
    for (int i = 68; i < 248; i += 8) {
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, MZK_RATE / SNARE_RATE);
    }
    for (int i = 324; i < 480; i += 8) {
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, MZK_RATE / SNARE_RATE);
    }

    // open hihats
    for (int i = 130; i < 248; i += 4) {
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, openHatBuf, OPENHAT_SAMPLES, MZK_RATE / OPENHAT_RATE);
    }
    for (int i = 386; i < 504; i += 4) {
        addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, openHatBuf, OPENHAT_SAMPLES, MZK_RATE / OPENHAT_RATE);
    }

    // Pulse stabs
    float f;
    int n;
    int i;
    for (i = 192; i < 256; i += 2) {
        n = ((i - 192) >> 1) % 16;
        if (pulseNotes[n] != NOFF) {
            f = mn2f<const char>(pulseNotes[n]);
            addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, pulseBuf, PULSE_SAMPLES, PULSE_FREQ_BASE / f);
        }
    }
    for (i = 256; i < 384; i++) {
        n = (i - 256) % 32;
        if (pulseNotes2[n] != NOFF) {
            f = mn2f<const char>(pulseNotes2[n]);
            addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, pulseBuf, PULSE_SAMPLES, PULSE_FREQ_BASE / f);
        }
    }
    for (i = 384; i < 480; i += 2) {
        n = ((i - 384) >> 1) % 16;
        if (pulseNotes[n] != NOFF) {
            f = mn2f<const char>(pulseNotes[n]);
            addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, pulseBuf, PULSE_SAMPLES, PULSE_FREQ_BASE / f);
        }
    }

    for (i = 256; i < 506; i += 2) {
        n = ((i - 256) >> 1) % 16;
        if (pulseNotes3[n] != NOFF) {
            f = mn2f<const char>(pulseNotes3[n]);
            addSamples(buffer + ((MZK_RATE >> 3) * i) * 2, pulse2Buf, PULSE2_SAMPLES, PULSE_FREQ_BASE / f);
        }
    }
}

void intro_end() {
    delete[] kickBuf;
    delete[] snareBuf;
    delete[] openHatBuf;
    delete[] pulseBuf;
    delete[] pulse2Buf;
}