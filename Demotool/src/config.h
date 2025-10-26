#pragma once

#define SAMPLE_RATE  96000 // Sample rate to use

#define DEMO_DURATION 64 // Duration in seconds

//#define LISSAJOUS_INTRO // Use a lissajous pattern while the demo calculates (roughly 100-150 bytes)

//#define ROTATION_SMOOTH // Rotate at constant rate instead of frequency dependent (roughly 100 bytes)

#ifdef ROTATION_SMOOTH
#define ROTATION_SAMPLES 1200 // Samples per rotation increment
#endif

//#define SAMPLED_KICK // Use a sampled kick drum (roughly 1000 bytes)
//#define SAMPLED_SNARE // Use a sampled snare drum (roughly 1000 bytes)
//#define NOISE_HIHAT // Use noise for the hihat instead of square waves

//#define OCTA // Use an octahedron as well as a cube
