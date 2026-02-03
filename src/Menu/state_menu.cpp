#include "state_menu.h"



StateMenu::StateMenu(){

}

StateMenu::~StateMenu(){

}

void StateMenu::onEnter(){

}

void StateMenu::onExit(){

}


void StateMenu::update(appstate* currentState){
    estadoAtual = currentState;
}

void StateMenu::draw(){
    BeginDrawing();
    ClearBackground(BLACK);

    
    
    if (EstadoMenu==MENU){
        DrawButtonsMenu();
        /*
        DrawButtonsDemo();
        DrawInputsDemo();
        DrawListsDemo();
        DrawSlidersDemo();
        DrawWindowsDemo();*/
        
    }

    if (EstadoMenu == CARREGAR){
        DrawMundosLista();
    }

    if(EstadoMenu == CRIAR){
        CreateMundo();
    }
        
  


    EndDrawing();
}



void StateMenu::DrawButtonsMenu(){
    //GuiGroupBox((Rectangle){ 50, 200, 180, 160 }, "Buttons");
    float Largura_Tela = GetRenderWidth();
    //float Altura_Tela = GetRenderHeight();
    float Meio_tela = (Largura_Tela/2) - 70;

    if (GuiButton((Rectangle){ Meio_tela, 300, 140, 30 }, T("Menu1"))) {
        EstadoMenu = CARREGAR;
        carregar_mundos(&mundos);
    }
    if (GuiButton((Rectangle){ Meio_tela, 340, 140, 30 }, T("Menu2"))) {
        EstadoMenu = CRIAR;
        
    }
    if (GuiButton((Rectangle){ Meio_tela, 380, 140, 30 }, T("Menu3"))) {}
    if (GuiButton((Rectangle){ Meio_tela, 420, 140, 30 }, T("Menu4"))) {*estadoAtual = STATE_EXIT;}
}

void StateMenu::CreateMundo(){
    float Largura_Tela = GetRenderWidth();
    //float Altura_Tela = GetRenderHeight();
    static char text[64] = "Nome do Mundo";
    static char text2[64] = "Seed";
    static bool textEdit = false;
    static bool textEdit2 = false;

    float Meio = (Largura_Tela/2) - (245/2);


    DrawRectangleRec((Rectangle){ Meio, 50, 245, 300 }, Fade(DARKGRAY, 0.5f));
    GuiLabel((Rectangle){ Meio+30, 50, 245, 30 }, "Criar Mundo");
    if (GuiButton((Rectangle){ 20, 20, 140, 30 }, T("Voltar"))) {EstadoMenu = MENU;}

    

    if (GuiTextBox((Rectangle){ Meio+5, 130, 235, 30 }, text, 64, textEdit))
        textEdit = !textEdit;

    if (GuiTextBox((Rectangle){ Meio+5, 170, 235, 30 }, text2, 64, textEdit2))
        textEdit2 = !textEdit2;

    
    if (GuiButton((Rectangle){ Meio+5, 300, 140, 30 }, T("Criar"))) {
        MundoData mundoNovo;
        mundoNovo.nomeMundo = text;
        mundoNovo.seed = text2;
        criar_mundo(&mundoNovo);
        EstadoMenu = MENU;
    }
}



void StateMenu::DrawMundosLista(){
    float Largura_Tela = GetRenderWidth();
    //float Altura_Tela = GetRenderHeight();
    static int listScroll = 0;
    static int listActive = -1;

    static std::string lista;   // persiste entre frames
    lista = mundosToList(mundos);
    if (GuiButton((Rectangle){ 20, 20, 140, 30 }, T("Voltar"))) {EstadoMenu = MENU;}
    GuiListView((Rectangle){ 20, 100, 200, 300 }, lista.c_str(), &listScroll, &listActive);


    //-------- Informações do Mundo ------------
    DrawRectangleRec((Rectangle){ Largura_Tela-250, 50, 245, 300 }, Fade(DARKGRAY, 0.5f));
    GuiLabel((Rectangle){ Largura_Tela-250, 50, 245, 30 }, T("Informações do Mundo"));

    // Botão para entrar
    if (GuiButton((Rectangle){ Largura_Tela-245, 310, 100, 30 }, T("Entrar"))) {
        if (listActive >= 0 && listActive < (int)mundos.size()) {
            NomeMundo = mundos[listActive].nomeMundo;
            *estadoAtual = STATE_GAME;
        }

    }

    if (GuiButton((Rectangle){ Largura_Tela-120, 310, 100, 30 }, T("Deletar"))) {
        if (listActive >= 0 && listActive < (int)mundos.size()) {
            deletarMundo(&mundos[listActive]);
            carregar_mundos(&mundos);
        }

    }

    

    // Informações ao clicar
    if (listActive >= 0 && listActive < (int)mundos.size()) {
        
        // mundo selecionado:
        // mundos[listActive].nomeMundo
        GuiLabel((Rectangle){ Largura_Tela-245, 80, 245, 30 }, mundos[listActive].nomeMundo.c_str());
        GuiLabel((Rectangle){ Largura_Tela-245, 120, 245, 30 }, mundos[listActive].seed.c_str());
    }
}


std::string StateMenu::GetNomeMundo(){
    return NomeMundo;
}

















