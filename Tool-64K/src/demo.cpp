//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#include "demo.h"
#include "revision-quanitzed.h"

bool intro(short* buffer);

bool textSection(short* buffer);
bool textDrums(short* buffer);

bool cube(short* buffer);
bool cubeDrums(short* buffer);
bool plasma(short* buffer, bool octave);
bool ball(short* buffer);
bool ballDrums(short* buffer);
bool greetz(short* buffer);
//bool finale(short* buffer);
//void finaleDrums(short* buffer, bool pulses);
bool midSection(short* buffer);
//void pulse(short* buffer, int n);

short* kickBuf;
short* snareBuf;
short* openHatBuf;
short* closedHatBuf;
short* crashBuf;
short* riserBuf;
/*
short* pulse1Buf;
short* pulse2Buf;
short* pulse3Buf;
short* pulse4Buf;
*/
short* pulseSingleBuf;
short* pulseSingleBuf2;
float* ballOutlineVerts;
float* revisionVerts3D;

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

bool demo_do(long itime, short* buffer, int section)
{
    bool b = true;
    int DEMO_NUMSAMPLESC = f2i(demo_length(section) * SAMPLE_RATE * 2);
    switch (section) {
    case INTRO_SECTION:
        return intro(buffer);
    case BALL_SECTION:
        return ball(buffer);
    case CUBE_SECTION:
        return cube(buffer);
    case PLASMA_SECTION:
        return plasma(buffer, 0);
    case GREETZ_SECTION:
        return greetz(buffer);
    //case FINALE_SECTION:
    //    b = finale(buffer);
    //    finaleDrums(buffer, false);
    //    return b;
    case MID_SECTION:
        return midSection(buffer);
    case PLASMA2_SECTION:
        return plasma(buffer, 1);
    case TEXT_SECTION:
        return textSection(buffer);
    default:
        memset(buffer, 0, DEMO_NUMSAMPLESC * sizeof(short));
    }
}

