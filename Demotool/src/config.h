#pragma once

#define FILE_RATE 96000
#define BPM 128.f
#define SAMPLE_RATE ((int)(120.f / BPM * FILE_RATE)) // Sample rate to use

#define PRECALC_SECTION  0
#define PRECALC_DURATION 2

#define CUBE_SECTION  1
#define CUBE_DURATION 32

#define TWISTER_SECTION  2
#define TWISTER_DURATION 66 // Duration in seconds

#define PLASMA_SECTION  3
#define PLASMA_DURATION 32

#define AUDIO_SECONDS (PRECALC_DURATION + TWISTER_DURATION + CUBE_DURATION)
#define DEMO_SECTIONS 3

//#define LISSAJOUS_INTRO // Use a lissajous pattern while the demo calculates (roughly 100-150 bytes)

//#define ROTATION_SMOOTH // Rotate at constant rate instead of frequency dependent (roughly 100 bytes)
 
#ifdef ROTATION_SMOOTH
#define ROTATION_SAMPLES 1200 // Samples per rotation increment
#endif

//#define SAMPLED_KICK // Use a sampled kick drum (roughly 1000 bytes)
//#define SAMPLED_SNARE // Use a sampled snare drum (roughly 1000 bytes)
//#define NOISE_HIHAT // Use noise for the hihat instead of square waves

//#define SPIRAL // make the column a spiral