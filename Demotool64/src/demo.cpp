//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#include "demo.h"

void twisterDrums(short* buffer);
#define TWISTER_SEGMENTS 192

#define OPENHAT_SAMPLES (6000)
#define CLOSEDHAT_SAMPLES (1024)
#define CRASH_SAMPLES 24000
#define RISER_SAMPLES 12000

short kickBuf[KICK_SAMPLES * 2];
short snareBuf[SNARE_SAMPLES * 2];
short openHatBuf[OPENHAT_SAMPLES * 2];
short closedHatBuf[CLOSEDHAT_SAMPLES * 2];
short crashBuf[CRASH_SAMPLES * 2];
short riserBuf[RISER_SAMPLES * 2];
float twisterVerts[3 * TWISTER_SEGMENTS * 4];

#define CRASH_RATE 12000
#define HAT_RATE (48000)

#define HAT_PERIOD0 (392 * 2)
#define HAT_PERIOD1 (314 * 2)
#define HAT_PERIOD2 (263 * 2)
#define HAT_PERIOD3 (231 * 2)
#define HAT_PERIOD4 (220 * 2)
#define HAT_PERIOD5 (155 * 2)

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

float twisterHeight;
float rotMatrix1[16];
static int seed = 0;

#define PULSE_SAMPLES SAMPLE_RATE / 4

int demo_init( int itime )
{
    twisterHeight = 2.f / TWISTER_SEGMENTS;
    for (int i = 0; i < TWISTER_SEGMENTS; i++) {
        float h = i * twisterHeight - 1.f;

        twisterVerts[3 * i] = TWISTER_WIDTH;
        twisterVerts[3 * i + 1] = h;
        twisterVerts[3 * i + 2] = -TWISTER_WIDTH;

        twisterVerts[3 * (TWISTER_SEGMENTS * 1 + i)] = TWISTER_WIDTH;
        twisterVerts[3 * (TWISTER_SEGMENTS * 1 + i) + 1] = h;
        twisterVerts[3 * (TWISTER_SEGMENTS * 1 + i) + 2] = TWISTER_WIDTH;

        twisterVerts[3 * (TWISTER_SEGMENTS * 2 + i)] = -TWISTER_WIDTH;
        twisterVerts[3 * (TWISTER_SEGMENTS * 2 + i) + 1] = h;
        twisterVerts[3 * (TWISTER_SEGMENTS * 2 + i) + 2] = TWISTER_WIDTH;

        twisterVerts[3 * (TWISTER_SEGMENTS * 3 + i)] = -TWISTER_WIDTH;
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
            kickBuf[i * 2] = f2i(SHRT_MAX * squareWaveNoDC(kSamp, f2i(kSPC), p0d60));
            kickBuf[i * 2 + 1] = kickBuf[i * 2];
            i++;
        }
        if (kFreq > KICK_FBASE) kFreq -= KICK_FSTEP;
        else kFreq = KICK_FBASE;
    }
    fadeBuffer(kickBuf, KICK_SAMPLES, KICK_START_V, KICK_END_V);

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
    fadeBuffer(snareBuf + SNARE_SAMPLES, SNARE_SAMPLES >> 1, p0d80, 0.f);

    // hihat
    float state1 = 0;
    float state2 = 0;
    //float state3 = 0;
    float retval = 0;
    for (int i = 0; i < OPENHAT_SAMPLES; i++) {
        retval = short(demo_rand(&seed));
        openHatBuf[i * 2] = f2i(retval);
        openHatBuf[i * 2 + 1] = openHatBuf[i * 2];
    }
    memcpy(closedHatBuf, openHatBuf, CLOSEDHAT_SAMPLES * 2 * sizeof(short));
    fadeBuffer(openHatBuf, OPENHAT_SAMPLES, 0.25f, p0d00);
    fadeBuffer(closedHatBuf, CLOSEDHAT_SAMPLES, 0.25f, p0d00);

    for (int i = 0; i < CRASH_SAMPLES; i++) {
        crashBuf[i * 2] = demo_rand(&seed);
        crashBuf[i * 2 + 1] = crashBuf[i * 2];
    }
    memcpy(riserBuf, crashBuf, RISER_SAMPLES * 2 * sizeof(short));
    fadeBuffer(crashBuf, CRASH_SAMPLES, p0d30, p0d00);
    fadeBuffer(riserBuf, RISER_SAMPLES, p0d00, p0d50);

    return 1;
}

void demo_end() {
}

