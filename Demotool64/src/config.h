#pragma once

/****************************************
* Here you can find configuration for   *
* various aspects of the demo. Options  *
* called SECONDS are actually measured  *
* in seconds at 120 BPM (AKA half notes)*
* so it'll be shorter at higher BPMs.   *
*****************************************/

#define FILE_RATE 192000 // Actual sample rate. Changing this changes stuff a lot, including how the drums sound
#define BPM 120 // BPM. I think you should understand this

// Don't touch these, they adjust the pitches and sample rates to match the BPM
#define SPEED_FACTOR 1
#define SAMPLE_RATE FILE_RATE

#define DEMO_SECTIONS 2 // How many sections to put in the demo

#define INTRO_SECTION  0
#define INTRO_SECONDS 8

#define TWISTER_SECTION  1
#define TWISTER_SECONDS 65 // Duration in seconds

#define AUDIO_SECONDS (INTRO_SECONDS + TWISTER_SECONDS)

#define BORDER_SAMPLES 800
 
#define NOISE_HIHAT // Use noise for the hihat instead of square waves
