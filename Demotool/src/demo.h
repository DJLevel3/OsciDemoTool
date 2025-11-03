//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#ifndef _INTRO_H_
#define _INTRO_H_

#ifndef SAVE_FILE
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#endif
#include "config.h"
#include "system.h"
#include "tool.h"

#ifdef SAMPLED_SNARE
#include "snare.h"
#else
#define SNARE_RATE 96000
#define SNARE_SAMPLES 12000

#define SNARE_FMAX 330
#define SNARE_FBASE 110
#define SNARE_FSTEP 14

#define SNARE_WIREWAIT 2000
#define SNARE_WIRETIME 10000
#endif

#ifdef SAMPLED_KICK
#include "kick_l.h"
#else
#define KICK_RATE 96000
#define KICK_SAMPLES 12000
#define KICK_FMAX 165
#define KICK_FBASE 55
#define KICK_FSTEP 20

#define KICK_START_V p0d50
#define KICK_FADE_START 1000

#define KICK_MID_V p0d40
#define KICK_FADE_MID 3000

#define KICK_END_V p0d10
#define KICK_FADE_END KICK_SAMPLES
#endif

int   demo_init( int itime );
float demo_length( int section );
void  demo_do( long time, short * buffer, int section );
void  demo_end( void );

#endif
