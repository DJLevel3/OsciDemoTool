//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#include "demo.h"

void intro(short* buffer);

void cube(short* buffer);
void cubeDrums(short* buffer);

void plasma(short* buffer);

void twister(short* buffer);
void twisterDrums(short* buffer);

void finale(short* buffer);
void finaleDrums(short* buffer, bool pulses);

void outro(short* buffer);

short* kickBuf;
short* snareBuf;
short* openHatBuf;
short* closedHatBuf;
short* crashBuf;
short* riserBuf;
short* pulse1Buf;
short* pulse2Buf;
short* pulse3Buf;
short* pulse4Buf;
float* twisterVerts;

#define OPENHAT_SAMPLES (48000)
#define CLOSEDHAT_SAMPLES (6000)
#define HAT_RATE (192000)

#define HAT_PERIOD0 (392 * 2)
#define HAT_PERIOD1 (314 * 2)
#define HAT_PERIOD2 (263 * 2)
#define HAT_PERIOD3 (231 * 2)
#define HAT_PERIOD4 (220 * 2)
#define HAT_PERIOD5 (155 * 2)

#define CRASH_SAMPLES 24000
#define RISER_SAMPLES 24000
#define CRASH_RATE 12000

void demo_do(long itime, short* buffer, int section)
{
    int DEMO_NUMSAMPLESC = f2i(demo_length(section) * SAMPLE_RATE * 2);
    switch (section) {
    case INTRO_SECTION:
        return intro(buffer);
    case TWISTER_SECTION:
        return twister(buffer);
    case CUBE_SECTION:
        return cube(buffer);
    case PLASMA_SECTION:
        return plasma(buffer);
    case FINALE_SECTION:
        finale(buffer);
        return finaleDrums(buffer, false);
    case OUTRO_SECTION:
        return outro(buffer);
    default:
        memset(buffer, 0, DEMO_NUMSAMPLESC * sizeof(short));
    }
}

