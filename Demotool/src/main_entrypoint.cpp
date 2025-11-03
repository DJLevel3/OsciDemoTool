//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <mmsystem.h>

#include "system.h"
#include "config.h"
#include "demo.h"
#include "tool.h"

//#define SAVE_FILE
#ifdef SAVE_FILE
#include <cstdio>
#include <cstdlib>
#endif

static int wavHeader[11] = {
    0x46464952,
#ifdef LISSAJOUS_INTRO
    INTROMUSIC_NUMSAMPLESC * 2 + 36,
#else
    SAMPLE_RATE * 2 * AUDIO_SECONDS + 36,
#endif
    0x45564157,
    0x20746D66,
    16,
    WAVE_FORMAT_PCM | (2 << 16),
    FILE_RATE,
    FILE_RATE * 2 * sizeof(short),
    (2 * sizeof(short)) | ((8 * sizeof(short)) << 16),
    0x61746164,
#ifdef LISSAJOUS_INTRO
    INTROMUSIC_NUMSAMPLESC * sizeof(short)
#else
    SAMPLE_RATE * 2 * AUDIO_SECONDS * sizeof(short)
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

short* demoAudioA;
long writePointer;

void entrypoint( void )
{
    demoAudioA = new short[SAMPLE_RATE * 2 * AUDIO_SECONDS + 22];
    memset(demoAudioA + 22, 0, SAMPLE_RATE * 2 * AUDIO_SECONDS * sizeof(short));

    float DEMO_DURATION;
    int DEMO_NUMSAMPLESC;

    bool done = 0;
    long t = timeGetTime();
    long st = 0;
    // init and build the demo
    if (!demo_init(t)) return;
    int section = 0;
    
    short* demoBuffer;

    done = GetAsyncKeyState(VK_ESCAPE);

    memcpy(demoAudioA, wavHeader, 44);
    writePointer = 22;

    while (section < DEMO_SECTIONS && !done) {
        DEMO_DURATION = demo_length(section);
        DEMO_NUMSAMPLESC = DEMO_DURATION * SAMPLE_RATE * 2;

        demoBuffer = new short[DEMO_NUMSAMPLESC];

        t = timeGetTime();
        demo_do(t, demoBuffer, section);
        memcpy(demoAudioA + writePointer, demoBuffer, DEMO_NUMSAMPLESC * sizeof(short));
        writePointer += DEMO_NUMSAMPLESC;

        if (section == 0) {
            st = timeGetTime();
            sndPlaySound((const char*)demoAudioA, SND_ASYNC | SND_MEMORY);
        }
        section++;

        delete[] demoBuffer;
        done = GetAsyncKeyState(VK_ESCAPE);
    }

    demo_end();

#ifdef SAVE_FILE
    FILE* oFile = fopen("demo.wav", "wb");
    fwrite(demoAudioA, sizeof(short), SAMPLE_RATE * 2 * AUDIO_SECONDS + 22, oFile);
    fclose(oFile);
#endif

    while (!done && timeGetTime() < st + 1000 * AUDIO_SECONDS + 500) {
        Sleep(10);
        done = GetAsyncKeyState(VK_ESCAPE);
    }

    sndPlaySound(0, 0);

    delete[] demoAudioA;

    ExitProcess(0);
}