float demo_length(int section)
{
    switch (section) {
    case INTRO_SECTION:
        return INTRO_SECONDS;
    case BALL_SECTION:
        return BALL_SECONDS;
    case CUBE_SECTION:
        return CUBE_SECONDS;
    case PLASMA_SECTION:
        return PLASMA_SECONDS;
    //case FINALE_SECTION:
    //    return FINALE_SECONDS;
    case MID_SECTION:
        return MID_SECONDS;
    case TEXT_SECTION:
        return TEXT_SECONDS;
    case GREETZ_SECTION:
        return GREETZ_SECONDS;
    case PLASMA2_SECTION:
        return PLASMA2_SECONDS;
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

#define TEXT_N_PITCHES 64
#define TEXT_N_TIMES 1
static const float textNoteTimes[TEXT_N_TIMES] = {
    0.125
};
static const char textNotes[TEXT_N_PITCHES] = {
    64,
    59,
    64,
    66,
    67,
    59,
    64,
    66,
    69,
    59,
    67,
    66,
    59,
    64,
    66,
    67,
    64,
    59,
    64,
    66,
    67,
    59,
    67,
    69,
    71,
    59,
    69,
    67,
    59,
    66,
    67,
    66,
    64,
    60,
    64,
    66,
    67,
    60,
    66,
    67,
    69,
    60,
    67,
    66,
    60,
    64,
    66,
    67,
    66,
    59,
    63,
    64,
    66,
    59,
    66,
    67,
    69,
    59,
    71,
    66,
    59,
    64,
    66,
    67,
};
static const char textNotes2[4] = {
    55,
    54,
    52,
    51
};
static const char textNotes3[4] = {
    28,
    38,
    36,
    35,
};

#define BALL_OUTLINE_SEGMENTS 32
#define BALL_OUTLINE_VERTS (BALL_OUTLINE_SEGMENTS + 1)
#define BALL_OUTLINE_SEGMENT_ANGLE (2 * M_PI_F / (BALL_OUTLINE_SEGMENTS - 1))

#define BALL_ANGLE -p0d42
#define BALL_ANGLE_DELTA p0d05
#define BALL_X_SEGS 4
#define BALL_Y_SEGS 3

#define BALL_RADIUS_START p0d25
#define BALL_RADIUS_END p0d40

#define BALL_SPEED_X (-860.f / SAMPLE_RATE)
#define BALL_ACC (64.f / SAMPLE_RATE)

#define BALL_N_PITCHES 64
#define BALL_N_TIMES 1
static const float ballNoteTimes[BALL_N_TIMES] = {
    0.125
};

static const char ballNotes[BALL_N_PITCHES] = {
    38,
    50,
    50,
    50,
    50,
    50,
    50,
    36,
    38,
    50,
    50,
    50,
    50,
    50,
    50,
    38,
    39,
    51,
    51,
    51,
    51,
    51,
    51,
    38,
    39,
    51,
    51,
    51,
    51,
    51,
    51,
    39,

    38,
    50,
    50,
    50,
    50,
    50,
    50,
    36,
    38,
    50,
    50,
    50,
    50,
    50,
    50,
    38,
    39,
    51,
    51,
    51,
    51,
    51,
    51,
    51,
    53,
    53,
    53,
    53,
    54,
    54,
    54,
    54,
};



static const unsigned short ball2Fades = 0b0111111001111110;

#define CUBE_N_PITCHES 16
#define CUBE_N_TIMES 1
static const float cubeNoteTimes[CUBE_N_PITCHES] = {
    0.125
};

static const char cubeNotes[CUBE_N_PITCHES] = {
    38,
    50,
    38,
    50,
    51,
    39,
    51,
    39,
    51,
    50,
    38,
    50,
    51,
    39,
    51,
    39,
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
    38,
    NOFF,
    40,
    41,
    NOFF,
    45,
    50,
    NOFF,
    44,
    43,
    NOFF,
    40
};

/*
static const char pulse1Notes[3] = {
    74,
    77,
    81
};

static const char pulse2Notes[3] = {
    74,
    77,
    82
};

static const char pulse3Notes[3] = {
    76,
    79,
    82
};

static const char pulse4Notes[3] = {
    76,
    79,
    84
};*/

float twisterHeight;
float rotMatrix1[16];
static int seed = 0;

#define PULSE_SAMPLES SAMPLE_RATE / 4

int demo_init( int itime )
{
    kickBuf = (short*)malloc(sizeof(short) * KICK_SAMPLES * 2);
    if (kickBuf == 0) return 0;

    snareBuf = (short*)malloc(sizeof(short) * SNARE_SAMPLES * 2);
    if (snareBuf == 0) goto RETA;

    openHatBuf = (short*)malloc(sizeof(short) * OPENHAT_SAMPLES * 2);
    if (openHatBuf == 0) goto RETB;

    closedHatBuf = (short*)malloc(sizeof(short) * CLOSEDHAT_SAMPLES * 2);
    if (closedHatBuf == 0) goto RETC;

    crashBuf = (short*)malloc(sizeof(short) * CRASH_SAMPLES * 2);
    if (crashBuf == 0) goto RETD;
    /*
    pulse1Buf = (short*)malloc(sizeof(short) * PULSE_SAMPLES * 2);
    if (pulse1Buf == 0) goto RETE;

    pulse2Buf = (short*)malloc(sizeof(short) * PULSE_SAMPLES * 2);
    if (closedHatBuf == 0) goto RETF;

    pulse3Buf = (short*)malloc(sizeof(short) * PULSE_SAMPLES * 2);
    if (closedHatBuf == 0) goto RETG;

    pulse4Buf = (short*)malloc(sizeof(short) * PULSE_SAMPLES * 2);
    if (closedHatBuf == 0) goto RETH;
    */
    ballOutlineVerts = (float*)malloc(sizeof(float) * 3 * BALL_OUTLINE_VERTS);
    if (ballOutlineVerts == 0) goto RETI;

    riserBuf = (short*)malloc(sizeof(short) * RISER_SAMPLES * 2);
    if (riserBuf == 0) goto RETJ;

    pulseSingleBuf = (short*)malloc(sizeof(short) * PULSE_SAMPLES);
    if (pulseSingleBuf == 0) goto RETK;

    pulseSingleBuf2 = (short*)malloc(sizeof(short) * PULSE_SAMPLES / 2);
    if (pulseSingleBuf2 == 0) goto RETL;

    revisionVerts3D = (float*)malloc(sizeof(float) * 3 * TOTAL_REVISION_VERTS);
    if (revisionVerts3D == 0) {
        free(pulseSingleBuf2);
    RETL:
        free(pulseSingleBuf);
    RETK:
        free(riserBuf);
    RETJ:
        free(ballOutlineVerts);
    RETI:
        /*
        free(pulse4Buf);
        RETH:
        free(pulse3Buf);
        RETG:
        free(pulse2Buf);
        RETF:
        free(pulse1Buf);
        RETE:*/
        free(crashBuf);
    RETD:
        free(closedHatBuf);
    RETC:
        free(openHatBuf);
    RETB:
        free(snareBuf);
    RETA:
        free(kickBuf);
        return 0;
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
    scaleBuffer(kickBuf, KICK_SAMPLES, 1.5f);
    scaleBuffer(kickBuf, KICK_FADE_START, KICK_START_V);
    fadeBuffer(kickBuf + (KICK_FADE_START * 2), KICK_FADE_MID - KICK_FADE_START, KICK_START_V, KICK_MID_V);
    fadeBuffer(kickBuf + (KICK_FADE_MID * 2), KICK_FADE_END - KICK_FADE_MID, KICK_MID_V, KICK_END_V);
    scaleBuffer(kickBuf + (KICK_FADE_END * 2), KICK_SAMPLES - KICK_FADE_END, KICK_END_V);

    // snare
    float sFreq = SNARE_FMAX;
    float sSPC;
    int sSamp;
    float n;
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
    scaleBuffer(snareBuf, SNARE_SAMPLES, p0d80);
    fadeBuffer(snareBuf + SNARE_SAMPLES, SNARE_SAMPLES / 2, 1.f, 0.f);

    // hihat
    float state1 = 0;
    float state2 = 0;
    //float state3 = 0;
    float retval;
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
    fadeBuffer(openHatBuf, OPENHAT_SAMPLES, p0d65, p0d00);
    fadeBuffer(closedHatBuf, CLOSEDHAT_SAMPLES, p0d65, p0d00);

    short x, y;
    float d;
    for (int i = 0; i < CRASH_SAMPLES; i++) {
        do {
            x = demo_rand(&seed);
            y = demo_rand(&seed);
            d = sqrtf(int(x) * x + int(y) * y);
        } while (d > SHRT_MAX);
        crashBuf[i * 2] = x; 
        crashBuf[i * 2 + 1] = y;

        riserBuf[i * 2] = demo_rand(&seed);
        riserBuf[i * 2 + 1] = demo_rand(&seed);
    }
    fadeBuffer(crashBuf, CRASH_SAMPLES, p0d30, p0d00);
    fadeBuffer(riserBuf, RISER_SAMPLES, p0d00, p0d50);
    filterBuffer(riserBuf, RISER_SAMPLES, p0d85, 0, p0d65);

    memset(pulseSingleBuf, 0, PULSE_SAMPLES * sizeof(short));
    {
        for (int s = 0; s < PULSE_SAMPLES / 2; s++) {
            pulseSingleBuf[s * 2] = f2i(SHRT_MAX * squareWaveZeroed(s, f2i(SAMPLE_RATE / mn2f(55)), 0.85));;
            pulseSingleBuf[s * 2 + 1] = 0;
        }
    }
    fadeBuffer(pulseSingleBuf, PULSE_SAMPLES / 2, 0.6f, 0.f);

    memset(pulseSingleBuf2, 0, PULSE_SAMPLES / 2 * sizeof(short));
    {
        for (int s = 0; s < PULSE_SAMPLES / 4; s++) {
            pulseSingleBuf2[s * 2] = 0;
            pulseSingleBuf2[s * 2 + 1] = f2i(SHRT_MAX * squareWaveZeroed(s, f2i(SAMPLE_RATE / mn2f(55)), 0.85));
        }
    }
    fadeBuffer(pulseSingleBuf2, PULSE_SAMPLES / 4, 0.6f, 0.f);

    int index = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < revisionLengths[i]; j++) {
            revisionVerts3D[index * 3] = revisionVerts[i][j * 2] / 96.f - 0.63f;
            revisionVerts3D[index * 3 + 1] = revisionVerts[i][j * 2 + 1] / 96.f - 0.63f;
            revisionVerts3D[index * 3 + 2] = 0;
            index++;
        }
        revisionOffsets[i + 1] = index;
    };

    return 1;
}

void demo_end() {
    free(kickBuf);
    free(snareBuf);
    free(openHatBuf);
    free(closedHatBuf);
    free(crashBuf);
    /*
    free(pulse1Buf);
    free(pulse2Buf);
    free(pulse3Buf);
    free(pulse4Buf);
    */
    free(ballOutlineVerts);
    free(pulseSingleBuf);
    free(pulseSingleBuf2);
    free(riserBuf);
    free(revisionVerts3D);
}

const unsigned short kickPattern =  0b1000001000100100;
const unsigned int hatPattern =     0b00000000010000000000000001000100;
const unsigned int hatRollPattern = 0b00000100000000000000001000000000;

const char sayIt[] = "SAY IT";
bool ball(short* buffer) {
    int DEMO_NUMSAMPLES = f2i(demo_length(BALL_SECTION) * SAMPLE_RATE);
    int s;
    float nextTime = 0;
    int lastTime = 0;
    int p = 0;
    float freq = 110;
    float nSPC, nSPS;

    const int nStrokes = 16;
    freq = mn2f(cubeNotes[0]); // TODO - yes I intended to have the first note be an octave up
    nSPC = SAMPLE_RATE / freq;
    nSPS = nSPC / nStrokes;
    nextTime += SAMPLE_RATE * cubeNoteTimes[p];

    float* line = (float*)malloc(sizeof(float) * 6);
    if (line == 0) {
        memset(buffer, 0, DEMO_NUMSAMPLES * 2);
        return false;
    }
    line[2] = 0;
    line[5] = 0;

    int counter = 0;
    double prog = 0;
    double targetProg = 0;
    int samples;
    int samplesCorrected;
    float timer = 0;
    float scroller = 0;

    float ballPosX = -p0d40;
    float ballPosY = p0d50;
    float ballVelY = 0;
    float ballVelX = 1;

    float ballRadius = BALL_RADIUS_START;
    int sFac;
    s = 0;

    int pos = 0;

    int now;
    float nowX;
    float nowY;

    for (; s + nSPC < BALL_SECONDS * SAMPLE_RATE;) {
        demo_rand(&seed);
        if (counter == 2) {
            targetProg += 6 * nSPS;
            samples = f2i(targetProg - prog);
            prog += samples;
            if (s < 32 * SAMPLE_RATE) {
                now = min(4, max(0, s - 6 * SAMPLE_RATE) * 2 / SAMPLE_RATE);
                gridBuffer(buffer + 2 * s, samples, 1, now);
                now = max(0, s - 16 * SAMPLE_RATE);
                nowX = min(now / 8.f / SAMPLE_RATE, 1.f);
                wobbleBufferEnv(buffer + 2 * s, samples, SAMPLE_RATE, now % SAMPLE_RATE, 1.f, 1.f, 0.1f * nowX, 0.1f * nowX, 0.5f);
                punchBuffer(buffer + 2 * s, samples, f2i(ballPosX * SHRT_MAX), f2i(ballPosY * SHRT_MAX), ballRadius);
            }
            else if (pos < 0.078125f * SAMPLE_RATE || !(ball2Fades & 1 << (15 - (p % 16))) || s >= 48 * SAMPLE_RATE) {
                nowX = max(0.0f, float(s) / SAMPLE_RATE - 40.f);
                nowY = max(0.0f, float(s) / SAMPLE_RATE - 40.f - M_PI_F / 6.f);
                for (int i = 0; i < 3; i++) {
                    line[0] = sinf(M_PI_F * 2 / 3 * i + nowX);
                    line[3] = line[0];
                    line[1] = -1.f;
                    line[4] = 1.f;
                    lineToSamples(line, line + 3, buffer + (s + samples * i / 6) * 2, samples / 6);
                }

                for (int i = 3; i < 6; i++) {
                    line[1] = sinf(M_PI_F * 2 / 3 * i + nowY);
                    line[4] = line[1];
                    line[0] = -1.f;
                    line[3] = 1.f;
                    lineToSamples(line, line + 3, buffer + (s + samples * i / 6) * 2, i == 5 ? samples - (samples / 6 * 5) : samples / 6);
                }
                if (s >= 48 * SAMPLE_RATE) wobbleBufferEnv(buffer + 2 * s, samples, SAMPLE_RATE / 2.f, s % SAMPLE_RATE, 1.f, 1.f, 0.1f, 0.1f, 0.5f);
                punchBuffer(buffer + 2 * s, samples, f2i(ballPosX * SHRT_MAX), f2i(ballPosY * SHRT_MAX), ballRadius);
            }
            counter = (counter + 6) % nStrokes;
            s += samples;
            pos += samples;
        }
        else if (counter == 0) {
            targetProg += 2 * nSPS;
            samples = f2i(targetProg - prog);
            prog += samples;
            ballOutlineVerts[0] = -ballRadius;
            ballOutlineVerts[1] = 0.f;
            ballOutlineVerts[2] = 0.f;
            for (int i = 1; i < BALL_OUTLINE_VERTS; i++) {
                rotZ(ballOutlineVerts, ballOutlineVerts + 3 * i, i * BALL_OUTLINE_SEGMENT_ANGLE);
            }
            strokeToCycle2D(ballOutlineVerts, BALL_OUTLINE_VERTS, buffer + 2 * s, samples);
            offsetBuffer(buffer + 2 * s, samples, f2i(ballPosX * SHRT_MAX), f2i(ballPosY * SHRT_MAX));
            counter = (counter + 2) % nStrokes;
            s += samples;
            pos += samples;
        }
        else if (s >= SAMPLE_RATE * 8 && counter < 8 + BALL_Y_SEGS) {
            targetProg += nSPS;
            samples = f2i(targetProg - prog);
            prog += samples;
            if (s < 32 * SAMPLE_RATE || (s % (SAMPLE_RATE * 2) < SAMPLE_RATE)) {
                line[0] = -ballRadius;
                line[1] = -ballRadius + (((counter - 8) + 0.5) * ballRadius * 2 / (BALL_Y_SEGS));
                line[3] = ballRadius;
                line[4] = line[1];
                strokeToCycle2D(line, 2, buffer + 2 * s, samples);
                addShade(buffer + 2 * s, samples, 3, 0, ballRadius / 2 / BALL_Y_SEGS);
                offsetBufferCheckered(buffer + 2 * s, samples, 0, f2i(SHRT_MAX * ballRadius / BALL_Y_SEGS), samples / BALL_X_SEGS, f2i(fmodf(scroller, 1.f) * samples / BALL_X_SEGS), 0.5f);
                ballify(buffer + 2 * s, samples, ballRadius, true);
                rotateBuffer(buffer + 2 * s, samples, BALL_ANGLE);
                offsetBuffer(buffer + 2 * s, samples, f2i(ballPosX * SHRT_MAX), f2i(ballPosY * SHRT_MAX));
            }
            else {
                strokeToCycle2D(revisionVerts3D + 3 * revisionOffsets[(counter - 8) * 2], revisionLengths[(counter - 8) * 2], buffer + 2 * s, samples);
                strokeToCycle2D(revisionVerts3D + 3 * revisionOffsets[(counter - 8) * 2 + 1], revisionLengths[(counter - 8) * 2 + 1], buffer + 2 * (s + samples), samples);
                scaleBuffer(buffer + 2 * s, samples * 2, ballRadius * 1.3f);
                rotateBuffer(buffer + 2 * s, samples * 2, scroller * M_PI_F / 2.f);
                offsetBuffer(buffer + 2 * s, samples * 2, f2i(ballPosX * SHRT_MAX), f2i(ballPosY * SHRT_MAX));
                s += samples;
                pos += samples;
                prog += samples;
                targetProg += nSPS;
            }
            s += samples;
            counter = (counter + 1) % nStrokes;
            pos += samples;
        }
        else {
            sFac = s >= SAMPLE_RATE * 8 ? 8 - BALL_Y_SEGS : 8;
            targetProg += nSPS * ((s >= SAMPLE_RATE * 32) && (s % (SAMPLE_RATE * 2) >= SAMPLE_RATE) ? sFac - 3 : sFac);
            samples = f2i(targetProg - prog);
            prog += samples;
            samplesCorrected = f2i(samples * (s >= 12 * SAMPLE_RATE && s < 16 * SAMPLE_RATE) / 18.f);
            for (int i = 0; i < 6 && samplesCorrected > 0; i++) {
                if (sayIt[i] != ' ') {
                    drawChar(buffer + 2 * s, samplesCorrected, sayIt[i], (i - 3) * 0.3f, -0.125f + sinf(i * 0.6f + 4.f * s / SAMPLE_RATE) / 15, (i - 3) * 0.3f + 0.25f, 0.125f + sinf(i * 0.6f + 4.f * s / SAMPLE_RATE) / 15);
                    s += samplesCorrected;
                }
            }
            if (s < 32 * SAMPLE_RATE || pos < 0.078125f * SAMPLE_RATE || !(ball2Fades & 1 << (15 - (p % 16))) || s >= 48 * SAMPLE_RATE) {
                strokeToCycle2D(border, 6, buffer + 2 * s, samples - 5 * samplesCorrected);
            }
            counter = (counter + sFac) % nStrokes;
            s += samples - 5 * samplesCorrected;
            pos += samples;
        }

        if (counter == 0) {
            float xAcc = -1.f;
            if (s < SAMPLE_RATE * 32) {
                ballRadius = BALL_RADIUS_START + ((max(0, (float(s) / SAMPLE_RATE) - 8) / (BALL_SECONDS - 8.f)) * (BALL_RADIUS_END - BALL_RADIUS_START));
                xAcc = -1.1f;
            }
            timer += 1.f / freq;
            prog -= nSPC;
            targetProg -= nSPC;
            ballPosX += ballVelX * BALL_SPEED_X;
            ballPosY += ballVelY;
            if (ballPosX > 1.f - ballRadius) {
                ballPosX -= (ballPosX + ballRadius - 1.f);
                ballVelX = xAcc * ballVelX;
            }
            if (ballPosX < -1.f + ballRadius) {
                ballPosX -= (ballPosX - ballRadius + 1.f);
                ballVelX = xAcc * ballVelX;
            }
            if (ballPosY < -1.f + ballRadius) {
                ballPosY -= (ballPosY - ballRadius + 1.f);
                ballVelY = -ballVelY;
            }
            ballVelY -= BALL_ACC;
            scroller = fmodf((scroller + (ballVelX * BALL_ANGLE_DELTA / fabsf(ballVelX))) + 4.f, 4.f);
        }

        while (s > nextTime) {
            pos = 0;
            prog = 0;
            targetProg = 0;
            p++;
            if (s < 32 * SAMPLE_RATE) {
                nextTime += SAMPLE_RATE * cubeNoteTimes[p % CUBE_N_TIMES];
                freq = mn2f(cubeNotes[p % CUBE_N_PITCHES] - 12);
            } else
            {
                nextTime += SAMPLE_RATE * ballNoteTimes[p % BALL_N_TIMES];
                freq = mn2f(ballNotes[p % BALL_N_PITCHES] - 12);
            }
            nSPC = SAMPLE_RATE / freq;// -BORDER_SAMPLES;
            nSPS = nSPC / nStrokes;
            lastTime = s;
        }

        if (GetAsyncKeyState(VK_ESCAPE)) {
            free(line);
            return false;
        }
    }

    scaleBuffer(buffer, DEMO_NUMSAMPLES, p0d45);
    free(line);

    return ballDrums(buffer);
}
bool ballDrums(short* buffer) {
    addSamples(buffer, crashBuf, CRASH_SAMPLES, FILE_RATE / CRASH_RATE);
    addSamples(buffer + SAMPLE_RATE * 30 * 2, riserBuf, RISER_SAMPLES, SAMPLE_RATE / CRASH_RATE);
    addSamples(buffer + SAMPLE_RATE * 32 * 2, crashBuf, CRASH_SAMPLES, FILE_RATE / CRASH_RATE);
    //return 1;

    int i;
    bool didntKick;
#define KICKS4F 0b1000
#define KICKSPAT 0b0100
#define SNARES 0b0010
#define HATS 0b0001
    // 1000 kicks(4-on-the-floor)
    // 0100 kicks (pattern)
    // 0010 snares
    // 0001 hats
    short parts = 0;
    
    

    fadeBuffer(buffer, 48000, p0d10, p1d00);
    addSamples(buffer, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
    for (i = 0; i < 512; i++) {
        if (i == 48 || i == 65) parts ^= KICKS4F;
        if (i == 125) parts ^= KICKSPAT + SNARES;
        if (i == 192) parts ^= HATS;
        if (i == 256) parts = 0;
        if (i == 316) parts ^= SNARES;
        if (i == 320) parts ^= KICKSPAT + HATS;
        if (i == 480) parts ^= KICKS4F + KICKSPAT + SNARES;
        didntKick = true;
        if (((i % 4 == 0) && (parts & KICKS4F)) || ((kickPattern & 1 << (15 - (i % 16))) && (parts & KICKSPAT))) {
            didntKick = false;
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 48000, p0d10, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        }
        if (i % 8 == 4 && (parts & SNARES)) {
            if (didntKick) fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 36000, p0d50, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, FILE_RATE / SNARE_RATE);
        }
        if ((parts & HATS)) {
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
        if (i % 10 == 0 && GetAsyncKeyState(VK_ESCAPE)) {
            return false;
        }
    }
    return true;
}

bool cube(short* buffer) {
    int DEMO_NUMSAMPLES = f2i(demo_length(CUBE_SECTION) * SAMPLE_RATE);
    int s;
    float nextTime = 0;
    int p = 0;
    int nStrokes;
    int faces;
    int curFace = 0;
    float freq, nSPC, nSPS;

    nStrokes = 0;
    faces = curFace;
    while (faces == curFace) {
        demo_rand(&seed);
        demo_rand(&seed);
        demo_rand(&seed);
        faces = (demo_rand(&seed) & 0b1111) % 6;
    }
    curFace = faces;
    faces = 0b111111111111 - (0b11 << (faces * 2));
    nStrokes = 1;

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
    if (currentStroke == 0) {
        return false;
    }
    
    for (; s + nSPC + BORDER_SAMPLES < DEMO_NUMSAMPLES;) {
        demo_rand(&seed);
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;
        int mask = (0b11 << (counter * 2));
        if ((nStrokes == 6 || ((faces & mask) == false)) && (cubeNotes[p % CUBE_N_PITCHES] != NOFF)) {
            for (int v = 0; v < 5; v++) {
                currentStroke[v * 3 + 0] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 0];
                currentStroke[v * 3 + 1] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 1];
                currentStroke[v * 3 + 2] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 2];
                rotX(currentStroke + v * 3, currentStroke + v * 3, sinf(M_PI_F * timer / 2) * M_PI_F);
                rotY(currentStroke + v * 3, currentStroke + v * 3, M_PI_F * timer);
                rotZ(currentStroke + v * 3, currentStroke + v * 3, M_PI_F * timer);
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
            else if (s >= 8 * SAMPLE_RATE) {
                nStrokes = 0;
                while (nStrokes == 0) {
                    faces = demo_rand(&seed) & 0b111111111111;
                    for (int i = 0; i < 6; i++) {
                        if ((faces & (3 << (i * 2))) == false) {
                            nStrokes++;
                        }
                    }
                }
            }
            else if (p % 4 == 0) {
                faces = curFace;
                while (faces == curFace) {
                    demo_rand(&seed);
                    demo_rand(&seed);
                    demo_rand(&seed);
                    faces = (demo_rand(&seed) & 0b1111) % 6;
                }
                curFace = faces;
                faces = 0b111111111111 - (0b11 << (faces * 2));
                nStrokes = 1;
            }

            // check pitch and shit
            freq = mn2f(cubeNotes[p % CUBE_N_PITCHES] - 12);
            if (cubeNotes[p % CUBE_N_PITCHES] == NOFF) freq = 110;
            nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
            nSPS = nSPC / nStrokes;
            nextTime += SAMPLE_RATE * cubeNoteTimes[p % CUBE_N_TIMES];
        }

        if (GetAsyncKeyState(VK_ESCAPE)) {
            free(currentStroke);
            return false;
        }
    }

    free(currentStroke);

    wobbleBufferEnv(buffer + 24 * SAMPLE_RATE * 2, SAMPLE_RATE * 8, SAMPLE_RATE / 1.5f, 0, 2.f / 3, 0.f, 0.12f, 0.0f, 0.5f);
    wobbleBufferEnv(buffer + 24 * SAMPLE_RATE * 2, SAMPLE_RATE * 8, SAMPLE_RATE / 2.0f, 0, 0.f, 2.f / 3, 0.0f, 0.12f, 0.5f);
    scaleBuffer(buffer, DEMO_NUMSAMPLES, p0d45);

    if (GetAsyncKeyState(VK_ESCAPE)) {
        return false;
    }
    
    return cubeDrums(buffer);
}
bool cubeDrums(short* buffer){
    addSamples(buffer, crashBuf, CRASH_SAMPLES, FILE_RATE / CRASH_RATE);
    fadeBuffer(buffer, 48000, p0d10, p1d00);
    addSamples(buffer, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);

    int i;

    // sidechain
    for (i = 128; i < 256; i++) {
        if (i % 4 == 0) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 36000, p0d10, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        }
        if (i >= 192 && i % 8 == 4) {
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, FILE_RATE / SNARE_RATE);
        }
        if (i > 64) {
            if (i % 16 == 10) addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, openHatBuf, min(OPENHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
            else addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
        }
    }

    return true;
}

