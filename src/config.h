#pragma once
#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define MAX_LIGHTS 4
#define LARGURA_TELA 1280
#define ALTURA_TELA 720