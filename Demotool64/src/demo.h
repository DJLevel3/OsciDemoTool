//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//
#pragma once

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include "config.h"
#include "tool.h"
#include "system.h"

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
#define KICK_FMAX 330
#define KICK_FBASE 55
#define KICK_FSTEP 24

#define KICK_START_V p0d60
#define KICK_FADE_START 2000

#define KICK_END_V p0d10
#endif

int   demo_init( int itime );
void  demo_end( void );
void intro(short* buffer);

void twister(short* buffer);