unsigned char plasmaFunction(short x, short y, float t)
{
    float xN = 1.5 * float(x) / (SHRT_MAX);
    float yN = 1.5 * float(y) / (SHRT_MAX);
    float x2 = xN + 2.f;
    float y2 = yN + 2.f;

    float f = cos(1.2f * sin(t - 3.f * sqrtf(x2 * x2 + y2 * y2))) * (
        sin(xN * 4.0f + t)
        - 1.3f * cos(yN - 2.0f * t)
        + sin(t) * sin(xN * 1.5334f + 1.232f * sin(t) + t) * sin(yN * 3.0f + xN * cos(t)));
    return f2i(255 * min(1.f, max(0.f, 1.f - pow(fabsf(f), 1.0f))));
}
bool plasma(short* buffer, bool octave) {
    int DEMO_NUMSAMPLES = f2i((demo_length(octave ? PLASMA2_SECTION : PLASMA_SECTION) - 0.25f * octave) * SAMPLE_RATE);
    int s;
    int nStrokes;
    int faces;
    float freq, nSPC, nSPS;

    nStrokes = 1;

    float startFreq = octave ? 26 : 8;
    float endFreq = 26;
    float dF = (endFreq - startFreq) / DEMO_NUMSAMPLES * SAMPLE_RATE;
    freq = mn2f(startFreq) * (1 + octave);
    nSPC = SAMPLE_RATE / freq;
    nSPS = nSPC / nStrokes - BORDER_SAMPLES;

    int counter = 0;
    double prog = 0;
    double targetProg = 0;
    int samples;
    float timer = 0;
    float hilliTimer = PLASMA_SECONDS * octave;

    s = 0;

    for (; s + nSPS + BORDER_SAMPLES < DEMO_NUMSAMPLES;) {
        demo_rand(&seed);
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        short* marker = buffer + 2 * s;

        if (!hilligoss(marker, samples / 2, demo_rand(&seed), hilliTimer, 1.f / SAMPLE_RATE, plasmaFunction, 1)) {
            return false;
        }
        s += samples / 2;
        strokeToCycle2D(border, 6, buffer + 2 * s, BORDER_SAMPLES / 2);
        s += BORDER_SAMPLES / 2;

        memcpy(buffer + 2 * s, marker, (samples / 2 + BORDER_SAMPLES / 2) * 2 * sizeof(short));
        s += samples / 2;
        s += BORDER_SAMPLES / 2;

        timer += (1.f / freq);
        hilliTimer += (1.f / freq) + octave / 165.f;

        // check pitch and shit
        freq = mn2f(startFreq + dF * timer);
        nSPC = SAMPLE_RATE / freq;
        nSPS = nSPC / nStrokes - BORDER_SAMPLES;

        if (GetAsyncKeyState(VK_ESCAPE)) {
            return false;
        }
    }

    scaleBuffer(buffer, DEMO_NUMSAMPLES, p0d45);

    if (!octave) {
        filterBuffer(buffer + (SAMPLE_RATE >> 1) * 46, (SAMPLE_RATE >> 1), 0.45, 1, 0);

        for (int i = 8; i < 24; i++) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 1) * i) * 2, 36000, p0d10, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 1) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        }
        addSamples(buffer + (SAMPLE_RATE >> 2) * 47 * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        addSamples(buffer + SAMPLE_RATE * (10) * 2, riserBuf, RISER_SAMPLES, SAMPLE_RATE / CRASH_RATE);
        filterBuffer(buffer, DEMO_NUMSAMPLES / 2, 0.55, 0.f, 1.f);
    }
    else {
        filterBuffer(buffer, DEMO_NUMSAMPLES, 0.55, 1, 0);
    }
    fadeBuffer(buffer, 48000, p0d10, p1d00);
    addSamples(buffer, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);

    return true;
}

