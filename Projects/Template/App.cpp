
#include "Engine/Assets/FontManager.h"
#include "Projects/Template/States/States.h"
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

/** ======================================================
    📝 Templete da Função Principal do Programa

====================================================== */ 
int main2(){
    
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE); 

    I18N_Load_File("Projects/Template/Assets/lang/pt.json"); // Arquivo de textos
    
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Raylib 3D - RoboCup");
    SetExitKey(KEY_NULL);

    SetTargetFPS(60);
    GuiLoadStyleAmber();

    Font uiFont = LoadFont();
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR); // Suaviza a fonte
    GuiSetFont(uiFont);

    

    GetStateMachine().ChangeState(new State1()); // Muda para o estado de menu inicial

    EngineRun(); // Inicia o loop principal da Engine
    return 0;
}

       


