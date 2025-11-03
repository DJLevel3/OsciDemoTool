//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#include "demo.h"

void twister(short* buffer);
void twisterDrums(short* buffer);

void cube(short* buffer);
void cubeDrums(short* buffer);

void demo_do(long itime, short* buffer, int section)
{
    int DEMO_NUMSAMPLESC = f2i(demo_length(section) * SAMPLE_RATE * 2);
    memset(buffer, 0, DEMO_NUMSAMPLESC * sizeof(short));
    switch (section) {
    case PRECALC_SECTION:
        return;
    case TWISTER_SECTION:
        return twister(buffer);
    case CUBE_SECTION:
        return cube(buffer);
    case PLASMA_SECTION:
        return cube(buffer);
    }
}

float demo_length(int section)
{
    switch (section) {
    case PRECALC_SECTION:
        return PRECALC_DURATION;
    case TWISTER_SECTION:
        return TWISTER_DURATION;
    case CUBE_SECTION:
        return CUBE_DURATION;
    case PLASMA_SECTION:
        return CUBE_DURATION;
    }
    return 2;
}

//---------------------------------------------------------------------
#define NOFF 0

static float border[] = {
    -1.f,  1.f, 0.f,
    -1.f, -1.f, 0.f,
     1.f, -1.f, 0.f,
     1.f,  1.f, 0.f,
    -1.f,  1.f, 0.f,
    -1.f, -1.f, 0.f,
};

const static char cubeIndices[] = {
    0, 1, 2, 3, 0,
    4, 5, 6, 7, 4,
    0, 1, 6, 7, 0,
    2, 3, 4, 5, 2,
    1, 2, 5, 6, 1,
    0, 3, 4, 7, 0,
};

static float cubeVerts[] = {
 -1.0f, -1.0f,  1.0f, // 0
 -1.0f,  1.0f,  1.0f, // 1
 -1.0f,  1.0f, -1.0f, // 2
 -1.0f, -1.0f, -1.0f, // 3

  1.0f, -1.0f, -1.0f, // 4
  1.0f,  1.0f, -1.0f, // 5
  1.0f,  1.0f,  1.0f, // 6
  1.0f, -1.0f,  1.0f, // 7
};

#define TWISTER_N_PITCHES 14
static const float twisterNoteTimes[TWISTER_N_PITCHES] = {
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
    1.25,
    0.25,
    0.25,
    0.25,
};

static const char twisterNotes[TWISTER_N_PITCHES] = {
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
   31,
   30,
   42,
};

#define CUBE_N_PITCHES 21
static const float cubeNoteTimes[CUBE_N_PITCHES] = {
    0.25,
    0.125,
    0.125,
    0.25,
    0.125,
    0.25,
    0.125,
    0.25,
    0.125,
    0.25,
    0.125,
    0.25,
    0.125,
    0.125,
    0.25,
    0.125,
    0.25,
    0.125,
    0.25,
    0.25,
    0.25
};

