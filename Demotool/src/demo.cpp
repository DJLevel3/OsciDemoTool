//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#include "demo.h"

//---------------------------------------------------------------------
#define NOFF 0

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
  /*
 -1.0f, -1.0f,  1.0f, // 0
 -1.0f,  1.0f,  1.0f, // 1
 -1.0f,  1.0f, -1.0f, // 2
 -1.0f, -1.0f, -1.0f, // 3

  1.0f, -1.0f, -1.0f, // 4
  1.0f,  1.0f, -1.0f, // 5
  1.0f,  1.0f,  1.0f, // 6
  1.0f, -1.0f,  1.0f, // 7
  */
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

#define N_PITCHES 12
static const float pitchChangeTimes[N_PITCHES] = {
    7.25,
    0.25,
    0.25,
    0.25,
    1.5,
    0.5,
    1.5,
    0.5,
    1.5,
    0.5,
    1.75,
    0.25,
};

static const char notes[N_PITCHES] = {
   30,
   31,
   30,
   42,
   30,
   31,
   30,
   31,
   30,
   31,
   30,
   42,
};
#ifdef PULSES
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
#endif

static const unsigned short kickPattern = 0b1000001000100100;
static const unsigned int hatPattern =     0b00000000010000000000000001000100;
static const unsigned int hatRollPattern = 0b00000100000000000000001000000000;

#define OPENHAT_SAMPLES (24000)
#define CLOSEDHAT_SAMPLES (3000)
#define HAT_RATE (96000)

#ifdef PULSES
#define PULSE_WIDTH p0d10
#define PULSE_FREQ_BASE 27.5f
#define PULSE_SAMPLES (SAMPLE_RATE * 2)

#define PULSE2_WIDTH p0d95
#define PULSE2_SAMPLES (SAMPLE_RATE * 5 / 2)
#endif

#define HAT_PERIOD0 392
#define HAT_PERIOD1 314
#define HAT_PERIOD2 263
#define HAT_PERIOD3 231
#define HAT_PERIOD4 220
#define HAT_PERIOD5 155

#define TWISTER_SEGMENTS 128
#define TWISTER_WIDTH p0d40

#define BORDER_SAMPLES 400

short* kickBuf;
short* snareBuf;
short* openHatBuf;
short* closedHatBuf;
short* pulseBuf;
short* pulse2Buf;

float rotMatrix1[16];
#ifdef OCTA
float rotMatrix2[16];
float rotMatrix3[16];
#endif

int demo_init( void )
{
    kickBuf = new short[KICK_SAMPLES * 2];
    snareBuf = new short[SNARE_SAMPLES * 2];
    openHatBuf = new short[OPENHAT_SAMPLES * 2];
    closedHatBuf = new short[CLOSEDHAT_SAMPLES * 2];
#ifdef PULSES
    pulseBuf = new short[PULSE_SAMPLES * 2];
    pulse2Buf = new short[PULSE2_SAMPLES * 2];
#endif

    int seed = 1;

    // kick
    float kFreq = KICK_FMAX;
    float kSPC;
    int kSamp;
    for (int i = 0; i < KICK_SAMPLES;) {
        kSPC = FILE_RATE / kFreq;
        for (kSamp = 0; kSamp < kSPC && i < KICK_SAMPLES; kSamp++) {
            kickBuf[i * 2] = f2i(32768.f * squareWave(kSamp, kSPC, p0d40));
            kickBuf[i * 2 + 1] = f2i(32768.f * squareWave(kSamp + f2i(kSPC * 0.25f), kSPC, p0d40));
            i++;
        }
        if (kFreq > KICK_FBASE) kFreq -= KICK_FSTEP;
        else kFreq = KICK_FBASE;
    }
    normalizeBuffer(kickBuf, KICK_FADE_START, KICK_START_V);
    fadeBuffer(kickBuf + (KICK_FADE_START * 2), KICK_FADE_MID - KICK_FADE_START, KICK_START_V, KICK_MID_V);
    fadeBuffer(kickBuf + (KICK_FADE_MID * 2), KICK_FADE_END - KICK_FADE_MID, KICK_MID_V, KICK_END_V);
    normalizeBuffer(kickBuf + (KICK_FADE_END * 2), KICK_SAMPLES - KICK_FADE_END, KICK_END_V);

    // snare
    float sFreq = SNARE_FMAX;
    float sSPC;
    int sSamp;
    float n = 0;
    for (int i = 0; i < SNARE_SAMPLES; ) {
        sSPC = FILE_RATE / sFreq;
        sSamp = 0;
        for (sSamp = 0; sSamp < sSPC && i < SNARE_SAMPLES; sSamp++) {
            if (sSamp % 7 == 0) n = short(demo_rand(&seed)) * powf(min(max((float)i - SNARE_WIREWAIT, 0), (float)SNARE_WIRETIME) / (SNARE_WIRETIME), 2.0f);
            snareBuf[i * 2] = f2i(0.5f * SHRT_MAX * squareWave(sSamp, sSPC, p0d60) + n);
            snareBuf[i * 2 + 1] = snareBuf[i * 2];
            i++;
        }
        if (sFreq > SNARE_FBASE) sFreq -= SNARE_FSTEP;
        else sFreq = SNARE_FBASE;
    }
    normalizeBuffer(snareBuf, SNARE_SAMPLES, p0d80);
    fadeBuffer(snareBuf + SNARE_SAMPLES, SNARE_SAMPLES / 2, 1.f, 0.f);

    // hihat
    float state1 = 0;
    float state2 = 0;
    //float state3 = 0;
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
        state1 += p0d37 * (retval + state1 - 2 * state2);
        state2 += p0d37 * (state1 - state2);
        //state3 += p0d45 * (state2 - state3);
        openHatBuf[i * 2] = f2i((retval - state2));
        openHatBuf[i * 2 + 1] = openHatBuf[i * 2];
        if (i < CLOSEDHAT_SAMPLES) {
            closedHatBuf[i * 2] = openHatBuf[i * 2];
            closedHatBuf[i * 2 + 1] = openHatBuf[i * 2];
        }
    }
    normalizeBuffer(openHatBuf, OPENHAT_SAMPLES, 1.f);
    fadeBuffer(openHatBuf, OPENHAT_SAMPLES, p0d65, p0d00);

    normalizeBuffer(closedHatBuf, CLOSEDHAT_SAMPLES, 1.f);
    fadeBuffer(closedHatBuf, CLOSEDHAT_SAMPLES, p0d65, p0d00);

