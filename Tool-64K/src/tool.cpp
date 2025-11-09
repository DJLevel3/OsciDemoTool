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

/*
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
}*/

static float vec2Distance(float* vec2A, float* vec2B)
{
    float diff[2] = { vec2B[0] - vec2A[0],
                      vec2B[1] - vec2A[1] };

    return sqrtf(diff[0] * diff[0] + diff[1] * diff[1]);
}

static inline void vec2ToSample(float* vec2, short* buffer)
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
    float* lengths = (float*)malloc(nPoints * sizeof(float));
    if (lengths == 0) return false;

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
    free(lengths);

    return true;
}

bool strokeToCycle2D(float* points, int nPoints, short* buffer, int samples)
{
    if (nPoints < 2) return false;
    float distance = 0;

    // allocate floats for lengths
    float* lengths = (float*)malloc(nPoints * sizeof(float));
    if (lengths == 0) return false;

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
    free(lengths);
    
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
        value1 = fabsf(buffer[i * 2]);
        value2 = fabsf(buffer[i * 2 + 1]);
        value1 = max(value1, value2);
        maxValue = max(value1, maxValue);
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

        wx = sinf(((i + phase) / periodT + scaleX * y) * 2.f * M_PI);
        wy = sinf(((i + phase) / periodT + scaleY * x) * 2.f * M_PI);

        ex = (powf(M_E, -curve * x * x)- powf(M_E, -curve)) / (1 - powf(M_E, -curve));
        ey = (powf(M_E, -curve * y * y)- powf(M_E, -curve)) / (1 - powf(M_E, -curve));

        buffer[i * 2] = min(SHRT_MAX, max(SHRT_MIN, buffer[i * 2] + f2i(ex * wx * intensityX * SHRT_MAX)));
        buffer[i * 2 + 1] = min(SHRT_MAX, max(SHRT_MIN, buffer[i * 2 + 1] + f2i(ey * wy * intensityY * SHRT_MAX)));
    }
}
/*
void wobbleBuffer(short* buffer, int samples, float periodT, int phase, float intensityX, float intensityY)
{
    float x, y, wx, wy;
    for (int i = 0; i < samples; i++) {
        x = buffer[i * 2] / (float)SHRT_MAX;
        y = buffer[i * 2 + 1] / (float)SHRT_MAX;

        wx = sinf(((i + phase) / periodT) * 2.f * M_PI);
        wy = sinf(((i + phase) / periodT) * 2.f * M_PI);

        buffer[i * 2] = min(SHRT_MAX, max(SHRT_MIN, buffer[i * 2] + f2i(wx * intensityX * SHRT_MAX)));
        buffer[i * 2 + 1] = min(SHRT_MAX, max(SHRT_MIN, buffer[i * 2 + 1] + f2i(wy * intensityY * SHRT_MAX)));
    }
}*/
/*
void circleFill(short* buffer, int samples, int period, float level)
{
    for (int i = 0; i < samples; i++) {
        buffer[i * 2] = f2i(SHRT_MAX * sin(i * 2.f * M_PI / period) * level);
        buffer[i * 2 + 1] = f2i(SHRT_MAX * cos(i * 2.f * M_PI / period) * level);
    }
}*/
float squareWaveNoDC(int t, int period, float pulseWidth) {
    return ((t % period) <= pulseWidth * period) ? (pulseWidth - 1) : (pulseWidth);
}
float squareWaveCentered(int t, int period, float pulseWidth) {
    return ((t % period) <= pulseWidth * period) ? -0.5 : 0.5;
}
/*float squareWaveZeroed(int t, int period, float pulseWidth) {
    return ((t % period) <= pulseWidth * period) ? 0 : 1;
}*/

