#pragma once
#include "src/state_machine/state.h"
#include "src/state_machine/state_machine.h"
#include "raylib.h"
#include "raymath.h"
#include "src/config.h"

#include "src/rlights.h"

#define FUNDO (Color){ 63, 63, 63, 255 }
#define COR_GRID (Color){ 80, 80, 80, 255 }
#define COR_GRID2 (Color){ 95, 95, 95, 255 }


class StateGame : public State {
public:

    StateGame();
    void onEnter() override;
    void onExit() override;

    void update(appstate* currentState) override;
    void draw() override;

private:
    Camera3D camera = { 0 };

    float yaw   = 0.0f; //rotação horizontal
    float pitch = 0.0f; //rotação vertical
    float distance = 6.0f;
    Shader shader;
    int ambientLoc;
    Light lights[MAX_LIGHTS] = { 0 };

    void DrawGridXZ(int size, float step);

};
