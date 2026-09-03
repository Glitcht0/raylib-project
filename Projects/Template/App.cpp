
#include "Engine/Assets/FontManager.h"
#include "Projects/Template/States/States.h"
#include "Engine/Core/Engine.h"
#include "config.h"
#include "raylib.h"
#define RLIGHTS_IMPLEMENTATION
#include "libs/rlights.h"


#define RAYGUI_IMPLEMENTATION
#include "libs/raygui.h"
#include "Engine/resources/style/style_amber.h"
#include "Engine/Assets/FontManager.h"

#include "Engine/i18n/i18n.h"

/** ======================================================
    📝 Templete da Função Principal do Programa

====================================================== */ 
int main(){ // Renomeado de main2 para main
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE); 
    I18N_Load_File("Projects/Template/Assets/lang/pt.json"); 
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Raylib 3D - Template");
    SetTargetFPS(60);
    GuiLoadStyleAmber();

    Font uiFont = LoadFont();
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR); // Suaviza a fonte
    GuiSetFont(uiFont);
    

    // ⚙️ TUTORIAL: REGISTRO DE ESTADOS (A FÁBRICA)
    // Antes de rodar, ensinamos a Engine a criar as telas do jogo.
    StateMachine& sm = GetStateMachine();
    sm.RegisterState<State1>(StateID::State1);
    sm.RegisterState<State2>(StateID::State2);

    // 🎮 Inicia o jogo na tela principal
    sm.ChangeState(StateID::State1);

    // 🚂 Roda o Loop da Engine (Delega o controle)
    EngineRun(); 

    return 0;
}

       


