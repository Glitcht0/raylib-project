#include "PauseMenu.h"





void PauseMenu(appstate* currentState, bool *AtivatMenu){
    float w = GetRenderWidth();
    float h = GetRenderHeight();

    float sx = w / BASE_W;
    float sy = h / BASE_H;
    float s = fminf(sx, sy); // mantém proporção

    float recW = 880 * s;
    float recH = 520 * s;
    float x = (w - recW) * 0.5f;
    float y = (h - recH) * 0.5f;

    DrawRectangleRec({ x, y, recW, recH }, COR_MESA);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 32 * s);
    GuiLabel({ x, y + 30*s, recW, 40*s }, T("Pause Menu"));

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20 * s);

    float bw = 140 * s;
    float bh = 30 * s;
    float bx = w * 0.5f - bw * 0.5f;

    if (GuiButton({ bx, y + 120*s, bw, bh }, T("Continuar"))) *AtivatMenu = false;
    if (GuiButton({ bx, y + 170*s, bw, bh }, T("Menu3"))) *AtivatMenu = false;
    if (GuiButton({ bx, y + 220*s, bw, bh }, T("Menu4"))) *currentState = STATE_MENU;
}

