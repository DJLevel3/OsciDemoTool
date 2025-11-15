//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008/2021                                   //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <mmsystem.h>

#include "config.h"
#include "demo.h"

#ifdef SAVE_FILE
#include <cstdio>
#include <cstdlib>
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    int  _fltused = 0;
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

HWAVEOUT wave_out;
#define CHUNK_SIZE 8192
WAVEHDR header[2] = { 0 };
short chunks[2][CHUNK_SIZE * 2];
bool chunk_swap = false;
long fileCounter;

void CALLBACK WaveOutProc(HWAVEOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);

#ifdef SAVE_FILE
static const unsigned int dotWavHeader[11] = {
    0x46464952,
    SAMPLE_RATE * AUDIO_SECONDS * 2 * 2 + 36,
    0x45564157,
    0x20746D66,
    16,
    WAVE_FORMAT_PCM | (2 << 16),
    FILE_RATE,
    FILE_RATE * 2 * sizeof(short),
    (2 * sizeof(short)) | ((8 * sizeof(short)) << 16),
    0x61746164,
    SAMPLE_RATE * 2 * AUDIO_SECONDS * sizeof(short)
};
#endif

short* demoAudioA;
long writePointer;

bool safe = true;
void entrypoint(void)
{
    demoAudioA = (short*)malloc((SAMPLE_RATE * 2 * AUDIO_SECONDS + 22) * sizeof(short));
    if (demoAudioA == 0) return;
    memset(demoAudioA, 0, SAMPLE_RATE * AUDIO_SECONDS * 2 + 44);

    fileCounter = 22;

    WAVEFORMATEX format{};
    format.wFormatTag = WAVE_FORMAT_PCM,
    format.nChannels = 2;
    format.nSamplesPerSec = FILE_RATE;
    format.wBitsPerSample = 16;
    format.cbSize = 0;
    format.nBlockAlign = format.nChannels * format.wBitsPerSample / 8;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

    /*
    int devices = waveOutGetNumDevs();
    WAVEOUTCAPS caps;
    for (int i = 0; i < devices; i++) {
        waveOutGetDevCaps(i, &caps, 52);
        printf("%d ", i);
        if (caps.dwFormats & WAVE_FORMAT_96S16) printf(caps.szPname);
        printf("\n");
    }
    */

    if (waveOutOpen(&wave_out, -1, &format, (DWORD_PTR)WaveOutProc, (DWORD_PTR)NULL, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) {
        ExitProcess(-1);
    }

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
#ifdef SAVE_FILE
    memcpy(demoAudioA, dotWavHeader, 44);
#endif
    writePointer = 22;

    st = timeGetTime();
    while (section < DEMO_SECTIONS && !done) {
        DEMO_NUMSAMPLESC = f2i((demo_length(section)) * SAMPLE_RATE * 2);

        demoBuffer = (short*)malloc((SAMPLE_RATE * 2 + DEMO_NUMSAMPLESC) * sizeof(short));
        if (demoBuffer != 0) {
            t = timeGetTime();
            done = !demo_do(t, demoBuffer, section);
            memcpy(demoAudioA + writePointer, demoBuffer, DEMO_NUMSAMPLESC * sizeof(short));
            writePointer += DEMO_NUMSAMPLESC;
            free(demoBuffer);
        }
        if (section == 0) {
            for (int i = 0; i < 2; ++i) {
                memset(chunks[chunk_swap], 0, 2 * CHUNK_SIZE * sizeof(short));
                header[i].lpData = (CHAR*)chunks[i];
                header[i].dwBufferLength = CHUNK_SIZE * 2 * 2;
                if (waveOutPrepareHeader(wave_out, &header[i], sizeof(header[i])) != MMSYSERR_NOERROR) {
                    free(demoAudioA);
                    ExitProcess(-2);
                }
                if (waveOutWrite(wave_out, &header[i], sizeof(header[i])) != MMSYSERR_NOERROR) {
                    free(demoAudioA);
                    ExitProcess(-3);
                }
            }
        }
        section++;
        done = done || GetAsyncKeyState(VK_ESCAPE);
    }

    demo_end();

#ifdef SAVE_FILE
    FILE* oFile = fopen("demo.wav", "wb");
    if (oFile != NULL) {
        fwrite(demoAudioA, sizeof(short), SAMPLE_RATE * 2 * AUDIO_SECONDS + 22, oFile);
        fclose(oFile);
    }
#endif

    while (!GetAsyncKeyState(VK_ESCAPE) && timeGetTime() < st + 1000 * AUDIO_SECONDS * SPEED_FACTOR + 500) {
        Sleep(10);
    }

    waveOutUnprepareHeader(wave_out, &header[0], sizeof(header[0]));
    waveOutUnprepareHeader(wave_out, &header[0], sizeof(header[1]));

    safe = false;
    free(demoAudioA);

    ExitProcess(0);
}

void CALLBACK WaveOutProc(HWAVEOUT wave_out_handle, UINT message, DWORD_PTR instance, DWORD_PTR param1, DWORD_PTR param2) {
    switch (message) {
    case WOM_DONE: {
        for (int i = 0; i < CHUNK_SIZE; ++i) {
            if (safe) {
                chunks[chunk_swap][i * 2] = demoAudioA[fileCounter * 2];
                chunks[chunk_swap][i * 2 + 1] = demoAudioA[fileCounter * 2 + 1];
                if (fileCounter < SAMPLE_RATE * AUDIO_SECONDS - 1) fileCounter++;
            }
        }
        if (waveOutWrite(wave_out, &header[chunk_swap], sizeof(header[chunk_swap])) != MMSYSERR_NOERROR) {
#ifdef _DEBUG
#ifdef SAVE_FILE
            printf("Wave Out Write Failed - Something's Fucked :3\n");
#endif
#endif
        }
        chunk_swap = !chunk_swap;
    } break;
    }
}