static const char cubeNotes[CUBE_N_PITCHES] = {
    30,
    42,
    NOFF,
    37,
    42,
    30,
    NOFF,
    30,
    42,
    30,
    42,
    26,
    38,
    NOFF,
    26,
    38,
    32,
    37,
    32,
    37,
    32
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

#define BORDER_SAMPLES 300

short* kickBuf;
short* snareBuf;
short* openHatBuf;
short* closedHatBuf;
#ifdef PULSES
short* pulseBuf;
short* pulse2Buf;
#endif

float rotMatrix1[16];
static int seed;

int demo_init( int itime )
{
    kickBuf = new short[KICK_SAMPLES * 2];
    snareBuf = new short[SNARE_SAMPLES * 2];
    openHatBuf = new short[OPENHAT_SAMPLES * 2];
    closedHatBuf = new short[CLOSEDHAT_SAMPLES * 2];
#ifdef PULSES
    pulseBuf = new short[PULSE_SAMPLES * 2];
    pulse2Buf = new short[PULSE2_SAMPLES * 2];
#endif

    seed = itime;

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
    return 1;
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

const unsigned short kickPattern = 0b1000001000100100;
const unsigned int hatPattern = 0b00000000010000000000000001000100;
const unsigned int hatRollPattern = 0b00000100000000000000001000000000;

void twister(short* buffer) {
    int DEMO_NUMSAMPLES = f2i(demo_length(TWISTER_SECTION) * SAMPLE_RATE);
    int s;
    float nextTime = 0;
    int p = 0;
    float freq = 110;
    float nSPC, nSPS;
    
    int nStrokes = TWISTER_SEGMENTS * 3;
    freq = mn2f(twisterNotes[0]);
    nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
    nSPS = nSPC / nStrokes;
    nextTime += SAMPLE_RATE * twisterNoteTimes[p];

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

    int counter = 0;
    float prog = 0;
    float targetProg = 0;
    int samples;
    float timer = 0;
    float yPos;
    float theta;
    float deflection;

    s = 0;

    for (; s + nSPC + BORDER_SAMPLES < 64.5 * SAMPLE_RATE;) {
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        yPos = twister[counter * 3 + 1];

#ifdef SPIRAL
        theta = max(timer - 8.f, 0.f)
            + (timer / 16.f) * (cosf(timer * 1.5f * M_PI) + 0.5f) * powf(M_E, -0.5 * powf(fmodf(timer, 8.f) - 4.f, 2.0f)) * powf(M_E, -6 * yPos * yPos)
            + yPos * M_PI * 3 / 4;
#else
        theta = max(timer - 8.f, 0.f)
            + (timer / 16.f) * (cosf(timer * 2.0f * M_PI) + 0.5f) * powf(M_E, -0.5 * powf(fmodf(timer, 8.f) - 4.f, 2.0f)) * powf(M_E, -4 * yPos * yPos)
            + -2 * sinf(M_PI * max(timer - 8.f, 0.f) * 0.125f) * (float(counter % TWISTER_SEGMENTS) / TWISTER_SEGMENTS) * (max(0.f, min((timer - 16) / 8, 2.f)) + 1);
#endif

        rotY(twister + counter * 3, line, theta);
        rotY(twister + counter * 3, line + 3, theta);
#ifndef SPIRAL
        line[4] += twisterHeight;
#endif

        if (line[2] > -TWISTER_WIDTH || (line[2] == -TWISTER_WIDTH && line[0] > 0)) { // line is visible
#ifdef SPIRAL
            line[0] = line[0] / 3.f - 0.5f;
            line[1] = 0;
            line[3] = line[3] / 3.f - 0.5f;
            line[4] = 0;
            rotZ(line, line, (yPos * M_PI));
            rotZ(line + 3, line + 3, ((yPos + twisterHeight) * M_PI));
#endif
            lineToSamples(line, line + 3, buffer + 2 * s, samples);
            s += samples;
        }

        counter = (counter + 1) % (TWISTER_SEGMENTS * 4);
        if (counter == 0) {
            timer += 1.f / freq;
            prog -= nSPC;
            targetProg -= nSPC;
            strokeToCycle2D(border, 6, buffer + 2 * s, BORDER_SAMPLES);
            s += BORDER_SAMPLES;
        }

        while (s > nextTime) {
            p++;
            nextTime += SAMPLE_RATE * twisterNoteTimes[p % TWISTER_N_PITCHES];
            freq = mn2f(twisterNotes[p % TWISTER_N_PITCHES]);
            nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
            nSPS = nSPC / nStrokes;
        }
    }
#ifndef SPIRAL
    wobbleBufferEnv(buffer + 16 * 2 * SAMPLE_RATE, SAMPLE_RATE * 32,   SAMPLE_RATE * 1.5, 0, 0.f,     0.f, 0.4f, 0.0f, 1.f );
    wobbleBufferEnv(buffer + 50 * 2 * SAMPLE_RATE, SAMPLE_RATE * 14.5, SAMPLE_RATE * 1.5, 0, 2.f / 3, 0.f, 0.4f, 0.0f, 0.5f);
    wobbleBufferEnv(buffer + 32 * 2 * SAMPLE_RATE, SAMPLE_RATE * 16,   SAMPLE_RATE,       0, 0.f,     0.f, 0.0f, 0.6f, 1.f );
#endif

    normalizeBuffer(buffer, DEMO_NUMSAMPLES, p0d45);
    delete[] twister;
    delete[] line;

    twisterDrums(buffer);
}
void twisterDrums(short* buffer) {
    const unsigned int cutoutPattern = 0b11111111000010101100111111000000;

    int i;
    // cutouts
    for (i = 768; i < 800; i++) {
        if (!(cutoutPattern & 1 << (31 - (i % 32)))) {
            for (int j = ((SAMPLE_RATE >> 4) * i); j < ((SAMPLE_RATE >> 4) * (i + 1)); j++) {
                buffer[j * 2] = 0;
                buffer[j * 2 + 1] = 0;
            }
        }
    }
    // sidechain
    for (i = 0; i < 512; i++) {
        if ((i <= 384 || i >= 400) && (kickPattern & 1 << (15 - (i % 16)))) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 24000, p0d10, p1d00);
        }
        else if ((i + 4) % 8 == 0 && i > 128) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 18000, p0d50, p1d00);
        }
    }

    // kick drums
    for (i = 0; i < 512; i++) {
        if ((i <= 384 || i >= 400) && (kickPattern & 1 << (15 - (i % 16)))) {
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        }
    }

    // snare drums
    for (i = 132; i < 512; i += 8) {
        addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, FILE_RATE / SNARE_RATE);
    }

    // hihats
    for (i = 256; i < 512; i++) {
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

void cube(short* buffer) {
    int DEMO_NUMSAMPLES = f2i(demo_length(CUBE_SECTION) * SAMPLE_RATE);
    int s;
    float nextTime = 0;
    int p = 0;
    int nStrokes;
    int faces;
    float freq, nSPC, nSPS;

    nStrokes = 0;
    while (nStrokes == 0) {
        faces = demo_rand(&seed) & 0b111111;
        for (int i = 0; i < 6; i++) {
            if (faces & (1 << i)) {
                nStrokes++;
            }
        }
    }

    freq = mn2f(cubeNotes[0]);
    if (cubeNotes[p % CUBE_N_PITCHES] == NOFF) freq = 110;
    nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
    nSPS = nSPC / nStrokes;
    nextTime += SAMPLE_RATE * cubeNoteTimes[p % CUBE_N_PITCHES];

    int counter = 0;
    float prog = 0;
    float targetProg = 0;
    int samples;
    float timer = 0;
    float yPos;
    float theta;
    float deflection;

    s = 0;

    float* currentStroke = new float[3 * 5];

    for (; s + nSPC + BORDER_SAMPLES < DEMO_NUMSAMPLES;) {
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        if ((s >= 8 * SAMPLE_RATE || faces & (1 << counter)) && (cubeNotes[p % CUBE_N_PITCHES] != NOFF)) {
            for (int v = 0; v < 5; v++) {
                currentStroke[v * 3 + 0] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 0];
                currentStroke[v * 3 + 1] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 1];
                currentStroke[v * 3 + 2] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 2];
                rotZ(currentStroke + v * 3, currentStroke + v * 3, timer * M_PI);
                rotY(currentStroke + v * 3, currentStroke + v * 3, timer * M_PI);
            }
            strokeToCycle3D(currentStroke, 5, buffer + 2 * s, samples, view_matrix);
            s += samples;
        }

        counter = (counter + 1) % 6;
        if (counter == 0) {
            prog -= nSPC;
            targetProg -= nSPC;
            if (cubeNotes[p % CUBE_N_PITCHES] != NOFF) {
                timer += 1.f / freq;
                strokeToCycle2D(border, 6, buffer + 2 * s, BORDER_SAMPLES);
            }
            s += BORDER_SAMPLES;
        }

        while (s > nextTime) {
            p++;

            if (s >= 8 * SAMPLE_RATE) {
                nStrokes = 6;
            }
            else {
                nStrokes = 0;
                while (nStrokes == 0) {
                    faces = demo_rand(&seed) & 0b111111;
                    for (int i = 0; i < 6; i++) {
                        if (faces & (1 << i)) {
                            nStrokes++;
                        }
                    }
                }
            }

            // check pitch and shit
            freq = mn2f(cubeNotes[p % CUBE_N_PITCHES]);
            if (cubeNotes[p % CUBE_N_PITCHES] == NOFF) freq = 110;
            nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
            nSPS = nSPC / nStrokes;
            nextTime += SAMPLE_RATE * cubeNoteTimes[p % CUBE_N_PITCHES];
        }
    }

    delete[] currentStroke;

    wobbleBufferEnv(buffer + 4 * SAMPLE_RATE * 2, SAMPLE_RATE * 28, SAMPLE_RATE / 2.0f, 0, 2.f / 3, 0.f, 0.1f, 0.0f, 0.5f);
    wobbleBufferEnv(buffer + 4 * SAMPLE_RATE * 2, SAMPLE_RATE * 28, SAMPLE_RATE / 3.0f, 0, 0.f, 2.f / 3, 0.0f, 0.1f, 0.5f);
    normalizeBuffer(buffer, DEMO_NUMSAMPLES, p0d45);

    cubeDrums(buffer);

}
void cubeDrums(short* buffer) {
    int i;
    // sidechain
    for (i = 128; i < 256; i++) {
        if (i <= 224 && (i % 4 == 0)) {//(kickPattern & (1 << (15 - (i % 16))))) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 18000, p0d10, p1d00);
        }
        else if ((i + 4) % 8 == 0 && i > 64) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 18000, p0d50, p1d00);
        }
    }

    // kick drums
    for (i = 128; i <= 224; i++) {
        if (i % 4 == 0) {//kickPattern & (1 << (15 - (i % 16)))) {
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        }
    }

    // snare drums
    for (i = 132; i < 256; i += 8) {
        addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, FILE_RATE / SNARE_RATE);
    }

    // hihats
    for (i = 64; i < 256; i += 2) {
        if (i % 32 == 26) addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, openHatBuf, min(OPENHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
        else addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
    }
    /*
    for (i = 256; i < 512; i++) {
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
    }*/
}