const unsigned short kickPattern = 0b1000001000100100;
const unsigned int hatPattern =     0b00000000010000000000000001000100;
const unsigned int cutoutPattern = 0b11111111000010101100111111000000;

const char* text = "INERCIA 2025    INERCIA 2025    ";
void intro(short* buffer) {
    int DEMO_NUMSAMPLES = INTRO_SECONDS * SAMPLE_RATE;
    int s = 0;
    int p = 0;
    float freq;
    int nSPC;
    int samples;

    int nStrokes = 1;
    freq = mn2f(42);
    nSPC = f2i(SAMPLE_RATE / freq);

    for (s = 0; s + nSPC < DEMO_NUMSAMPLES;) {
        demo_rand(&seed);
        samples = nSPC;

        if (((p + 1) * SAMPLE_RATE >> 2) - s > (SAMPLE_RATE >> 3)) {
            drawChar(buffer + s * 2, samples, text[p % 32], -0.5, -0.5, 0.5, 0.5);
        }
        s += samples;
        
        if (s > (p + 1) * SAMPLE_RATE >> 2) {
            p++;
        }
    }

    normalizeBuffer(buffer, DEMO_NUMSAMPLES, p0d25);
    addSamples(buffer + SAMPLE_RATE * 14, riserBuf, RISER_SAMPLES >> 2, 64);
}

float line[6];

void twister(short* buffer)
{
    int DEMO_NUMSAMPLES = TWISTER_SECONDS * SAMPLE_RATE;
    int s;
    float nextTime = 0;
    int p = 0;
    float freq;
    float nSPC, nSPS;
    
    int nStrokes = TWISTER_SEGMENTS * 3;
    freq = mn2f(twisterNotes[0]);
    nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
    nSPS = nSPC / nStrokes;
    nextTime += SAMPLE_RATE * twisterNoteTimes[p];

    int counter = 0;
    double prog = 0;
    double targetProg = 0;
    int samples;
    float timer = 0;
    float yPos;
    float theta;

    s = 0;

    for (; s + nSPS + BORDER_SAMPLES < 64.5 * SAMPLE_RATE;) {
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        yPos = twisterVerts[counter * 3 + 1];

        theta = max(timer - 8.f, 0.f) * 1.25f
            + (max(0.0, timer - 16) / 8.f) * (cosf(timer * 2.0f * float(M_PI)) + 0.5f) * sinf((max(0.0f, timer - 8) / 2) / 4.f * float(M_PI)) * powf((float(M_E)), -4 * yPos * yPos)
            + (timer > 16 ? (max(0.0f,(timer - 8)/8) * (float(counter % TWISTER_SEGMENTS) / TWISTER_SEGMENTS) * (max(0.f, min((timer - 32) / 8, 2.f)) - 1)) : 0);

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
    }
    wobbleBufferEnv(buffer + 16 * 2 * SAMPLE_RATE, SAMPLE_RATE * 32,   SAMPLE_RATE * 1.5, 0, 0.f,     0.f, 0.4f, 0.0f, 1.f );
    wobbleBufferEnv(buffer + 50 * 2 * SAMPLE_RATE, SAMPLE_RATE * 14, SAMPLE_RATE * 1.5, 0, 2.f / 3, 0.f, 0.4f, 0.0f, 0.5f);
    wobbleBufferEnv(buffer + 32 * 2 * SAMPLE_RATE, SAMPLE_RATE * 16,   SAMPLE_RATE,       0, 0.f,     0.f, 0.0f, 0.6f, 1.f );

    normalizeBuffer(buffer, DEMO_NUMSAMPLES, p0d45);

    twisterDrums(buffer);
}

inline void twisterDrums(short* buffer)
{
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
    for (i = 0; i <= 512; i++) {
        if ((i <= 384 || i >= 400) && (kickPattern & 1 << (15 - (i % 16)))) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 48000, p0d10, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, 2);
        }
        if ((i + 4) % 8 == 0 && i > 128) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 36000, p0d50, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, 2);
        }
    }

    // hihats
    for (i = 256; i < 512; i++) {
        if (i < 384 || i >= 400) {
            /*if (hatRollPattern & (1 << (31 - (i % 32)))) {
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 4), FILE_RATE / HAT_RATE);
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i + (SAMPLE_RATE >> 4)) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 5), FILE_RATE / HAT_RATE);
            }
            else*/ if (hatPattern & (1 << (31 - (i % 32)))) {
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, openHatBuf, min(OPENHAT_SAMPLES, SAMPLE_RATE >> 3), 6);
            }
            else {
                addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 3), 6);
            }
        }
    }
}