float demo_length(int section)
{
    switch (section) {
    case INTRO_SECTION:
        return INTRO_SECONDS;
    case TWISTER_SECTION:
        return TWISTER_SECONDS;
    case CUBE_SECTION:
        return CUBE_SECONDS;
    case PLASMA_SECTION:
        return PLASMA_SECONDS;
    case FINALE_SECTION:
        return FINALE_SECONDS;
    case OUTRO_SECTION:
        return OUTRO_SECONDS;
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
    5, 6, 7, 4, 5,
    5, 2, 3, 4, 5,
    0, 1, 6, 7, 0,
    0, 3, 4, 7, 0,
    5, 6, 1, 2, 5,
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

#define TWISTER_SEGMENTS 256
#define TWISTER_WIDTH p0d30
#define TWISTER_N_PITCHES 14
#define TWISTER_N_TIMES 14
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
#define CUBE_N_TIMES 21
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
    28,
    40,
    28,
    40,
    28
};

#define FINALE_N_PITCHES 12
#define FINALE_N_TIMES 6
static const float finaleNoteTimes[FINALE_N_PITCHES] = {
    1.5,
    1,
    1.5,
    1.5,
    1,
    1.5,
};
static const char finaleNotes[FINALE_N_PITCHES] = {
    30,
    NOFF,
    32,
    33,
    NOFF,
    37,
    42,
    NOFF,
    38,
    35,
    NOFF,
    32
};

static const char pulse1Notes[3] = {
    66,
    69,
    73
};

static const char pulse2Notes[3] = {
    66,
    69,
    74
};

static const char pulse3Notes[3] = {
    68,
    71,
    74
};

static const char pulse4Notes[3] = {
    68,
    71,
    76
};

float twisterHeight;
float rotMatrix1[16];
static int seed = 0;

#define PULSE_SAMPLES SAMPLE_RATE / 4

int demo_init( int itime )
{
    kickBuf = (short*)malloc(sizeof(short) * KICK_SAMPLES * 2);
    if (kickBuf == 0) return 0;
    snareBuf = (short*)malloc(sizeof(short) * SNARE_SAMPLES * 2);
    if (snareBuf == 0) {
        free(kickBuf);
        return 0;
    }
    openHatBuf = (short*)malloc(sizeof(short) * OPENHAT_SAMPLES * 2);
    if (openHatBuf == 0) {
        free(kickBuf);
        free(snareBuf);
        return 0;
    }
    closedHatBuf = (short*)malloc(sizeof(short) * CLOSEDHAT_SAMPLES * 2);
    if (closedHatBuf == 0) {
        free(kickBuf);
        free(snareBuf);
        free(openHatBuf);
        return 0;
    }
    crashBuf = (short*)malloc(sizeof(short) * CRASH_SAMPLES * 2);
    if (crashBuf == 0) {
        free(kickBuf);
        free(snareBuf);
        free(openHatBuf);
        free(closedHatBuf);
        return 0;
    }
    pulse1Buf = (short*)malloc(sizeof(short) * PULSE_SAMPLES * 2);
    if (pulse1Buf == 0) {
        free(kickBuf);
        free(snareBuf);
        free(openHatBuf);
        free(closedHatBuf);
        free(crashBuf);
        return 0;
    }
    pulse2Buf = (short*)malloc(sizeof(short) * PULSE_SAMPLES * 2);
    if (closedHatBuf == 0) {
        free(kickBuf);
        free(snareBuf);
        free(openHatBuf);
        free(closedHatBuf);
        free(crashBuf);
        free(pulse1Buf);
        return 0;
    }
    pulse3Buf = (short*)malloc(sizeof(short) * PULSE_SAMPLES * 2);
    if (closedHatBuf == 0) {
        free(kickBuf);
        free(snareBuf);
        free(openHatBuf);
        free(closedHatBuf);
        free(crashBuf);
        free(pulse1Buf);
        free(pulse2Buf);
        return 0;
    }
    pulse4Buf = (short*)malloc(sizeof(short) * PULSE_SAMPLES * 2);
    if (closedHatBuf == 0) {
        free(kickBuf);
        free(snareBuf);
        free(openHatBuf);
        free(closedHatBuf);
        free(crashBuf);
        free(pulse1Buf);
        free(pulse2Buf);
        free(pulse3Buf);
        return 0;
    }
    twisterVerts = (float*)malloc(sizeof(float) * 3 * TWISTER_SEGMENTS * 4);
    if (twisterVerts == 0) {
        free(kickBuf);
        free(snareBuf);
        free(openHatBuf);
        free(closedHatBuf);
        free(crashBuf);
        free(pulse1Buf);
        free(pulse2Buf);
        free(pulse3Buf);
        free(pulse4Buf);
        return 0;
    }
    riserBuf = (short*)malloc(sizeof(short) * RISER_SAMPLES * 2);
    if (riserBuf == 0) {
        free(kickBuf);
        free(snareBuf);
        free(openHatBuf);
        free(closedHatBuf);
        free(crashBuf);
        free(pulse1Buf);
        free(pulse2Buf);
        free(pulse3Buf);
        free(pulse4Buf);
        free(twisterVerts);
        return 0;
    }

    twisterHeight = 2.f / TWISTER_SEGMENTS;
    for (int i = 0; i < TWISTER_SEGMENTS; i++) {
        float h = i * twisterHeight - 1.f;

        twisterVerts[3 * (TWISTER_SEGMENTS * 0 + i) + 0] = TWISTER_WIDTH;
        twisterVerts[3 * (TWISTER_SEGMENTS * 0 + i) + 1] = h;
        twisterVerts[3 * (TWISTER_SEGMENTS * 0 + i) + 2] = -TWISTER_WIDTH;

        twisterVerts[3 * (TWISTER_SEGMENTS * 1 + i) + 0] = TWISTER_WIDTH;
        twisterVerts[3 * (TWISTER_SEGMENTS * 1 + i) + 1] = h;
        twisterVerts[3 * (TWISTER_SEGMENTS * 1 + i) + 2] = TWISTER_WIDTH;

        twisterVerts[3 * (TWISTER_SEGMENTS * 2 + i) + 0] = -TWISTER_WIDTH;
        twisterVerts[3 * (TWISTER_SEGMENTS * 2 + i) + 1] = h;
        twisterVerts[3 * (TWISTER_SEGMENTS * 2 + i) + 2] = TWISTER_WIDTH;

        twisterVerts[3 * (TWISTER_SEGMENTS * 3 + i) + 0] = -TWISTER_WIDTH;
        twisterVerts[3 * (TWISTER_SEGMENTS * 3 + i) + 1] = h;
        twisterVerts[3 * (TWISTER_SEGMENTS * 3 + i) + 2] = -TWISTER_WIDTH;
    }

    seed = itime;

    // kick
    float kFreq = KICK_FMAX;
    float kSPC;
    int kSamp;
    for (int i = 0; i < KICK_SAMPLES;) {
        kSPC = FILE_RATE / kFreq;
        for (kSamp = 0; kSamp < kSPC && i < KICK_SAMPLES; kSamp++) {
            kickBuf[i * 2] = f2i(SHRT_MAX * squareWaveCentered(kSamp, f2i(kSPC), p0d60));
            kickBuf[i * 2 + 1] = f2i(SHRT_MAX * squareWaveCentered(kSamp + f2i(kSPC * 0.25f), f2i(kSPC), p0d60));
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
            if (sSamp % 7 == 0) n = demo_rand(&seed) * powf(min(max((float)i - SNARE_WIREWAIT, 0), (float)SNARE_WIRETIME) / (SNARE_WIRETIME), 2.0f);
            snareBuf[i * 2] = f2i(0.5f * SHRT_MAX * squareWaveNoDC(sSamp, f2i(sSPC), p0d40) + n);
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
            squareWaveNoDC(i, HAT_PERIOD0, 0.5f) +
            squareWaveNoDC(i, HAT_PERIOD1, 0.5f) +
            squareWaveNoDC(i, HAT_PERIOD2, 0.5f) +
            squareWaveNoDC(i, HAT_PERIOD3, 0.5f) +
            squareWaveNoDC(i, HAT_PERIOD4, 0.5f) +
            squareWaveNoDC(i, HAT_PERIOD5, 0.5f)
            );
#endif
        state1 += p0d37 * 0.5 * (retval + state1 - 2 * state2);
        state2 += p0d37 * 0.5 * (state1 - state2);
        //state3 += p0d45 * (state2 - state3);
        openHatBuf[i * 2] = f2i(retval - state2);
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

    for (int i = 0; i < CRASH_SAMPLES; i++) {
        crashBuf[i * 2] = demo_rand(&seed);
        crashBuf[i * 2 + 1] = demo_rand(&seed);
        riserBuf[i * 2] = demo_rand(&seed);
        riserBuf[i * 2 + 1] = demo_rand(&seed);
    }
    fadeBuffer(crashBuf, CRASH_SAMPLES, p0d30, p0d00);
    fadeBuffer(riserBuf, RISER_SAMPLES, p0d00, p0d50);
    filterBuffer(riserBuf, RISER_SAMPLES, p0d85, 0, p0d65);

    for (int i = 0; i < 8; i++) {
        for (int s = 0; s < PULSE_SAMPLES / 8; s++) {
            pulse1Buf[s * 2 + i * PULSE_SAMPLES / 8] = f2i(SHRT_MAX * squareWaveNoDC(s, f2i(SAMPLE_RATE / mn2f(pulse1Notes[i % 3])), 0.5));
            pulse1Buf[s * 2 + 1 + i * PULSE_SAMPLES / 8] = pulse1Buf[s * 2 + i * PULSE_SAMPLES / 8];
        }
    }
    fadeBuffer(pulse1Buf, PULSE_SAMPLES, 0.5f, 0.f);

    for (int i = 0; i < 8; i++) {
        for (int s = 0; s < PULSE_SAMPLES / 8; s++) {
            pulse2Buf[s * 2 + i * PULSE_SAMPLES / 8] = f2i(SHRT_MAX * squareWaveNoDC(s, f2i(SAMPLE_RATE / mn2f(pulse2Notes[i % 3])), 0.5));
            pulse2Buf[s * 2 + 1 + i * PULSE_SAMPLES / 8] = pulse2Buf[s * 2 + i * PULSE_SAMPLES / 8];
        }
    }
    fadeBuffer(pulse2Buf, PULSE_SAMPLES, 0.5f, 0.f);

    for (int i = 0; i < 8; i++) {
        for (int s = 0; s < PULSE_SAMPLES / 8; s++) {
            pulse3Buf[s * 2 + i * PULSE_SAMPLES / 8] = f2i(SHRT_MAX * squareWaveNoDC(s, f2i(SAMPLE_RATE / mn2f(pulse3Notes[i % 3])), 0.5));
            pulse3Buf[s * 2 + 1 + i * PULSE_SAMPLES / 8] = pulse3Buf[s * 2 + i * PULSE_SAMPLES / 8];
        }
    }
    fadeBuffer(pulse3Buf, PULSE_SAMPLES, 0.5f, 0.f);

    memset(pulse4Buf, 0, PULSE_SAMPLES * 2 * sizeof(short));
    for (int i = 0; i < 8; i++) {
        for (int s = 0; s < PULSE_SAMPLES / 8; s++) {
            pulse4Buf[s * 2 + i * PULSE_SAMPLES / 8] = f2i(SHRT_MAX * squareWaveNoDC(s, f2i(SAMPLE_RATE / mn2f(pulse4Notes[i % 3])), 0.5));
            pulse4Buf[s * 2 + 1 + i * PULSE_SAMPLES / 8] = pulse4Buf[s * 2 + i * PULSE_SAMPLES / 8];
        }
    }
    fadeBuffer(pulse4Buf, PULSE_SAMPLES, 0.5f, 0.f);

    return 1;
}

void demo_end() {
    free(kickBuf);
    free(snareBuf);
    free(openHatBuf);
    free(closedHatBuf);
    free(crashBuf);
    free(pulse1Buf);
    free(pulse2Buf);
    free(pulse3Buf);
    free(pulse4Buf);
    free(twisterVerts);
    free(riserBuf);
}

const unsigned short kickPattern = 0b1000001000100100;
const unsigned int hatPattern =     0b00000000010000000000000001000100;
const unsigned int hatRollPattern = 0b00000100000000000000001000000000;
const unsigned int cutoutPattern = 0b11111111000010101100111111000000;

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

    float* line = (float*)malloc(sizeof(float) * 6);
    if (line == 0) {
        memset(buffer, 0, DEMO_NUMSAMPLES * 2);
        return;
    }

    int counter = 0;
    double prog = 0;
    double targetProg = 0;
    int samples;
    float timer = 0;
    float yPos;
    float theta;
    float deflection;

    s = 0;

    for (; s + nSPS + BORDER_SAMPLES < TWISTER_SECONDS * SAMPLE_RATE;) {
        demo_rand(&seed);
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        yPos = twisterVerts[counter * 3 + 1];

        theta = max(timer - 8.f, 0.f)
            + (timer / 16.f) * (cosf(timer * 2.0f * M_PI) + 0.5f) * powf(M_E, -0.5 * powf(fmodf(timer, 8.f) - 4.f, 2.0f)) * powf(M_E, -4 * yPos * yPos)
            + -2 * sinf(M_PI * max(timer - 8.f, 0.f) * 0.125f) * (float(counter % TWISTER_SEGMENTS) / TWISTER_SEGMENTS) * (max(0.f, min((timer - 16) / 8, 2.f)) + 1);

        rotY(twisterVerts + counter * 3, line, theta);
        rotY(twisterVerts + counter * 3, line + 3, theta);
        line[4] += twisterHeight;

        if (line[2] > -TWISTER_WIDTH || (line[2] == -TWISTER_WIDTH && line[0] > 0)) { // line is visible
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
            prog = 0;
            targetProg = 0;
            p++;
            nextTime += SAMPLE_RATE * twisterNoteTimes[p % TWISTER_N_TIMES];
            freq = mn2f(twisterNotes[p % TWISTER_N_PITCHES]);
            nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
            nSPS = nSPC / nStrokes;
        }

        if (GetAsyncKeyState(VK_ESCAPE)) {
            free(line);
            return;
        }
    }
    wobbleBufferEnv(buffer + 16 * 2 * SAMPLE_RATE, SAMPLE_RATE * 32,   SAMPLE_RATE * 1.5, 0, 0.f,     0.f, 0.4f, 0.0f, 1.f );
    wobbleBufferEnv(buffer + 50 * 2 * SAMPLE_RATE, f2i(SAMPLE_RATE * 14), SAMPLE_RATE * 1.5, 0, 2.f / 3, 0.f, 0.4f, 0.0f, 0.5f);
    wobbleBufferEnv(buffer + 32 * 2 * SAMPLE_RATE, SAMPLE_RATE * 16,   SAMPLE_RATE,       0, 0.f,     0.f, 0.0f, 0.6f, 1.f );

    normalizeBuffer(buffer, DEMO_NUMSAMPLES, p0d45);
    free(line);

    twisterDrums(buffer);
}
void twisterDrums(short* buffer) {
    addSamples(buffer, crashBuf, CRASH_SAMPLES, FILE_RATE / CRASH_RATE);

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
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 48000, p0d10, p1d00);
        }
        else if ((i + 4) % 8 == 0 && i > 128) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 36000, p0d50, p1d00);
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

    addSamples(buffer + SAMPLE_RATE * 62 * 2, riserBuf, RISER_SAMPLES, SAMPLE_RATE / CRASH_RATE);
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
        faces = demo_rand(&seed) & 0b111111111111;
        for (int i = 0; i < 6; i++) {
            if ((faces & (1 << (i * 2))) && (faces & (1 << (i * 2 + 1)))) {
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
    double prog = 0;
    double targetProg = 0;
    int samples;
    float timer = 0;

    s = 0;

    float* currentStroke = (float*)malloc(sizeof(float)* (3 * 5));

    for (; s + nSPC + BORDER_SAMPLES < DEMO_NUMSAMPLES;) {
        demo_rand(&seed);
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        if ((s >= 16 * SAMPLE_RATE || ((faces & (1 << (counter * 2))) && (faces & (1 << (counter * 2 + 1))))) && (cubeNotes[p % CUBE_N_PITCHES] != NOFF)) {
            for (int v = 0; v < 5; v++) {
                currentStroke[v * 3 + 0] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 0];
                currentStroke[v * 3 + 1] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 1];
                currentStroke[v * 3 + 2] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 2];
                rotX(currentStroke + v * 3, currentStroke + v * 3, sin(M_PI * timer / 2) * M_PI);
                rotY(currentStroke + v * 3, currentStroke + v * 3, M_PI * timer);
                rotZ(currentStroke + v * 3, currentStroke + v * 3, M_PI * timer);
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
            prog = 0;
            targetProg = 0;

            if (s >= 16 * SAMPLE_RATE) {
                nStrokes = 6;
            }
            else {
                nStrokes = 0;
                while (nStrokes == 0) {
                    faces = demo_rand(&seed) & 0b111111111111;
                    for (int i = 0; i < 6; i++) {
                        if ((faces & (1 << (i * 2))) && (faces & (1 << (i * 2 + 1)))) {
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
            nextTime += SAMPLE_RATE * cubeNoteTimes[p % CUBE_N_TIMES];
        }

        if (GetAsyncKeyState(VK_ESCAPE)) {
            free(currentStroke);
            return;
        }
    }

    free(currentStroke);

    wobbleBufferEnv(buffer + 8 * SAMPLE_RATE * 2, SAMPLE_RATE * 56, SAMPLE_RATE / 1.5f, 0, 2.f / 3, 0.f, 0.12f, 0.0f, 0.5f);
    wobbleBufferEnv(buffer + 8 * SAMPLE_RATE * 2, SAMPLE_RATE * 56, SAMPLE_RATE / 2.0f, 0, 0.f, 2.f / 3, 0.0f, 0.12f, 0.5f);
    normalizeBuffer(buffer, DEMO_NUMSAMPLES, p0d45);

    cubeDrums(buffer);
}
void cubeDrums(short* buffer){
    addSamples(buffer, crashBuf, CRASH_SAMPLES, FILE_RATE / CRASH_RATE);
    fadeBuffer(buffer, 48000, p0d10, p1d00);
    addSamples(buffer, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
    int i;
    int j = 0;
    // arps
    for (i = 384; i < 512; i++) {
        if (i % 4 == 2) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, PULSE_SAMPLES, 0.5f, 1.f);
            switch (j) {
            case 0:
            case 1:
            case 4:
            case 5:
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulse1Buf, PULSE_SAMPLES, 1);
                break;
            case 2:
            case 6:
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulse2Buf, PULSE_SAMPLES, 1);
                break;
            case 3:
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulse3Buf, PULSE_SAMPLES, 1);
                break;
            case 7:
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulse4Buf, PULSE_SAMPLES, 1);
                break;
            }
        }
        if (i % 8 == 6) j = (j + 1) % 8;
    }

    // sidechain
    for (i = 192; i < 512; i++) {
        if (i % 4 == 0 && i >= 256) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 36000, p0d10, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        }
        if (i % 8 == 4) {
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, FILE_RATE / SNARE_RATE);
        }
    }

    // hihats
    for (i = 128; i < 512; i += 2) {
        if (i % 32 == 26) addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, openHatBuf, min(OPENHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
        else addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
    }
}

