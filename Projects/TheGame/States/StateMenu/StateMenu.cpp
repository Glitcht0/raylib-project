#include "StateMenu.h"
#include "Projects/TheGame/States/StateGame/StateGame.h"

StateMenu::StateMenu()
{
}

StateMenu::~StateMenu()
{
}

void StateMenu::onEnter()
{
}

void StateMenu::onExit()
{
}

void StateMenu::update(){
    
}

void StateMenu::draw(){
    BeginDrawing();
    ClearBackground(BLACK);
    float larguraTela = GetRenderWidth();
    float alturaTela = GetRenderHeight();

    
    
    if (EstadoMenu==MENU){
        DrawButtonsMenu(larguraTela, alturaTela);
        /*
        DrawButtonsDemo();
        DrawInputsDemo();
        DrawListsDemo();
        DrawSlidersDemo();
        DrawWindowsDemo();*/
        
    }

    if (EstadoMenu == CARREGAR){
        DrawMundosLista(larguraTela, alturaTela);
    }

    if(EstadoMenu == CRIAR){
        CreateMundo();
    }
        
  


    EndDrawing();
}


void StateMenu::DrawButtonsMenu(float larguraTela, float alturaTela){
    
    float sx = larguraTela / 1280.0f;
    float sy = alturaTela / 720.0f;
    float s = (sx < sy) ? sx : sy;

    float btnW = 140 * s;
    float btnH = 30 * s;
    float x = larguraTela/2 - btnW/2;

    float y0 = 300 * s;
    float dy = 40 * s;

    if (GuiButton({ x, y0 + 0*dy, btnW, btnH }, T("Menu1"))) {
        EstadoMenu = CARREGAR;
        carregar_mundos(&mundos);
    }
    if (GuiButton({ x, y0 + 1*dy, btnW, btnH }, T("Menu2"))) {
        EstadoMenu = CRIAR;
    }
    if (GuiButton({ x, y0 + 2*dy, btnW, btnH }, T("Menu3"))) {}
    if (GuiButton({ x, y0 + 3*dy, btnW, btnH }, T("Menu4"))) {
        ChangeState(nullptr);
    }


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


void StateMenu::DrawMundosLista(float larguraTela, float alturaTela){
    static int listScroll = 0;
    static int listActive = -1;

    static std::string lista;
    lista = mundosToList(mundos);

    // ===== Botão Voltar =====
    float margem = larguraTela * 0.02f;
    float botaoLargura = larguraTela * 0.12f;
    float botaoAltura = alturaTela * 0.045f;

    if (GuiButton({ margem, margem, botaoLargura, botaoAltura }, T("Voltar"))) {
        EstadoMenu = MENU;
    }

    // ===== Lista de mundos =====
    float listaX = margem;
    float listaY = margem * 4;
    float listaLargura = larguraTela * 0.25f;
    float listaAltura = alturaTela * 0.55f;

    GuiListView({ listaX, listaY, listaLargura, listaAltura },
                lista.c_str(), &listScroll, &listActive);

    // ===== Painel de informações (direita) =====
    float painelLargura = larguraTela * 0.28f;
    float painelAltura = listaAltura;
    float painelX = larguraTela - painelLargura - margem;
    float painelY = listaY;

    DrawRectangleRec({ painelX, painelY, painelLargura, painelAltura }, Fade(DARKGRAY, 0.5f));
    GuiLabel({ painelX, painelY, painelLargura, botaoAltura }, T("Informações do Mundo"));

    // ===== Botões =====
    float botaoInfoLargura = painelLargura * 0.42f;
    float botaoInfoAltura = botaoAltura;
    float espacoBotoes = botaoInfoLargura * 0.1f;

    float botoesY = painelY + painelAltura - botaoInfoAltura - margem;

    if (GuiButton({ painelX + margem, botoesY, botaoInfoLargura, botaoInfoAltura }, T("Entrar"))) {
        if (listActive >= 0 && listActive < (int)mundos.size()) {
            nomeMundo = mundos[listActive].nomeMundo;
            ChangeState(new StateGame(nomeMundo));
        }
    }

    if (GuiButton({ painelX + margem + botaoInfoLargura + espacoBotoes, botoesY, botaoInfoLargura, botaoInfoAltura }, T("Deletar"))) {
        if (listActive >= 0 && listActive < (int)mundos.size()) {
            deletarMundo(&mundos[listActive]);
            carregar_mundos(&mundos);
        }
    }

    // ===== Informações do mundo selecionado =====
    if (listActive >= 0 && listActive < (int)mundos.size()) {
        float textoY = painelY + botaoAltura * 1.8f;

        GuiLabel({ painelX + margem, textoY, painelLargura - 2*margem, botaoAltura }, mundos[listActive].nomeMundo.c_str());
        GuiLabel({ painelX + margem, textoY + botaoAltura * 1.4f, painelLargura - 2*margem, botaoAltura }, mundos[listActive].seed.c_str());
    }
}



std::string StateMenu::GetNomeMundo(){
    return nomeMundo;
}