/*
#define TWISTER_SEGMENTS 128
bool finale(short* buffer) {
    int DEMO_NUMSAMPLES = f2i(demo_length(FINALE_SECTION) * SAMPLE_RATE);
    memset(buffer, 0, DEMO_NUMSAMPLES * 2 * sizeof(short));
    return 1;
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
    if (currentStroke == 0) return false;

    float* line = (float*)malloc(sizeof(float) * 6);
    if (line == 0) {
        free(currentStroke);
        return false;
    }

    s = 0;

    for (; s + nSPS + BORDER_SAMPLES < DEMO_NUMSAMPLES;) {
        demo_rand(&seed);
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        // render
        if (useHilli) {
            short* marker = buffer + 2 * s;
            if (!hilligoss(marker, samples / 2, demo_rand(&seed), hilligossTimer, 2.f / SAMPLE_RATE, plasmaFunction, 1)) {
                free(currentStroke);
                free(line);
                return false;
            }
            s += samples / 2;
            strokeToCycle2D(border, 6, buffer + 2 * s, BORDER_SAMPLES / 2);
            s += BORDER_SAMPLES / 2;

            memcpy(buffer + 2 * s, marker, (samples / 2 + BORDER_SAMPLES / 2) * 2 * sizeof(short));
            s += samples / 2;
            s += BORDER_SAMPLES / 2;

            hilligossTimer += 2.0 / freq;
        }
        else {
            // pitched shit
            if (object) {
                yPos = ballOutlineVerts[counter * 3 + 1];

                theta = timer * 2
                    + ((timer + 32) / 16.f) * (cosf(timer * 2.0f * M_PI) + 0.5f) * powf(M_E, -0.5 * powf(fmodf(timer, 8.f) - 4.f, 2.0f)) * powf(M_E, -4 * yPos * yPos)
                    - ((timer - 8.f) / 3.f) * (float(counter % TWISTER_SEGMENTS) / TWISTER_SEGMENTS);

                rotY(ballOutlineVerts + counter * 3, line, theta);
                rotY(ballOutlineVerts + counter * 3, line + 3, theta);
                line[4] += twisterHeight;

                if (line[2] > -1 || (line[2] == -1 && line[0] > 0)) { // line is visible
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
                return false;
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
            if (finaleNotes[p % FINALE_N_PITCHES] == NOFF) freq = mn2f(18);
            else freq = mn2f(finaleNotes[p % FINALE_N_PITCHES]);
            nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
            nSPS = nSPC / (useHilli ? 1 : (object ? nStrokesTwister : nStrokesCube));
            nextTime += SAMPLE_RATE * 0.5f * finaleNoteTimes[p % FINALE_N_TIMES];
        }
    }

    scaleBuffer(buffer, DEMO_NUMSAMPLES, p0d45);

    free(currentStroke);
    free(line);

    return true;
}
void finaleDrums(short* buffer, bool pulses) {
    addSamples(buffer, crashBuf, CRASH_SAMPLES, FILE_RATE / CRASH_RATE);
    int i;
    if (pulses) {
        return;
        pulse(buffer, 256);
    }
    else {
        addSamples(buffer + SAMPLE_RATE * 30 * 2, riserBuf, RISER_SAMPLES, SAMPLE_RATE / CRASH_RATE);
        return;
    }
    
    bool didKick;
    // sidechain and drums
    for (i = 0; i < 256; i++) {
        didKick = false;
        if ((kickPattern & 1 << (15 - (i % 16)))) {
            didKick = true;
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 48000, p0d10, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        }
        if (i % 8 == 4) {
            if (!didKick) fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 36000, p0d50, p1d00);
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

bool outro(short* buffer)
{
    if (!finale(buffer)) return false;
    finaleDrums(buffer, true);
    fadeBuffer(buffer + MID_SECONDS * SAMPLE_RATE, MID_SECONDS * SAMPLE_RATE / 2, 1.f, 0.f);
    filterBuffer(buffer, MID_SECONDS * SAMPLE_RATE / 2, p0d85, p0d70, p0d05);
    filterBuffer(buffer + MID_SECONDS * SAMPLE_RATE, MID_SECONDS * SAMPLE_RATE / 2, p0d85, p0d05, 0.001f);
    fadeBuffer(buffer + MID_SECONDS * SAMPLE_RATE * 2 - 24000, 12000, 1.f, 0.f);
    return true;
}
*/

