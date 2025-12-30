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
//#define LARGURA_TELA 1920
//#define ALTURA_TELA 1080

enum class GameMode {
    EDIT,
    MOVE,
    GAME,
    ROTATE
};

#define COR_MESA (Color){ 45, 45, 45, 255 }
#define COR_INPUT (Color){ 84, 84, 84, 255 }