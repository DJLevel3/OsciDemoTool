//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#include "tool.h"

// loading tune
void mzk_init( short *buffer )
{
    for( int i=0; i<INTROMUSIC_NUMSAMPLES; i++ ) 
    {
        buffer[2 * i + 0] = f2i(16384.f * sinf(6.2831f * (0.0011458333f * (float)i)));
        buffer[2 * i + 1] = f2i(16384.f * sinf(6.2831f * (0.0023020834f * (float)i)));
    }
}

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

void vec3ToSample(float* vec3, short* buffer)
{
    float projected[3] = { vec3[0], vec3[1], vec3[2] };
    mvp43(view_matrix, projected, projected);
    mvp43(proj_matrix, projected, projected);
    buffer[0] = f2i(projected[0] * 32767.0f);
    buffer[1] = f2i(projected[1] * 32767.0f);
}

void lineToSamples(float* vec3Start, float* vec3End, short* buffer, int samples)
{
    float pos[3];
    float dPos[3];
    float delta = 1.0f / samples;

    pos[0] = vec3Start[0];
    pos[1] = vec3Start[1];
    pos[2] = vec3Start[2];
    dPos[0] = (vec3End[0] - vec3Start[0]) * delta;
    dPos[1] = (vec3End[1] - vec3Start[1]) * delta;
    dPos[2] = (vec3End[2] - vec3Start[2]) * delta;
    for (int i = 0; i < samples; i++) {
        vec3ToSample(pos, buffer + (2 * i));
        pos[0] += dPos[0];
        pos[1] += dPos[1];
        pos[2] += dPos[2];
    }
}

bool strokeToCycle(float* points, int nPoints, short* buffer, int samples)
{
    if (nPoints < 2) return false;
    float distance = 0;

    // allocate floats for lengths
    float* lengths = renderer_scratch + scSP;
    scSP += nPoints;

    // calculate line lengths and total frame distance
    for (int i = 1; i < nPoints; i++) {
        lengths[i - 1] = vec3Distance(points + (i - 1) * 3, points + (i * 3));
        distance += lengths[i - 1];
    }
    if (distance <= 0) return false;

    float lengthFactor = samples / distance;

    int bCounter = 0;
    int dS;

    for (int i = 1; i < nPoints; i++) {
        dS = f2i(lengths[i - 1] * lengthFactor);
        lineToSamples(points + (i - 1) * 3, points + (i * 3), buffer + (2 * bCounter), dS);
        bCounter += dS;
    }
    while (bCounter < samples) {
        buffer[bCounter] = buffer[bCounter - 2];
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