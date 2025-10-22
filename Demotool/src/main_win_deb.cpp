//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#define XRES        800
#define YRES        600

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include <stdio.h>
#include "intro.h"
#include "tool.h"

//----------------------------------------------------------------------------


#ifdef __cplusplus
extern "C"
{
#endif
    int  _fltused = 0;
#ifdef __cplusplus
}
#endif


static const int wavHeader[11] = {
    0x46464952, 
    MZK_NUMSAMPLES*2+36, 
    0x45564157, 
    0x20746D66, 
    16, 
    WAVE_FORMAT_PCM|(MZK_NUMCHANNELS<<16), 
    MZK_RATE, 
    MZK_RATE*MZK_NUMCHANNELS*sizeof(short), 
    (MZK_NUMCHANNELS*sizeof(short))|((8*sizeof(short))<<16),
    0x61746164, 
    MZK_NUMSAMPLES*sizeof(short)
    };

//==============================================================================================




//==============================================================================================

static short introtune[INTROMUSIC_NUMSAMPLESC + 22];

//==============================================================================================

void entrypoint(void)
{
    int done = 0;

    intro_init();

    mzk_init( introtune+22 );

    memcpy( introtune, wavHeader, 44 );
    sndPlaySound((const char*)&introtune, SND_ASYNC | SND_MEMORY);

    long to=timeGetTime();
    while (!GetAsyncKeyState(VK_ESCAPE) && !done)
    {
        long t = timeGetTime() - to;


        intro_do(t);

        if (t > (1000)) {
            sndPlaySound((const char*)&introtune, SND_ASYNC | SND_MEMORY);;
            to = timeGetTime();
        }
    }
    ExitProcess(0);
}



