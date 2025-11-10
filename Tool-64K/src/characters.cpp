#include "characters.h"

static const char characterVerts[] = {
    -10, -10, // S (overlaps C/U)
     10, -10,
     10,  0,
    -10,  0,
    -10,  10,
     10,  10, // C/U (overlaps S/R)
    -5,   10,
    -10,  5,
    -10, -5,
    -5,  -10,
     10, -10, // R (overlaps C/U/A/V)
    -10,  0,
     10,  0,
     10,  10,
    -10,  10,
    -10, -10, // A/V (overlaps R/B)
     0,   10,
     10, -10,
     5,   0,
    -5,   0,
    -10, -10, // B (overlaps A/V/M/W)
    -10,  10,
     6,   10,
     10,  6,
     10,  2,
     8,   0,
    -10,  0,
     8,   0,
     10, -2,
     10, -6,
     6,  -10,
    -10, -10, // M/W (overlaps B)
    -10,  6,
    -6,   10,
    -4,   10,
     0,   6,
     0,   0,
     0,   6,
     4,   10,
     6,   10,
     10,  6,
     10, -10, // H/I (overlaps M/W/I/Z/N)
    -10, -10,
     0,  -10,
     0,   10,
     10,  10,
    -10,  10, // Z/N (overlaps H/I/E)
     10,  10,
    -10, -10,
     10, -10,
    -10, -10,
     10,  10,
    -10,  10,
     10,  10, // E (overlaps Z/N/T)
    -10,  10,
    -10, -10,
     10, -10,
    -10, -10,
    -10,  0,
     10,  0,
    -10,  0,
    -10,  10,
     10,  10,
    -10,  10, // T (overlaps E)
     10,  10,
     0,   10,
     0,  -10,
    -10,  10, // L
    -10, -10,
     10, -10,
    -10,  5,  // O
    -5,   10,
     5,   10,
     10,  5,
     10, -5,
     5,  -10,
    -5,  -10,
    -10, -5,
    -10,  5,
    -5,   10,
    -10, -10, // K
    -10,  10,
    -10,  0,
     10,  10,
    -10,  0,
     10, -10,
};
static const char vertCounts[26] = {
    6,  // A
    12, // B
    6,  // C
    0,  // D
    11, // E
    0,  // F
    0,  // G
    6,  // H
    6,  // I
    0,  // J
    6,  // K
    3,  // L
    11, // M
    8,  // N
    10,  // O
    0,  // P
    0,  // Q
    6,  // R
    6,  // S
    4,  // T
    6,  // U
    3,  // V
    11, // W
    0,  // X
    0,  // Y
    8,  // Z
};

// swap, flipX, flipY (applied in that order)
static const char vertFlipsSwaps[26] = {
    0b000, // A
    0b000, // B
    0b000, // C
    0b000, // D
    0b000, // E
    0b000, // F
    0b000, // G
    0b100, // H
    0b000, // I
    0b000, // J
    0b000, // K
    0b000, // L
    0b000, // M
    0b101, // N
    0b000, // O
    0b000, // P
    0b000, // Q
    0b000, // R
    0b000, // S
    0b000, // T
    0b100, // U
    0b001, // V
    0b001, // W
    0b000, // X
    0b000, // Y
    0b000, // Z
};

static const char vertOffsets[26] = {
    15, // A
    20, // B
    5,  // C
    0,  // D
    53, // E
    0,  // F
    0,  // G
    41, // H
    41, // I
    0,  // J
    80,  // K
    67, // L
    31, // M
    46, // N
    70, // O
    0,  // P
    0,  // Q
    10, // R
    0,  // S
    63, // T
    5,  // U
    15, // V
    31, // W
    0,  // X
    0,  // Y
    46, // Z
};

void drawChar(short* buffer, int samples, char character, float x1, float y1, float x2, float y2)
{
    float offX = (x1 + x2) / 2;
    float scaleX = (x2 - x1) / 20;

    float offY = (y1 + y2) / 2;
    float scaleY = (y2 - y1) / 20;

    float* strokeVerts = (float*)malloc(3 * MAX_CHAR_VERTS * sizeof(float));
    if (strokeVerts == 0) {
        return;
    }
    int nVerts = 0;
    int offset = 0;
    bool swap = 0;
    int flipX = 0;
    int flipY = 0;
    if (character >= 0x30 && character <= 0x39) {
        // handle numbers
    } else {
        character -= 0x41;
        if (character >= 0x20) character -= 0x20;

        if (character < 0 || character > 25) goto DONE;

        nVerts = vertCounts[character];
        offset = vertOffsets[character];
        swap = vertFlipsSwaps[character] & 0b100;
        flipX = 1 - (vertFlipsSwaps[character] & 0b010);
        flipY = 1 - ((vertFlipsSwaps[character] & 0b001) << 1);

        if (nVerts > 0) {
            for (int i = 0; i < nVerts; i++) {
                strokeVerts[i * 3] = offX + scaleX * flipX * (swap ? characterVerts[(i + offset) * 2 + 1] : characterVerts[(i + offset) * 2]);
                strokeVerts[i * 3 + 1] = offY + scaleY * flipY * (swap ? characterVerts[(i + offset) * 2] : characterVerts[(i + offset) * 2 + 1]);
                strokeVerts[i * 3 + 2] = 0;
            }

            strokeToCycle2D(strokeVerts, nVerts, buffer, samples);
        }
    }

DONE:
    free(strokeVerts);
    return;
}