//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#include "tool.h"

#ifdef LISSAJOUS_INTRO
// loading tune
void mzk_init( short *buffer )
{
    for( int i=0; i<INTROMUSIC_NUMSAMPLES; i++ ) 
    {
        buffer[2 * i + 0] = f2i(16384 * sinf(p0d01 * (i * 1001 / 500)));
        buffer[2 * i + 1] = f2i(16384 * sinf(p0d01 * (i * 3)));
    }
}
#endif

static float vec3Distance(float* vec3A, float* vec3B)
{
    float dist = 0;
    float diff[3] = { vec3B[0] - vec3A[0],
                      vec3B[1] - vec3A[1],
                      vec3B[2] - vec3A[2] };

    dist += diff[0] * diff[0];
    dist += diff[1] * diff[1];
    dist += diff[2] * diff[2];

    return sqrtf(dist);
}

static float vec2Distance(float* vec2A, float* vec2B)
{
    float dist = 0;
    float diff[2] = { vec2B[0] - vec2A[0],
                      vec2B[1] - vec2A[1] };

    dist += diff[0] * diff[0];
    dist += diff[1] * diff[1];

    return sqrtf(dist);
}

void vec2ToSample(float* vec2, short* buffer)
{
    buffer[0] = f2i(vec2[0] * SHRT_MAX);
    buffer[1] = f2i(vec2[1] * SHRT_MAX);
}

void lineToSamples(float* vec3Start, float* vec3End, short* buffer, int samples)
{
    float pos[2];
    float dPos[2];
    float delta = 1.0f / samples;

    pos[0] = vec3Start[0];
    pos[1] = vec3Start[1];

    dPos[0] = (vec3End[0] - pos[0]) * delta;
    dPos[1] = (vec3End[1] - pos[1]) * delta;
    for (int i = 0; i < samples; i++) {
        vec2ToSample(pos, buffer + (2 * i));
        pos[0] += dPos[0];
        pos[1] += dPos[1];
    }
}

bool strokeToCycle3D(float* points, int nPoints, short* buffer, int samples, float* viewMatrix)
{
    if (nPoints < 2) return false;
    float distance = 0;

    // allocate floats for lengths
    float* lengths = renderer_scratch + scSP;
    scSP += nPoints;

    for (int i = 0; i < nPoints; i++) {
        mvp43(viewMatrix, points + i * 3, points + i * 3);
        mvp43(proj_matrix, points + i * 3, points + i * 3);
    }

    // calculate line lengths and total frame distance
    for (int i = 0; i < nPoints - 1; i++) {
        lengths[i] = vec2Distance(points + i * 3, points + ((i + 1) * 3));
        distance += lengths[i];
    }
    if (distance <= 0) return false;

    float lengthFactor = samples / distance;

    int bCounter = 0;
    int dS;

    for (int i = 0; i < nPoints - 1; i++) {
        dS = f2i(lengths[i] * lengthFactor);
        lineToSamples(points + i * 3, points + ((i + 1) * 3), buffer + (2 * bCounter), dS);
        bCounter += dS;
    }
    while (bCounter < samples) {
        buffer[bCounter * 2] = buffer[bCounter * 2 - 2];
        buffer[bCounter * 2 + 1] = buffer[bCounter * 2 - 1];
        bCounter++;
    }

    // deallocate scratch
    scSP -= nPoints;

    return true;
}

bool strokeToCycle2D(float* points, int nPoints, short* buffer, int samples)
{
    if (nPoints < 2) return false;
    float distance = 0;

    // allocate floats for lengths
    float* lengths = renderer_scratch + scSP;
    scSP += nPoints;

    // calculate line lengths and total frame distance
    for (int i = 0; i < nPoints - 1; i++) {
        lengths[i] = vec2Distance(points + i * 3, points + ((i + 1) * 3));
        distance += lengths[i];
    }
    if (distance <= 0) return false;

    float lengthFactor = samples / distance;

    int bCounter = 0;
    int dS;

    for (int i = 0; i < nPoints - 1; i++) {
        dS = f2i(lengths[i] * lengthFactor);
        lineToSamples(points + i * 3, points + ((i + 1) * 3), buffer + (2 * bCounter), dS);
        bCounter += dS;
    }
    while (bCounter < samples) {
        buffer[bCounter * 2] = buffer[bCounter * 2 - 2];
        buffer[bCounter * 2 + 1] = buffer[bCounter * 2 - 1];
        bCounter++;
    }

    // deallocate scratch
    scSP -= nPoints;

    return true;
}