unsigned char plasmaFunction(short x, short y, float t)
{
    float xNorm = float(x) / (2.f * SHRT_MAX) - 1.f;
    float yNorm = float(y) / (1.f * SHRT_MAX);
    float yNorm2 = yNorm + 0.3;
    float f = 0.5 + 0.5 * cosf(2.f * t + (sinf(t) * sinf(t * 2.f) + 2.0f) * xNorm * 1.5f * M_PI)
        * (0.3 + cosf(t * 5.f + (cosf(yNorm * 3.f / SHRT_MAX + 1.f) + 2.0f) * yNorm * M_PI))
        + 0.4f * (0.3 + cos(sqrtf(xNorm * xNorm * xNorm * xNorm + yNorm2 * yNorm2 * yNorm2 * yNorm2) * 2.f * M_PI + (t * -5.5f)));
    return f2i(255 * min(1.f, max(0.f, f)));
}
void plasma(short* buffer) {
    int DEMO_NUMSAMPLES = f2i(demo_length(PLASMA_SECTION) * SAMPLE_RATE);
    int s;
    int nStrokes;
    int faces;
    float freq, nSPC, nSPS;

    nStrokes = 1;

    float startFreq = 23;
    float endFreq = 42;
    float dF = (endFreq - startFreq) / demo_length(PLASMA_SECTION);
    freq = mn2f(startFreq);
    nSPC = SAMPLE_RATE / freq;
    nSPS = nSPC / nStrokes - BORDER_SAMPLES;

    int counter = 0;
    double prog = 0;
    double targetProg = 0;
    int samples;
    float timer = 0;
    int stroke = 0;

    s = 0;

    for (; s + nSPS + BORDER_SAMPLES < DEMO_NUMSAMPLES;) {
        demo_rand(&seed);
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        if (!hilligoss(buffer + 2 * s, samples, demo_rand(&seed), timer, 1.f / SAMPLE_RATE, plasmaFunction, 1)) return;
        s += samples;
        strokeToCycle2D(border, 6, buffer + 2 * s, BORDER_SAMPLES);
        s += BORDER_SAMPLES;
        stroke++;
        timer += 1.f / freq;

        // check pitch and shit
        freq = mn2f(startFreq + dF * timer);
        nSPC = SAMPLE_RATE / freq;
        nSPS = nSPC / nStrokes - BORDER_SAMPLES;
    }

    normalizeBuffer(buffer, DEMO_NUMSAMPLES, p0d45);
    for (int i = 8; i < 24; i++) {
        fadeBuffer(buffer + ((SAMPLE_RATE >> 1) * i) * 2, 36000, p0d10, p1d00);
        addSamples(buffer + ((SAMPLE_RATE >> 1) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
    }
    addSamples(buffer + (SAMPLE_RATE >> 2) * 47 * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
    filterBuffer(buffer, DEMO_NUMSAMPLES / 2 , 0.55, 0, 1);
    filterBuffer(buffer + (SAMPLE_RATE >> 1) * 46, (SAMPLE_RATE >> 1), 0.45, 1, 0);

    fadeBuffer(buffer, 48000, p0d10, p1d00);
    addSamples(buffer, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
    addSamples(buffer + SAMPLE_RATE * 10 * 2, riserBuf, RISER_SAMPLES, SAMPLE_RATE / CRASH_RATE);
}

void finale(short* buffer) {
    int DEMO_NUMSAMPLES = f2i(demo_length(FINALE_SECTION) * SAMPLE_RATE);
    memset(buffer, 0, DEMO_NUMSAMPLES * 2 * sizeof(short));
    int s;
    bool object = 0;
    bool useHilli = 0;
    float nextTime = 0;
    int p = 0;
    float freq = 110;
    float nSPC, nSPS;

    int nStrokesTwister = TWISTER_SEGMENTS * 3;
    int nStrokesCube = 6;
    freq = mn2f(finaleNotes[0]);
    nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
    nSPS = nSPC / (object ? nStrokesTwister : nStrokesCube);
    nextTime += SAMPLE_RATE * 0.5 * finaleNoteTimes[p];

    int counter = 0;
    double prog = 0;
    double targetProg = 0;
    int samples;
    float timer = 0;
    double hilligossTimer = 0;
    float yPos, theta;

    float* currentStroke = (float*)malloc(sizeof(float) * (3 * 5));
    if (currentStroke == 0) return;

    float* line = (float*)malloc(sizeof(float) * 6);
    if (line == 0) {
        free(currentStroke);
        return;
    }

    s = 0;

    for (; s + nSPS + BORDER_SAMPLES < DEMO_NUMSAMPLES;) {
        demo_rand(&seed);
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        // render
        if (useHilli) {
            if (!hilligoss(buffer + 2 * s, samples, demo_rand(&seed), timer, 4.f / SAMPLE_RATE, plasmaFunction, 1)) {
                free(currentStroke);
                free(line);
                return;
            }
            s += samples;
            strokeToCycle2D(border, 6, buffer + 2 * s, BORDER_SAMPLES);
            s += BORDER_SAMPLES;
            hilligossTimer += 4.0 / freq;
        }
        else {
            // pitched shit
            if (object) {
                yPos = twisterVerts[counter * 3 + 1];

                theta = timer * 2
                    + ((timer + 32) / 16.f) * (cosf(timer * 2.0f * M_PI) + 0.5f) * powf(M_E, -0.5 * powf(fmodf(timer, 8.f) - 4.f, 2.0f)) * powf(M_E, -4 * yPos * yPos)
                    - ((timer - 8.f) / 3.f) * (float(counter % TWISTER_SEGMENTS) / TWISTER_SEGMENTS);

                rotY(twisterVerts + counter * 3, line, theta);
                rotY(twisterVerts + counter * 3, line + 3, theta);
                line[4] += twisterHeight;

                if (line[2] > -TWISTER_WIDTH || (line[2] == -TWISTER_WIDTH && line[0] > 0)) { // line is visible
                    lineToSamples(line, line + 3, buffer + 2 * s, samples);
                    s += samples;
                }
            }
            else {
                for (int v = 0; v < 5; v++) {
                    currentStroke[v * 3 + 0] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 0];
                    currentStroke[v * 3 + 1] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 1];
                    currentStroke[v * 3 + 2] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 2];
                    rotX(currentStroke + v * 3, currentStroke + v * 3, sin(M_PI * timer / 2) * M_PI);
                    rotY(currentStroke + v * 3, currentStroke + v * 3, M_PI * timer);
                    rotZ(currentStroke + v * 3, currentStroke + v * 3, M_PI * timer);
                }
                strokeToCycle3D(currentStroke, 5, buffer + 2 * s, samples, view_matrix);
                s += samples;
            }

            // border
            counter = (counter + 1) % (object ? (TWISTER_SEGMENTS * 4) : nStrokesCube);
            if (counter == 0) {
                prog -= nSPC;
                targetProg -= nSPC;
                timer += 1.0 / freq;
                strokeToCycle2D(border, 6, buffer + 2 * s, BORDER_SAMPLES);
                s += BORDER_SAMPLES;
            }

            if (GetAsyncKeyState(VK_ESCAPE)) {
                free(currentStroke);
                free(line);
                return;
            }
        }

        // pitch handling
        while (s > nextTime) {
            p++;
            prog = 0;
            targetProg = 0;
            counter = 0;

            if (finaleNotes[p % FINALE_N_PITCHES] != NOFF) {
                object = !object;
                useHilli = false;
            }
            else {
                useHilli = true;
            }

            // check pitch and shit
            if (finaleNotes[p % FINALE_N_PITCHES] == NOFF) freq = mn2f(30);
            else freq = mn2f(finaleNotes[p % FINALE_N_PITCHES]);
            nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
            nSPS = nSPC / (useHilli ? 1 : (object ? nStrokesTwister : nStrokesCube));
            nextTime += SAMPLE_RATE * 0.5f * finaleNoteTimes[p % FINALE_N_TIMES];
        }
    }

    normalizeBuffer(buffer, DEMO_NUMSAMPLES, p0d45);

    free(currentStroke);
    free(line);
}
void finaleDrums(short* buffer, bool pulses) {
    addSamples(buffer, crashBuf, CRASH_SAMPLES, FILE_RATE / CRASH_RATE);
    int i;
    if (pulses) {
        int j = 0;
        // arps
        for (i = 0; i < 256; i++) {
            if (i % 4 == 2) {
                fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, PULSE_SAMPLES, 0.5f, 1.f);
                switch (j) {
                case 0:
                case 1:
                case 4:
                case 5:
                    addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulse1Buf, PULSE_SAMPLES, 1);
                    break;
                case 2:
                case 6:
                    addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulse2Buf, PULSE_SAMPLES, 1);
                    break;
                case 3:
                    addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulse3Buf, PULSE_SAMPLES, 1);
                    break;
                case 7:
                    addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulse4Buf, PULSE_SAMPLES, 1);
                    break;
                }
            }
            if (i % 8 == 6) j = (j + 1) % 8;
        }
    }
    else {
        addSamples(buffer + SAMPLE_RATE * 30 * 2, riserBuf, RISER_SAMPLES, SAMPLE_RATE / CRASH_RATE);
    }

    // sidechain and drums
    for (i = 0; i < 256; i++) {
        if ((kickPattern & 1 << (15 - (i % 16)))) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 48000, p0d10, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        }
        else if (i % 8 == 4) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 36000, p0d50, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, FILE_RATE / SNARE_RATE);
        }
    }

    // hihats
    for (i = 0; i < 255; i++) {
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

void outro(short* buffer)
{
    finale(buffer);
    finaleDrums(buffer, true);
    fadeBuffer(buffer + OUTRO_SECONDS * SAMPLE_RATE, OUTRO_SECONDS * SAMPLE_RATE / 2, 1.f, 0.f);
    filterBuffer(buffer, OUTRO_SECONDS * SAMPLE_RATE / 2, p0d85, p0d70, p0d05);
    filterBuffer(buffer + OUTRO_SECONDS * SAMPLE_RATE, OUTRO_SECONDS * SAMPLE_RATE / 2, p0d85, p0d05, 0.001f);
    fadeBuffer(buffer + OUTRO_SECONDS * SAMPLE_RATE * 2 - 24000, 12000, 1.f, 0.f);
}

void intro(short* buffer) {
    int i;
    // sidechain
    for (i = 0; i < 64; i++) {
        if (i % 4 == 0) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 36000, p0d10, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        }
        if (i % 8 == 4 && i > 48) {
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, FILE_RATE / SNARE_RATE);
        }
    }

    // hihats
    for (i = 32; i < 64; i ++) {
        if (i % 32 == 26) addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, openHatBuf, min(OPENHAT_SAMPLES, SAMPLE_RATE >> 3), FILE_RATE / HAT_RATE);
        else addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
    }

    addSamples(buffer + SAMPLE_RATE * 6 * 2, riserBuf, RISER_SAMPLES, SAMPLE_RATE / CRASH_RATE);
}