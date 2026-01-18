#pragma once
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

ma_engine audioEngine;

void audioInit()
{
    ma_engine_init(NULL, &audioEngine);
}

void audioPlay(const char* file)
{
    ma_engine_play_sound(&audioEngine, file, NULL);
}