void addSamples(short* buffer, short* toAdd, int samples, float stretch)
{
    int l, r;
    for (int i = 0; i < f2i(samples * stretch); i++) {
        l = buffer[i * 2] + toAdd[f2i(i / stretch) * 2];
        l = (l > SHRT_MAX ? SHRT_MAX : l);
        l = (l < SHRT_MIN ? SHRT_MIN : l);

        r = buffer[i * 2 + 1] + toAdd[f2i(i / stretch) * 2 + 1];
        r = (r > SHRT_MAX ? SHRT_MAX : r);
        r = (r < SHRT_MIN ? SHRT_MIN : r);

        buffer[i * 2] = l;
        buffer[i * 2 + 1] = r;
    }
}

float normalizeBuffer(short* buffer, int samples, float volume)
{
    float maxValue = 0;
    float value1 = 0;
    float value2 = 0;
    for (int i = 0; i < samples; i++) {
        value1 = fabsl(buffer[i * 2]);
        value2 = fabsl(buffer[i * 2 + 1]);
        value1 = (value1 > value2) ? value1 : value2;
        maxValue = (value1 > maxValue) ? value1 : maxValue;
    }

    maxValue /= 32767.f;
    for (int i = 0; i < samples; i++) {
        buffer[i * 2] = f2i(buffer[i * 2] * volume / maxValue);
        buffer[i * 2 + 1] = f2i(buffer[i * 2 + 1] * volume / maxValue);
    }

    return maxValue;
}

void fadeBuffer(short* buffer, int samples, float start, float end)
{
    float delta = (end - start) / samples;
    for (int i = 0; i < samples; i++) {
        buffer[i * 2] = f2i(buffer[i * 2] * (start + i * delta));
        buffer[i * 2 + 1] = f2i(buffer[i * 2 + 1] * (start + i * delta));
    }
}

void wobbleBufferEnv(short* buffer, int samples, float periodT, int phase, float scaleX, float scaleY, float intensityX, float intensityY, float curve)
{
    float x, y, wx, wy, ex, ey;
    for (int i = 0; i < samples; i++) {
        x = buffer[i * 2] / (float)SHRT_MAX;
        y = buffer[i * 2 + 1] / (float)SHRT_MAX;

        wx = sin(((i + phase) / periodT + scaleX * x) * 2.f * M_PI);
        wy = sin(((i + phase) / periodT + scaleY * y) * 2.f * M_PI);

        ex = (powf(M_E, -curve * x * x)- powf(M_E, -curve)) / (1 - powf(M_E, -curve));
        ey = (powf(M_E, -curve * y * y)- powf(M_E, -curve)) / (1 - powf(M_E, -curve));

        buffer[i * 2] = min(SHRT_MAX, max(SHRT_MIN, buffer[i * 2] + f2i(ex * wx * intensityX * SHRT_MAX)));
        buffer[i * 2 + 1] = min(SHRT_MAX, max(SHRT_MIN, buffer[i * 2 + 1] + f2i(ey * wy * intensityY * SHRT_MAX)));
    }
}

void wobbleBuffer(short* buffer, int samples, float periodT, int phase, float intensityX, float intensityY)
{
    float x, y, wx, wy;
    for (int i = 0; i < samples; i++) {
        x = buffer[i * 2] / (float)SHRT_MAX;
        y = buffer[i * 2 + 1] / (float)SHRT_MAX;

        wx = sin(((i + phase) / periodT) * 2.f * M_PI);
        wy = sin(((i + phase) / periodT) * 2.f * M_PI);

        buffer[i * 2] = min(SHRT_MAX, max(SHRT_MIN, buffer[i * 2] + f2i(wx * intensityX * SHRT_MAX)));
        buffer[i * 2 + 1] = min(SHRT_MAX, max(SHRT_MIN, buffer[i * 2 + 1] + f2i(wy * intensityY * SHRT_MAX)));
    }
}