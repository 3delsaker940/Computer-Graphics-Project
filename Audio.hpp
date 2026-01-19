#pragma once
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

static ma_engine audioEngine;

static ma_sound windSound;
static bool windLoaded = false;
static bool windPlaying = false;

static ma_sound musicSound;
static bool musicLoaded = false;
static bool musicPlaying = false;

inline void audioInit()
{
    ma_engine_init(NULL, &audioEngine);

        if (ma_sound_init_from_file(&audioEngine,
        "assets/sounds/wind.mp3",              MA_SOUND_FLAG_STREAM,
        NULL, NULL,
        &windSound) == MA_SUCCESS)
    {
        ma_sound_set_looping(&windSound, MA_TRUE);
        ma_sound_set_volume(&windSound, 0.5f);
        windLoaded = true;
    }

        if (ma_sound_init_from_file(&audioEngine,
        "assets/sounds/music.mp3",             MA_SOUND_FLAG_STREAM,
        NULL, NULL,
        &musicSound) == MA_SUCCESS)
    {
        ma_sound_set_looping(&musicSound, MA_TRUE);
        ma_sound_set_volume(&musicSound, 0.4f);
        musicLoaded = true;
    }
}

inline void audioPlay(const char* file)
{
    ma_engine_play_sound(&audioEngine, file, NULL);
}

inline void audioOutdoorStart()
{
    if (windLoaded && !windPlaying)
    {
        ma_sound_start(&windSound);
        windPlaying = true;
    }
}

inline void audioOutdoorStop()
{
    if (windLoaded && windPlaying)
    {
        ma_sound_stop(&windSound);
        windPlaying = false;
    }
}

inline void audioIndoorStart()
{
    if (musicLoaded && !musicPlaying)
    {
        ma_sound_start(&musicSound);
        musicPlaying = true;
    }
}

inline void audioIndoorStop()
{
    if (musicLoaded && musicPlaying)
    {
        ma_sound_stop(&musicSound);
        musicPlaying = false;
    }
}