const unsigned int introMorse = 0b11101010100010001110101110100000;
const char* introText = "RRRREEEEVVVVIIIISSSSIIIIOOOONNNN";
bool intro(short* buffer) {
    int i;
    int n;
    int c;
    
    for (i = 0; i < 16; i++) {
        n = f2i(mn2f(42) / 2);
        c = 0;
        for (; c < n / 2; c++) {
            drawChar(buffer + f2i(SAMPLE_RATE / 2 * i + SAMPLE_RATE * c / mn2f(42)) * 2, f2i(SAMPLE_RATE / mn2f(42)), introText[i * 2], -p0d20, -p0d20, p0d20, p0d20);
        }
        for (; c < n; c++) {
            drawChar(buffer + f2i(SAMPLE_RATE / 2 * i + SAMPLE_RATE * c / mn2f(42)) * 2, f2i(SAMPLE_RATE / mn2f(42)), introText[i * 2 + 1], -p0d20, -p0d20, p0d20, p0d20);
        }
    }
    for (i = 0; i < 128; i++) {
        if ((introMorse & (1 << 31 - (i % 32))) == 0){
            for (c = 0; c < SAMPLE_RATE >> 3; c++) {
                buffer[i * (SAMPLE_RATE >> 3) + c] = 0;
            }
        }
    }

    // sidechain
    for (i = 0; i < 52; i++) {
        if (i % 4 == 0) {
            fadeBuffer(buffer + ((SAMPLE_RATE >> 3) * i) * 2, 36000, p0d10, p1d00);
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
        }
        if (i % 8 == 4 && i > 48) {
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, snareBuf, SNARE_SAMPLES, FILE_RATE / SNARE_RATE);
        }
    }

    // hihats
    for (i = 32; i < 52; i ++) {
        if (i % 32 == 26) addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, openHatBuf, min(OPENHAT_SAMPLES, SAMPLE_RATE >> 3), FILE_RATE / HAT_RATE);
        else if (i % 32 == 24 || i % 32 == 9) {
            addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 3), FILE_RATE / HAT_RATE);
            addSamples(buffer + (SAMPLE_RATE >> 3) + ((SAMPLE_RATE >> 3) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 3), FILE_RATE / HAT_RATE);
        }
        else addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
    }

    addSamples(buffer + SAMPLE_RATE * 6 * 2, riserBuf, RISER_SAMPLES, SAMPLE_RATE / CRASH_RATE);
    return true;
}

