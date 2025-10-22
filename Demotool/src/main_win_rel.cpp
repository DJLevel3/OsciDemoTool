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

#include "intro.h"
#include "tool.h"

#ifdef SAVE_FILE
#include "AudioFile.h"
#endif

static int wavHeader[11] = {
    0x46464952, 
    INTROMUSIC_NUMSAMPLES*2+36, 
    0x45564157, 
    0x20746D66, 
    16, 
    WAVE_FORMAT_PCM|(MZK_NUMCHANNELS<<16), 
    MZK_RATE, 
    MZK_RATE*MZK_NUMCHANNELS*sizeof(short), 
    (MZK_NUMCHANNELS*sizeof(short))|((8*sizeof(short))<<16),
    0x61746164, 
    INTROMUSIC_NUMSAMPLES*sizeof(short)
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
    short* introtune = new short[INTROMUSIC_NUMSAMPLESC + 22];
    short* demoAudio = new short[DEMO_NUMSAMPLESC + 22];
    // init intro
    if (!intro_init()) return;

    // init mzk
    mzk_init( introtune+22 );
    memcpy( introtune, wavHeader, 44 );

    wavHeader[1] = DEMO_NUMSAMPLESC + 36;
    wavHeader[10] = DEMO_NUMSAMPLESC * sizeof(short);
    memcpy(demoAudio, wavHeader, 44);

    // play loading mzk
    sndPlaySound( (const char*)introtune, SND_ASYNC | SND_MEMORY | SND_LOOP );

    // build the intro
    long t = timeGetTime();
    intro_do(t, demoAudio + 22);

    sndPlaySound(0, 0);
    Sleep(1000);

#ifdef SAVE_FILE
    AudioFile<short> audio;
    audio.setAudioBufferSize(2, DEMO_NUMSAMPLES);
    audio.setBitDepth(16);
    audio.setSampleRate(MZK_RATE);
    for (int i = 0; i < DEMO_NUMSAMPLES; i++) {
        audio.samples[0][i] = demoAudio[2 * i];
        audio.samples[1][i] = demoAudio[2 * i + 1];
    }
    audio.save("demo.wav");
#endif

    // start intro
    sndPlaySound((const char*)demoAudio, SND_ASYNC | SND_MEMORY);

    t = timeGetTime();
    // wait for key press
    while (!GetAsyncKeyState(VK_ESCAPE) && timeGetTime() < t + (1000 * DEMO_DURATION) + 1000) Sleep(50);
    sndPlaySound(0, 0);


    intro_end();

    delete[] introtune;
    delete[] demoAudio;

    ExitProcess(0);
}