float mn2f(float n) {
    return SPEED_FACTOR * 261.63f * powf(2.0f, (n - 60) / 12.f);
}
void filterBuffer(short* buffer, int samples, float resonance, float start, float end)
{
    float state0_L = 0;
    float state1_L = 0;
    float state2_L = 0;
    float state0_R = 0;
    float state1_R = 0;
    float state2_R = 0;
    float fb, cut;
    float t = start;
    float dt = (end - start) / samples;
    for (int i = 0; i < samples; i++)
    {
        cut = min(t, 0.9999f);
        fb = resonance + resonance / (1.0 - cut);

        state0_L += cut * (buffer[2 * i] - state0_L + fb * (state1_L - state2_L));
        state1_L += cut * (state0_L - state1_L);
        state2_L += cut * (state1_L - state2_L);
        buffer[2 * i] = f2i(state2_L);


        state0_R += cut * (buffer[2 * i + 1] - state0_R + fb * (state1_R - state2_R));
        state1_R += cut * (state0_R - state1_R);
        state2_R += cut * (state1_R - state2_R);
        buffer[2 * i + 1] = f2i(state2_R);
        t += dt;
    }
}

void rotX(float* src, float* dest, float theta) {
    float sT = sinf(theta);
    float cT = cosf(theta);

    float x, y;
    x = cT * src[1] - sT * src[2];
    y = sT * src[1] + cT * src[2];

    dest[0] = src[0];
    dest[1] = x;
    dest[2] = y;
}

void rotY(float* src, float* dest, float theta) {
    float sT = sinf(theta);
    float cT = cosf(theta);

    float x, y;
    x = cT * src[0] - sT * src[2];
    y = sT * src[0] + cT * src[2];

    dest[0] = x;
    dest[1] = src[1];
    dest[2] = y;
}

void rotZ(float* src, float* dest, float theta) {
    float sT = sinf(theta);
    float cT = cosf(theta);

    float x, y;

    x = cT * src[0] - sT * src[1];
    y = sT * src[0] + cT * src[1];

    dest[0] = x;
    dest[1] = y;
    dest[2] = src[2];
}

bool hilligoss(short* buffer, int samples, int randomSeed, float tInitial, float tRate, unsigned char(*gen)(short x, short y, float t), int stretch) {
    short* plasmaPointsOrdered = (short*)malloc(samples * 2 * sizeof(short));
    if (plasmaPointsOrdered == 0) {
        memset(buffer, 0, samples * 2 * sizeof(short));
        return 0;
    }
    short* plasmaPointsUnordered = (short*)malloc(samples * 2 * sizeof(short));
    if (plasmaPointsUnordered == 0) {
        free(plasmaPointsOrdered);
        memset(buffer, 0, samples * 2 * sizeof(short));
        return 0;
    }
    float* lookup = (float*)malloc(256 * sizeof(float));
    if (lookup == 0) {
        free(plasmaPointsOrdered);
        free(plasmaPointsUnordered);
        memset(buffer, 0, samples * 2 * sizeof(short));
        return 0;
    }
    int seed = randomSeed;
    short random1;
    short random2;
    int nPoints = 0;
    int nPointsSaver = 0;
    float z;
    float thresh = 0;
    long iterationCounter = 0;

    // Lookup table
    for (int i = 0; i < 256; i++) {
        z = i / 256.f;
        z *= z;
        lookup[i] = z;
    }

    // choosePixels
    nPoints = 0;
    while (nPoints < samples && !GetAsyncKeyState(VK_ESCAPE)) {
        random1 = (demo_rand(&seed));
        random2 = (demo_rand(&seed));
        thresh = fabsf(demo_rand(&seed) / 32768.f);
        z = lookup[(*gen)(random1, random2, tInitial + tRate * nPoints)];
        if (z - lookup[30] > thresh) {
            plasmaPointsUnordered[nPoints * 2] = random1;
            plasmaPointsUnordered[nPoints * 2 + 1] = random2;
            nPoints++;
        }
        iterationCounter++;
        if (iterationCounter > MAX_HILLIGOSS_ITERATIONS) break;
    }

    if (nPoints == 0 || GetAsyncKeyState(VK_ESCAPE)) {
        free(lookup);
        free(plasmaPointsOrdered);
        free(plasmaPointsUnordered);
        memset(buffer, 0, samples * 2 * sizeof(short));
        return 0;
    }
    nPointsSaver = 0;
    while (nPoints < samples) {
        plasmaPointsUnordered[nPoints * 2]     = plasmaPointsUnordered[nPointsSaver * 2];
        plasmaPointsUnordered[nPoints * 2 + 1] = plasmaPointsUnordered[nPointsSaver * 2 + 1];
        nPoints++;
        nPointsSaver++;
    }

    // determinePath
    if (determinePath(plasmaPointsUnordered, samples)) {
        for (int i = 0; i < samples; i++) {
            buffer[i * 2] = plasmaPointsUnordered[(i / stretch) * stretch * 2];
            buffer[i * 2 + 1] = plasmaPointsUnordered[(i / stretch) * stretch * 2 + 1];
        }
    }
    else {
        free(lookup);
        free(plasmaPointsOrdered);
        free(plasmaPointsUnordered);
        return 0;
    }
    free(lookup);
    free(plasmaPointsOrdered);
    free(plasmaPointsUnordered);
    return nPoints;
}