#ifdef PULSES
    int pw;
    int oneCycle = (f2i(0.5f * SAMPLE_RATE / PULSE_FREQ_BASE));

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
#endif

#ifdef OCTA
    eulerMat(rotMatrix2, 0, p0d07, 0);
    eulerMat(rotMatrix3, p0d50, 0, -p0d79);
#endif
    return 1;
}

void demo_do(long itime, short* buffer)
{
    int s, vi;
    int rotationCounter;
    float nextTime = 0;
    int p = 0;

    for (s = 0; s < DEMO_NUMSAMPLESC; s++) {
        buffer[s] = 0;
    }

    unsigned char object = 0;
    int offset = 0;
    int nStrokes = TWISTER_SEGMENTS * 3;
    float freq = mn2f(notes[0]);
    float nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
    float nSPS = nSPC / nStrokes;
    nextTime += SAMPLE_RATE * pitchChangeTimes[p];

    float* twister = new float[3 * TWISTER_SEGMENTS * 4];
    float* line = new float[6];

    float twisterHeight = 2.f / TWISTER_SEGMENTS;
    for (int i = 0; i < TWISTER_SEGMENTS; i++) {
        float h = i * twisterHeight - 1.f;

        twister[3 * (TWISTER_SEGMENTS * 0 + i) + 0] = TWISTER_WIDTH;
        twister[3 * (TWISTER_SEGMENTS * 0 + i) + 1] = h;
        twister[3 * (TWISTER_SEGMENTS * 0 + i) + 2] = -TWISTER_WIDTH;

        twister[3 * (TWISTER_SEGMENTS * 1 + i) + 0] = TWISTER_WIDTH;
        twister[3 * (TWISTER_SEGMENTS * 1 + i) + 1] = h;
        twister[3 * (TWISTER_SEGMENTS * 1 + i) + 2] = TWISTER_WIDTH;

        twister[3 * (TWISTER_SEGMENTS * 2 + i) + 0] = -TWISTER_WIDTH;
        twister[3 * (TWISTER_SEGMENTS * 2 + i) + 1] = h;
        twister[3 * (TWISTER_SEGMENTS * 2 + i) + 2] = TWISTER_WIDTH;

        twister[3 * (TWISTER_SEGMENTS * 3 + i) + 0] = -TWISTER_WIDTH;
        twister[3 * (TWISTER_SEGMENTS * 3 + i) + 1] = h;
        twister[3 * (TWISTER_SEGMENTS * 3 + i) + 2] = -TWISTER_WIDTH;
    }

    float border[] = {
        -1.f,  1.f, 0.f,
        -1.f, -1.f, 0.f,
         1.f, -1.f, 0.f,
         1.f,  1.f, 0.f,
        -1.f,  1.f, 0.f,
        -1.f, -1.f, 0.f,
    };

    int counter = 0;
    int counter2 = 0;
    float prog = 0;
    float targetProg = 0;
    int samples;
    float timer = 0;
    float yPos;
    float theta;
    float deflection;

    s = 0;

    for (; s + nSPC + BORDER_SAMPLES < DEMO_NUMSAMPLES;) {
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        yPos = twister[counter * 3 + 1];
        theta = max(timer - 8.f, 0.f)
            + (timer / 16.f) * (cosf(timer * 1.5f * M_PI) + 0.5f) * powf(M_E, -0.5 * powf(fmodf(timer, 8.f) - 4.f, 2.0f)) * powf(M_E, -4 * yPos * yPos)
            + -2 * sinf(M_PI * max(timer - 8.f, 0.f) / 4.f) * (float(counter % TWISTER_SEGMENTS) / TWISTER_SEGMENTS) * (timer > 16 ? 2 : 1);

        deflection = sinf(max(timer - 32.f, 0.f) * M_PI * yPos);

        rotY(twister + counter * 3, line, theta);
        rotY(twister + counter * 3, line + 3, theta);
        line[4] += twisterHeight;

        if (line[2] > -TWISTER_WIDTH || (line[2] == -TWISTER_WIDTH && line[0] > 0)) { // line is visible
            lineToSamples(line, line + 3, buffer + 2 * s, samples);
            s += samples;
        }

        counter = (counter + 1) % (TWISTER_SEGMENTS * 4);
        counter2++;
        if (counter == 0) {
            timer += 1.f / freq;
            prog -= nSPC;
            targetProg -= nSPC;
            strokeToCycle2D(border, 6, buffer + 2 * s, BORDER_SAMPLES);
            s += BORDER_SAMPLES;
        }

        while (s > nextTime) {
            p++;
            nextTime += SAMPLE_RATE * pitchChangeTimes[p % N_PITCHES];
            // check pitch and shit
            freq = mn2f(notes[p % N_PITCHES]); // TODO
            nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
            nSPS = nSPC / nStrokes;
        }
    }
    wobbleBufferEnv(buffer + 16 * 2 * SAMPLE_RATE, SAMPLE_RATE * 32, SAMPLE_RATE * 1.5, 0, 0.f, 0.f, 0.4f, 0.0f, 1.f);
    wobbleBufferEnv(buffer + 32 * 2 * SAMPLE_RATE, SAMPLE_RATE * 32, SAMPLE_RATE, 0, 0.f, 0.f, 0.0f, 0.6f, 1.f);

    normalizeBuffer(buffer, DEMO_NUMSAMPLES, p0d45);
    delete[] twister;
    delete[] line;

    drums(buffer);
}

