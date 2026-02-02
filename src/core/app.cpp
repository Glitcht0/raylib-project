#include "src/core/state_machine.h"
#include "raylib.h"
#include "src/Game/state_game.h"
#include "src/Menu/state_menu.h"
#include "src/engine/i18n/i18n.h"

#define RLIGHTS_IMPLEMENTATION
#include "libs/rlights.h"


#define RAYGUI_IMPLEMENTATION
#include "libs/raygui.h"
#include "resources/style/style_amber.h"




void principal_loop(){
    appstate currentState = STATE_MENU;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_VSYNC_HINT);

    I18N_Load("resources/lang/pt.json");


  


    InitWindow(LARGURA_TELA, ALTURA_TELA, "Raylib 3D - Exemplo simples");
    SetExitKey(KEY_NULL);


    SetTargetFPS(60);
    GuiLoadStyleAmber();

    Font uiFont = Carregarfonte();
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR); // Suaviza a fonte
    GuiSetFont(uiFont);

    

    
    
    std::string nomeMundo;
    StateGame *gameState = nullptr;
    StateMenu menuState;

    
    bool stateEntered = false; // flag para controlar onEnter

    while (!WindowShouldClose() && currentState != STATE_EXIT){
        switch (currentState)
        {
            case STATE_MENU:



                if (!stateEntered) {
                    menuState.onEnter(); // entra no estado apenas uma vez
                    stateEntered = true;
                }

                menuState.update(&currentState);
                menuState.draw();

                if (currentState != STATE_MENU) { // saiu do estado
                    nomeMundo = menuState.GetNomeMundo();
                    menuState.onExit();
                    stateEntered = false;
                }
                break;

            
            

            case STATE_CONFIG:
                
                break;

            case STATE_GAME:
                if (!stateEntered) {
                    if(!gameState){
                        gameState = new StateGame(nomeMundo);
                    }
                    gameState->onEnter(); // entra no estado apenas uma vez
                    stateEntered = true;
                }

                gameState->update(&currentState);
                gameState->draw();

                if (currentState != STATE_GAME) { // saiu do estado
                    gameState->onExit();
                    delete gameState;
                    gameState = nullptr;
                    stateEntered = false;
                }
                break;

            default:
                break;
        }

    }


    CloseWindow();

}