// Find an order through which the pixels should be traversed and convert it into 16-bit PCM audio
bool determinePath(short* pixelsOriginal, int targetCount)
{
    short* path = (short*)malloc(targetCount * 2 * sizeof(short));
    if (path == 0) {
        return 0;
    }
    int pathLength = 0;
    int nPix = targetCount;

    // While we haven't hit the target count, and while there are still pixels on the map...
    while (pathLength < targetCount && nPix > 0)
    {
        if (GetAsyncKeyState(VK_ESCAPE)) {
            free(path);
            return 0;
        }
        // Add that starting point to the path
        path[pathLength * 2] = pixelsOriginal[0];
        path[pathLength * 2 + 1] = pixelsOriginal[1];
        pathLength++;
        nPix--;

        // Remove the starting point from the map
        pixelsOriginal[0] = pixelsOriginal[nPix * 2];
        pixelsOriginal[1] = pixelsOriginal[nPix * 2 + 1];

        for (int jumpCounter = 0; jumpCounter < 10000 && pathLength < targetCount; jumpCounter++)
        {
            // Closest index is invalid for now
            long closestIndex = -1;

            // Closest distance is infinite for now
            float minDistance = 655360;

            for (int pixel = 0; pixel < nPix; pixel++) {
                short x = pixelsOriginal[pixel * 2];
                short y = pixelsOriginal[pixel * 2 + 1];
                // If the pixel is within search distance
                if (fabsf(float(x) - path[pathLength * 2 - 2]) < 4096 && fabsf(float(y) - path[pathLength * 2 - 1]) < 4096) {
                    // Calculate the distance to the current pixel (sqrt(dx^2 + dy^2))
                    float dist = sqrtf(powf(float(path[(pathLength * 2 - 2)]) - float(x), 2.0) + powf(float(path[(pathLength * 2 - 1)] - y), 2.0));
                    // If it's a new record for the closest point
                    if ((dist > 0) && (dist <= minDistance))
                    {
                        // Set the closest index and distance to the new record point
                        closestIndex = pixel;
                        minDistance = dist;
                    }
                }
            }

            // If we found a pixel in the search radius
            if (closestIndex >= 0)
            {
                // Add it to the path
                path[pathLength * 2] = pixelsOriginal[closestIndex * 2];
                path[pathLength * 2 + 1] = pixelsOriginal[closestIndex * 2 + 1];
                pathLength++;

                nPix--;
                pixelsOriginal[closestIndex * 2] = pixelsOriginal[nPix * 2];
                pixelsOriginal[closestIndex * 2 + 1] = pixelsOriginal[nPix * 2 + 1];
            }
            // Otherwise, begin a new stroke
            else
            {
                break;
            }
        }
    }
    memcpy(pixelsOriginal, path, targetCount * 2 * sizeof(short));
    free(path);
    return 1;
}