void drums(short* buffer) {
    // sidechain
    for (int i = 0; i <= 512; i++) {
        if ((i <= 384 || i >= 400) && (kickPattern & 1 << (15 - (i % 16)))) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 24000, p0d10, p1d00);
        }
        else if ((i + 4) % 8 == 0 && i > 128) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 18000, p0d50, p1d00);
        }
    }

    // kick drums
    for (int i = 0; i <= 512; i++) {
        if ((i <= 384 || i >= 400) && (kickPattern & 1 << (15 - (i % 16)))) {
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        }
    }

    // snare drums
    for (int i = 132; i < 512; i += 8) {
        addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, FILE_RATE / SNARE_RATE);
    }

    // hihats
    for (int i = 256; i < 512; i++) {
        if (i < 384 || i >= 400) {
            if (hatRollPattern & (1 << (31 - (i % 32)))) {
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 4), FILE_RATE / HAT_RATE);
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i + (SAMPLE_RATE >> 4)) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 5), FILE_RATE / HAT_RATE);
            }
            else if (hatPattern & (1 << (31 - (i % 32)))) {
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, openHatBuf, min(OPENHAT_SAMPLES, SAMPLE_RATE >> 3), FILE_RATE / HAT_RATE);
            }
            else {
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 3), FILE_RATE / HAT_RATE);
            }
        }
    }

#ifdef PULSES
    // Pulse stabs
    float f;
    int n;
    int i;
    for (i = 192; i < 256; i += 2) {
        n = ((i - 192) >> 1) % 16;
        if (pulseNotes[n] != NOFF) {
            f = mn2f(pulseNotes[n]);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulseBuf, PULSE_SAMPLES, PULSE_FREQ_BASE / f);
        }
    }
    for (i = 256; i < 448; i++) {
        n = (i - 256) % 32;
        if (pulseNotes2[n] != NOFF) {
            f = mn2f(pulseNotes2[n]);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulseBuf, PULSE_SAMPLES, PULSE_FREQ_BASE / f);
        }
    }
    for (i = 448; i < 496; i += 2) {
        n = ((i - 448) >> 1) % 16;
        if (pulseNotes[n] != NOFF) {
            f = mn2f(pulseNotes[n]);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulseBuf, PULSE_SAMPLES, PULSE_FREQ_BASE / f);
        }
    }

    for (i = 256; i < 506; i += 2) {
        n = ((i - 256) >> 1) % 16;
        if (pulseNotes3[n] != NOFF) {
            f = mn2f(pulseNotes3[n]);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulse2Buf, PULSE2_SAMPLES, PULSE_FREQ_BASE / f);
        }
    }

#endif
    /*
    for (s = 0; s < DEMO_NUMSAMPLESC; s++) {
        buffer[s] *= 0.5;
    }/**/
}

void demo_end() {
    delete[] kickBuf;
    delete[] snareBuf;
    delete[] openHatBuf;
#ifdef PULSES
    delete[] pulseBuf;
    delete[] pulse2Buf;
#endif
}