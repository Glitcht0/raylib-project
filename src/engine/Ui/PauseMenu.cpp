#include "PauseMenu.h"



void PauseMenu(appstate* currentState, bool *AtivatMenu){
    float Largura_Tela = GetRenderWidth();
    float Altura_Tela = GetRenderHeight();

    float recLargura = Largura_Tela - 400;
    float recAltura  = Altura_Tela - 200;
    float xrec = (Largura_Tela - recLargura) / 2, yrec = (Altura_Tela - recAltura) / 2;

    DrawRectangleRec((Rectangle){ xrec , yrec , recLargura, recAltura }, COR_MESA);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));

    GuiLabel((Rectangle){ xrec, yrec+20, recLargura, 40 }, T("Pause Menu"));

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    if (GuiButton((Rectangle){ (Largura_Tela/2)-70, yrec+70, 140, 30 }, T("Continuar"))) {*AtivatMenu = false;}
    if (GuiButton((Rectangle){ (Largura_Tela/2)-70, yrec+110, 140, 30 }, T("Menu3"))) {*AtivatMenu = false;}
    if (GuiButton((Rectangle){ (Largura_Tela/2)-70, yrec+150, 140, 30 }, T("Menu4"))) {*currentState = STATE_MENU;}


}
