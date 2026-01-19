#pragma once
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

// محرك الصوت
static ma_engine audioEngine;

// ✅ صوت الرياح (خارج المعرض)
static ma_sound windSound;
static bool windLoaded = false;
static bool windPlaying = false;

// ✅ موسيقى داخل المعرض
static ma_sound musicSound;
static bool musicLoaded = false;
static bool musicPlaying = false;

// تهيئة الصوت + تحميل الملفات
inline void audioInit()
{
    ma_engine_init(NULL, &audioEngine);

    // حمّل صوت الرياح من assets/wind.wav
    if (ma_sound_init_from_file(&audioEngine,
        "assets/sounds/wind.mp3",      // غيّر المسار إذا لازم
        MA_SOUND_FLAG_STREAM,
        NULL, NULL,
        &windSound) == MA_SUCCESS)
    {
        ma_sound_set_looping(&windSound, MA_TRUE);
        ma_sound_set_volume(&windSound, 0.5f);
        windLoaded = true;
    }

    // حمّل موسيقى الداخل من assets/music.mp3
    if (ma_sound_init_from_file(&audioEngine,
        "assets/sounds/music.mp3",     // غيّر المسار إذا لازم
        MA_SOUND_FLAG_STREAM,
        NULL, NULL,
        &musicSound) == MA_SUCCESS)
    {
        ma_sound_set_looping(&musicSound, MA_TRUE);
        ma_sound_set_volume(&musicSound, 0.4f);
        musicLoaded = true;
    }
}

// تشغيل صوت لمرة واحدة (لأبواب، كبسات، الخ)
inline void audioPlay(const char* file)
{
    ma_engine_play_sound(&audioEngine, file, NULL);
}

// تشغيل صوت الرياح (خارج المعرض)
inline void audioOutdoorStart()
{
    if (windLoaded && !windPlaying)
    {
        ma_sound_start(&windSound);
        windPlaying = true;
    }
}

// إيقاف صوت الرياح
inline void audioOutdoorStop()
{
    if (windLoaded && windPlaying)
    {
        ma_sound_stop(&windSound);
        windPlaying = false;
    }
}

// تشغيل موسيقى الداخل (داخل المعرض)
inline void audioIndoorStart()
{
    if (musicLoaded && !musicPlaying)
    {
        ma_sound_start(&musicSound);
        musicPlaying = true;
    }
}

// إيقاف موسيقى الداخل
inline void audioIndoorStop()
{
    if (musicLoaded && musicPlaying)
    {
        ma_sound_stop(&musicSound);
        musicPlaying = false;
    }
}