/*
void pulse(short* buffer, int n) {
    int j = 0;
    // arps
    for (int i = 0; i < n; i++) {
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
}*/

static const char* textCharacters[2] = {
    " THIS IS   *NOT*  A VECTREX  DEMO   ",
    "LOAD DEMO         READY    RUN?     "
};
bool textSection(short* buffer)
{
    int DEMO_NUMSAMPLES = f2i(TEXT_SECONDS * SAMPLE_RATE);
    int s;
    float nextTime = 0;
    int p = 0;
    float freq = 110;
    float nSPC, nSPS;
    int c = 0;
    float posX;
    float posY;

    const int nStrokes = 32;
    freq = mn2f(textNotes3[0]);
    nSPC = SAMPLE_RATE / freq;
    nSPS = nSPC / nStrokes;
    nextTime += SAMPLE_RATE * 2;

    int counter = 0;
    double prog = 0;
    double targetProg = 0;
    int samples;
    float timer = 0;

    s = 0;
    int sFac;
    for (; s + nSPC < TEXT_SECONDS * SAMPLE_RATE;) {
        demo_rand(&seed);
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        sFac = (s >= 16 * SAMPLE_RATE ? 19 : 9) - min(max(0, (s * 3 / 2 / SAMPLE_RATE - 23)), 3);
        sFac -= (s * 4 / SAMPLE_RATE % 2 == 0) * (s >= 18 * SAMPLE_RATE);
        if (counter < sFac) {
            strokeToCycle2D(border, 6, buffer + 2 * s, samples * sFac);
            counter += sFac - 1;
            targetProg += nSPS * (sFac - 1);
            prog += samples * (sFac - 1);
            s += samples * sFac;
        }
        else {
            while (textCharacters[s >= 16 * SAMPLE_RATE][c] == ' ') c = (c + 1) % 36;
            posX = ((c % 9) + (s < 16 * SAMPLE_RATE ? c / 27 : 0) / 2.f) / 8.f - 0.5625f + 0.0125;
            posY = (c / 9) / -4.f + 0.25f + 0.0625f + 0.0125;
            drawChar(buffer + 2 * s, samples, textCharacters[s >= 16 * SAMPLE_RATE][c], (posX) * 1.5f, (posY) * 1.5f, (posX + p0d10) * 1.5f, (posY + p0d10) * 1.5f);
            s += samples;
            c = (c + 1) % 36;
        }

        counter = (counter + 1) % nStrokes;
        if (counter == 0) {
            timer += 1.f / freq;
            prog -= nSPC;
            targetProg -= nSPC;
            c = 0;
        }

        while (s > nextTime && s < SAMPLE_RATE * 16) {
            prog = 0;
            targetProg = 0;
            p++;
            nextTime += SAMPLE_RATE * 2;
            freq = mn2f(textNotes3[p % 4]);
            nSPC = SAMPLE_RATE / freq;
            nSPS = nSPC / nStrokes;
        }

        if (GetAsyncKeyState(VK_ESCAPE)) {
            return false;
        }
    }

    scaleBuffer(buffer, DEMO_NUMSAMPLES, p0d45);
    filterBuffer(buffer, 4 * SAMPLE_RATE, 0.0f, 0.0f, 0.1f);
    filterBuffer(buffer + 8 * SAMPLE_RATE, 8 * SAMPLE_RATE, 0.0f, 0.1f, 0.5f);
    filterBuffer(buffer + 24 * SAMPLE_RATE, 4 * SAMPLE_RATE, 0.0f, 0.5f, 1.f);

    return textDrums(buffer);
}
bool textDrums(short* buffer)
{
    addSamples(buffer, crashBuf, CRASH_SAMPLES, FILE_RATE / CRASH_RATE);
    fadeBuffer(buffer, 48000, p0d10, p1d00);
    addSamples(buffer, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
    for (int i = 0; i < 128; i++) {
        addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulseSingleBuf2, PULSE_SAMPLES / 4, mn2f(55) / mn2f(textNotes2[(i / 16) % 4]));
        addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulseSingleBuf, PULSE_SAMPLES / 2, mn2f(55) / mn2f(textNotes[i % TEXT_N_PITCHES]));
    }
    for (int i = 128; i < 160; i++) {
        addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulseSingleBuf2, PULSE_SAMPLES / 4, mn2f(55) / mn2f(textNotes2[3]));
        addSamples(buffer + ((SAMPLE_RATE >> 3) * i) * 2, pulseSingleBuf, PULSE_SAMPLES / 2, mn2f(55) / mn2f(textNotes[i % 16 + 48]));
    }
    addSamples(buffer + SAMPLE_RATE * 37, riserBuf, RISER_SAMPLES, SAMPLE_RATE / CRASH_RATE);
    return true;
}

