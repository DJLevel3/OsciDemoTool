#pragma once

/****************************************
* Here you can find configuration for   *
* various aspects of the demo. Options  *
* called SECONDS are actually measured  *
* in seconds at 120 BPM (AKA half notes)*
* so it'll be shorter at higher BPMs.   *
*****************************************/

#define FILE_RATE 192000 // Actual sample rate. Changing this changes stuff a lot, including how the drums sound
#define BPM 120.f // BPM. I think you should understand this

// Don't touch these, they adjust the pitches and sample rates to match the BPM
#define SPEED_FACTOR (120.f / BPM)
#define SAMPLE_RATE (int(SPEED_FACTOR * FILE_RATE))

#define DEMO_SECTIONS 9 // How many sections to put in the demo

#define INTRO_SECTION 0
#define INTRO_SECONDS 8

#define GREETZ_SECTION 1
#define GREETZ_SECONDS 28

#define TEXT_SECTION 2
#define TEXT_SECONDS 20

#define CUBE_SECTION 3
#define CUBE_SECONDS 32

#define PLASMA_SECTION 4
#define PLASMA_SECONDS 12

#define BALL_SECTION 5
#define BALL_SECONDS 64

#define MID_SECTION 6
#define MID_SECONDS 8

#define PLASMA2_SECTION 7
#define PLASMA2_SECONDS 8

#define OUTRO_SECTION 8
#define OUTRO_SECONDS 8

#define AUDIO_SECONDS (INTRO_SECONDS + GREETZ_SECONDS + TEXT_SECONDS + CUBE_SECONDS + PLASMA_SECONDS + BALL_SECONDS + MID_SECONDS + PLASMA2_SECONDS + OUTRO_SECONDS)

#define BORDER_SAMPLES 800
 
//#define NOISE_HIHAT // Use noise for the hihat instead of square waves
