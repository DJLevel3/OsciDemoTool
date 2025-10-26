//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

//#define USEDSOUND
//#define CLEANDESTROY          // destroy stuff (windows, glContext, ...)


#ifndef SAVE_FILE
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#endif
#include <windows.h>
#include <mmsystem.h>

#include "config.h"
#include "demo.h"
#include "tool.h"

#ifdef SAVE_FILE
#include <cstdio>
#include <cstdlib>
#endif

static int wavHeader[11] = {
    0x46464952,
#ifdef LISSAJOUS_INTRO
    INTROMUSIC_NUMSAMPLESC*2+36, 
#else
    DEMO_NUMSAMPLESC*2+36,
#endif
    0x45564157,
    0x20746D66,
    16,
    WAVE_FORMAT_PCM|(2<<16),
    SAMPLE_RATE,
    SAMPLE_RATE*2*sizeof(short),
    (2*sizeof(short))|((8*sizeof(short))<<16),
    0x61746164,
#ifdef LISSAJOUS_INTRO
    INTROMUSIC_NUMSAMPLESC*sizeof(short)
#else
    DEMO_NUMSAMPLESC*sizeof(short)
#endif
};

#ifdef __cplusplus
extern "C" 
{
#endif
int  _fltused = 0;
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

void entrypoint( void )
{
#ifdef LISSAJOUS_INTRO
    short* introtune = new short[INTROMUSIC_NUMSAMPLESC + 22];

    // init loading tone
    mzk_init( introtune+22 );
    memcpy( introtune, wavHeader, 44 );

    // play loading tone
    sndPlaySound( (const char*)introtune, SND_ASYNC | SND_MEMORY | SND_LOOP );

    // switch header info to demo
    wavHeader[1] = DEMO_NUMSAMPLESC + 36;
    wavHeader[10] = DEMO_NUMSAMPLESC * sizeof(short);
#endif
    short* demoAudio = new short[DEMO_NUMSAMPLESC + 22];
    memcpy(demoAudio, wavHeader, 44);

    // init and build the demo
    if (!demo_init()) return;
    long t = timeGetTime();
    demo_do(t, demoAudio + 22);

#ifdef LISSAJOUS_INTRO
    sndPlaySound(0, 0);
    Sleep(1000);
#endif

#ifdef SAVE_FILE
    FILE* oFile = fopen("demo.wav", "wb");
    fwrite(demoAudio, sizeof(short), DEMO_NUMSAMPLESC + 22, oFile);
    fclose(oFile);
#else
    // start intro
    sndPlaySound((const char*)demoAudio, SND_ASYNC | SND_MEMORY);

    t = timeGetTime();
    // wait for key press
    while (!GetAsyncKeyState(VK_ESCAPE) && timeGetTime() < t + (1000 * DEMO_DURATION) + 1000) Sleep(50);
    sndPlaySound(0, 0);
#endif
    demo_end();

#ifdef LISSAJOUS_INTRO
    delete[] introtune;
#endif
    delete[] demoAudio;

    ExitProcess(0);
}
