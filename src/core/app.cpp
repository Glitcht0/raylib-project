#include "src/core/state_machine.h"
#include "raylib.h"
#include "src/Game/state_game.h"
#include "src/engine/i18n/i18n.h"

#define RLIGHTS_IMPLEMENTATION
#include "libs/rlights.h"


#define RAYGUI_IMPLEMENTATION
#include "libs/raygui.h"
#include "resources/style/style_amber.h"




void principal_loop(){
    appstate currentState = STATE_GAME;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_VSYNC_HINT);

    I18N_Load("resources/lang/pt.json");


  


    InitWindow(LARGURA_TELA, ALTURA_TELA, "Raylib 3D - Exemplo simples");
    SetTargetFPS(60);
    GuiLoadStyleAmber();

    Font uiFont = Carregarfonte();
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR); // Suaviza a fonte
    GuiSetFont(uiFont);

    

    
    

    StateGame gameState("mundao1");
    
    bool stateEntered = false; // flag para controlar onEnter

    while (!WindowShouldClose() && currentState != STATE_EXIT){
        switch (currentState)
        {
            case STATE_MENU:
                BeginDrawing();
                ClearBackground(BLACK);

                mensagemBox(&currentState);

                EndDrawing();

            
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







void mensagemBox(appstate *currentState){
    
    float boxWidth = 350, boxHeight = 200;

    Rectangle box = { (LARGURA_TELA - boxWidth) / 2.0f , (ALTURA_TELA  - boxHeight) / 2.0f, boxWidth, boxHeight };

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

     
    int result = GuiMessageBox( box, T("MSG_TITLE"),T("MSG_BODY"), T("BTN_AVISO") );

    if (result >= 0)
        *currentState = STATE_GAME;
}
