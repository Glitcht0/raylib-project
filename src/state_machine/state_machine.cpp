#include "state_machine.h"
#include "raylib.h"
#include "src/Game/state_game.h"






void principal_loop(){
    appstate currentState = STATE_GAME;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(800, 600, "Raylib 3D - Exemplo simples");
    SetTargetFPS(60);

    
    

    StateGame gameState;
    
    bool stateEntered = false; // flag para controlar onEnter

    while (!WindowShouldClose() && currentState != STATE_EXIT){
        switch (currentState)
        {
            case STATE_MENU:
            
                break;

            case STATE_CONFIG:
                
                break;

            case STATE_GAME:
                if (!stateEntered) {
                    gameState.onEnter(); // entra no estado apenas uma vez
                    stateEntered = true;
                }

                gameState.update(&currentState);
                gameState.draw();

                if (currentState != STATE_GAME) { // saiu do estado
                    gameState.onExit();
                    stateEntered = false;
                }
                break;

            default:
                break;
        }
    }
    CloseWindow();

}