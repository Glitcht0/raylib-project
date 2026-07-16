
#include "Engine/Assets/FontManager.h"
#include "Projects/TheGame/States/States.h"
#include "Projects/TheGame/States/StateMenu/StateMenu.h"
#include "Engine/Core/Engine.h"
#include "config.h"
#include "raylib.h"
#define RLIGHTS_IMPLEMENTATION
#include "libs/rlights.h"


#define RAYGUI_IMPLEMENTATION
#include "libs/raygui.h"
#include "resources/style/style_amber.h"
#include "Engine/Assets/FontManager.h"

#include "Engine/i18n/i18n.h"

void mensagemBox(){
    float Largura_Tela = GetRenderWidth();
    float Altura_Tela = GetRenderHeight();

    BeginDrawing();
    ClearBackground(BLACK);

    float boxWidth = 350, boxHeight = 200;
    Rectangle box = {  (Largura_Tela - boxWidth) / 2.0f, (Altura_Tela  - boxHeight) / 2.0f, boxWidth, boxHeight  };

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    int result = GuiMessageBox(box, T("MSG_TITLE"), T("MSG_BODY"), T("BTN_AVISO"));

    EndDrawing();

    if (result >= 0)
        GetStateMachine().ChangeState(new StateMenu()); //STATE_MENU;
}

/** ======================================================
    

====================================================== */ 
int main(){
    
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE); 

    I18N_Load_File("Projects/TheGame/Assets/lang/pt.json"); // Arquivo de textos
    
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Raylib 3D - The Game");
    SetExitKey(KEY_NULL);

    SetTargetFPS(60);
    GuiLoadStyleAmber();

    Font uiFont = LoadFont();
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR); // Suaviza a fonte
    GuiSetFont(uiFont);

    mensagemBox();

    GetStateMachine().ChangeState(new StateMenu()); // Muda para o estado de menu inicial

    EngineRun(); // Inicia o loop principal da Engine
    return 0;
}

       


