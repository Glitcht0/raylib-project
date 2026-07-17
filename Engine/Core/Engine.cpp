#include "Engine.h"

#include <raylib.h>


static StateMachine machine;

StateMachine& GetStateMachine(){
    return machine;
}

void EngineRun(){
    while (!WindowShouldClose() && machine.IsRunning()){
        machine.Update();
        machine.Draw();

        if (IsKeyPressed(KEY_F11) ||
            (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER)))
        {
            ToggleFullscreen();
        }
    }

    CloseWindow();
}