static const char* greetsText[] = {
    "* READY? ***********",
    "* READY? ***********",
    "*CREDITS ***********",
    "*CREDITS ***********",
    "** CODE **DJ_LEVEL_3",
    "GFX  MUSICDJ_LEVEL_3",
    "CODIRECTORBUS ERROR ",
    "CODIRECTORTACHY BNKR",
    "** PLUS ** THE EPIC ",
    "BUS ERROR COLLECTIVE",
    "*BUS ERROR*DJ_LVL_3 ",
    "*MARV94   *MICKALEUS",
    "*TACHY     BUNKER   ",
    "*BRENDEN  *ANTHONY  ",
    "* GREETS **** TO ***",
    "* GREETS **** TO ***",
    "*TRSI     *FAIRLIGHT",
    "*DANKO    *ASTAROTH ",
    "*PSENOUGH *LILAC    ",
    "*JAMES H   BALL     ",
    "THANK  YOU REVISION ",
    "THANK  YOU REVISION ",
    "TLTYP CORPLOVES YOU ",
    "TLTYP CORPLOVES YOU ",
    "AMIGAAAAAAAAAAAAAAAA",
};
bool greetz(short* buffer)
{
    int DEMO_NUMSAMPLES = f2i(GREETZ_SECONDS * SAMPLE_RATE);
    int s;
    float nextTime = 0;
    int p = 0;
    float freq = 110;
    float nSPC, nSPS;
    int c = 0;
    float posX;
    float posY;

    int nStrokes = 21;
    freq = mn2f(38);
    nSPC = SAMPLE_RATE / freq;
    nSPS = nSPC / nStrokes;
    nextTime += SAMPLE_RATE * 2;

    int counter = 0;
    double prog = 0;
    double targetProg = 0;
    int samples;
    float timer = 0;

    s = 0;
    int sFac;
    bool drawn;
    for (; s + nSPC < (GREETZ_SECONDS - 6) * SAMPLE_RATE;) {
        demo_rand(&seed);
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;

        posX = ((c % 10) * (10 / 9.f) - 5.f) / 9.f - 0.05f;
        posY = (c / 10) * -0.2f + 0.05f;
        drawChar(buffer + 2 * s, samples, greetsText[min(24, (s / SAMPLE_RATE))][c], (posX) * 1.5f, (posY) * 1.5f, (posX + p0d10) * 1.5f, (posY + p0d10) * 1.5f);
        s += samples;
        c = (c + 1);
        if (c == 20) {
            c = 0;
            strokeToCycle2D(border, 6, buffer + 2 * s, samples);
            s += samples;
            prog += samples;
            targetProg += nSPS;
        }
    }
    nStrokes = 12;
    nSPS = nSPC / nStrokes;
    for (; s + nSPC < (GREETZ_SECONDS)*SAMPLE_RATE;) {
        demo_rand(&seed);
        targetProg += nSPS * (3 - c / 2);
        samples = f2i(targetProg - prog);
        prog += samples;

        strokeToCycle2D(revisionVerts3D + 3 * revisionOffsets[c], revisionLengths[c], buffer + 2 * s, samples);
        scaleBuffer(buffer + 2 * s, samples * 2, 1.3f);

        s += samples;
        c = (c + 1) % 6;
    }
    
    scaleBuffer(buffer, DEMO_NUMSAMPLES, 0.45f);

    // hihats
    int i;
    for (i = 0; i < 88; i++) {
        if (i % 16 == 13) addSamples(buffer + ((SAMPLE_RATE >> 2) * i) * 2, openHatBuf, min(OPENHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
        else if (i % 8 == 3) {
            addSamples(buffer + ((SAMPLE_RATE >> 2) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 3), FILE_RATE / HAT_RATE);
            addSamples(buffer + (SAMPLE_RATE >> 2) + ((SAMPLE_RATE >> 2) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 3), FILE_RATE / HAT_RATE);
        }
        else addSamples(buffer + ((SAMPLE_RATE >> 2) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
    }

    filterBuffer(buffer, SAMPLE_RATE * 3 / 2, 0.55, 0, 1);/*
    filterBuffer(buffer + 50 * SAMPLE_RATE, SAMPLE_RATE, 0.3, 1.0f, 0.5f);
    filterBuffer(buffer + 52 * SAMPLE_RATE, SAMPLE_RATE, 0.3, 0.5f, 0.25f);
    filterBuffer(buffer + 54 * SAMPLE_RATE, SAMPLE_RATE, 0.3, 0.25f, 0.0f);*/
    filterBuffer(buffer + 52 * SAMPLE_RATE, SAMPLE_RATE * 8, 0.0, 1.f, 0.0f);

    fadeBuffer(buffer, 48000, p0d10, p1d00);
    addSamples(buffer, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);

    addSamples(buffer + 52 * SAMPLE_RATE, riserBuf, RISER_SAMPLES, SAMPLE_RATE / CRASH_RATE);
    addSamples(buffer + 28 * SAMPLE_RATE * 2, crashBuf, CRASH_SAMPLES, FILE_RATE / CRASH_RATE);

    return true;
}

bool midSection(short* buffer) {
    int DEMO_NUMSAMPLES = f2i(demo_length(MID_SECTION) * SAMPLE_RATE);
    int s;
    float nextTime = 0;
    int p = 0;
    int nStrokes;
    int faces;
    int curFace = 0;
    float freq, nSPC, nSPS;

    nStrokes = 0;
    faces = curFace;
    while (faces == curFace) {
        demo_rand(&seed);
        demo_rand(&seed);
        demo_rand(&seed);
        faces = (demo_rand(&seed) & 0b1111) % 6;
    }
    curFace = faces;
    faces = 0b111111111111 - (0b11 << (faces * 2));
    nStrokes = 1;

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

    float* currentStroke = (float*)malloc(sizeof(float) * (3 * 5));
    if (currentStroke == 0) {
        return false;
    }

    for (; s + nSPC + BORDER_SAMPLES < DEMO_NUMSAMPLES;) {
        demo_rand(&seed);
        targetProg += nSPS;
        samples = f2i(targetProg - prog);
        prog += samples;
        int mask = (0b11 << (counter * 2));
        if ((faces & mask) == false || nStrokes == 6) {
            for (int v = 0; v < 5; v++) {
                currentStroke[v * 3 + 0] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 0];
                currentStroke[v * 3 + 1] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 1];
                currentStroke[v * 3 + 2] = cubeVerts[3 * cubeIndices[counter * 5 + v] + 2];
                rotX(currentStroke + v * 3, currentStroke + v * 3, sinf(M_PI_F * timer / 2) * M_PI_F);
                rotY(currentStroke + v * 3, currentStroke + v * 3, M_PI_F * timer);
                rotZ(currentStroke + v * 3, currentStroke + v * 3, M_PI_F * timer);
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
            
            nStrokes = 0;
            while (nStrokes == 0) {
                faces = demo_rand(&seed) & 0b111111111111;
                for (int i = 0; i < 6; i++) {
                    if ((faces & (3 << (i * 2))) == false) {
                        nStrokes++;
                    }
                }
            }
            if (s > SAMPLE_RATE * 6) {
                nStrokes = 6;
            }
            

            // check pitch and shit
            freq = mn2f(cubeNotes[p % CUBE_N_PITCHES] - 12 * (s < SAMPLE_RATE * 4));
            nSPC = SAMPLE_RATE / freq - BORDER_SAMPLES;
            nSPS = nSPC / nStrokes;
            nextTime += SAMPLE_RATE * cubeNoteTimes[p % CUBE_N_TIMES];
        }

        if (GetAsyncKeyState(VK_ESCAPE)) {
            free(currentStroke);
            return false;
        }
    }

    free(currentStroke);

    wobbleBufferEnv(buffer, SAMPLE_RATE * 6, SAMPLE_RATE / 1.5f, 0, 2.f / 3, 0.f, 0.12f, 0.0f, 0.5f);
    wobbleBufferEnv(buffer, SAMPLE_RATE * 6, SAMPLE_RATE / 2.0f, 0, 0.f, 2.f / 3, 0.0f, 0.12f, 0.5f);
    scaleBuffer(buffer, DEMO_NUMSAMPLES, p0d45);

    for (s = 0; s < 4; s++) {
        fadeBuffer(buffer + 2 * s * SAMPLE_RATE * 2, 48000, p0d10, p1d00);
        addSamples(buffer + 2 * s * SAMPLE_RATE * 2, kickBuf, KICK_SAMPLES, FILE_RATE / KICK_RATE);
    }
    for (int i = 0; i < 32; i++) {
        if (i % 16 == 13) addSamples(buffer + ((SAMPLE_RATE >> 2) * i) * 2, openHatBuf, min(OPENHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
        else if (i % 8 == 3) {
            addSamples(buffer + ((SAMPLE_RATE >> 2) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 3), FILE_RATE / HAT_RATE);
            addSamples(buffer + (SAMPLE_RATE >> 2) + ((SAMPLE_RATE >> 2) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 3), FILE_RATE / HAT_RATE);
        }
        else addSamples(buffer + ((SAMPLE_RATE >> 2) * i) * 2, closedHatBuf, min(CLOSEDHAT_SAMPLES, SAMPLE_RATE >> 2), FILE_RATE / HAT_RATE);
    }

    if (GetAsyncKeyState(VK_ESCAPE)) {
        return false;
    }
    return true;
}