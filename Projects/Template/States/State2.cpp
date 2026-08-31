#include "States.h"

State2::State2(){
}

State2::~State2(){
}

void State1::onEnter(std::shared_ptr<StateParams> params) {
    // 💡 TUTORIAL: Extraia seus parâmetros aqui, se houver!
}

void State2::onExit(){
}

void State2::update(){
    
}

void State2::draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    float larguraTela = GetRenderWidth();
    float alturaTela = GetRenderHeight();

    float sx = larguraTela / 1280.0f;
    float sy = alturaTela / 720.0f;
    float s = (sx < sy) ? sx : sy;

    float btnW = 140 * s;
    float btnH = 30 * s;
    float x = larguraTela/2 - btnW/2;

    float y0 = 300 * s;
    float dy = 40 * s;
    if (GuiButton({ x, y0 + 1*dy, btnW, btnH }, T("Menu2"))) {
        ChangeState(StateID::State1);
    }
    if (GuiButton({ x, y0 + 2*dy, btnW, btnH }, T("Menu3"))) {
        ChangeState(StateID::State1);
    }

    

    EndDrawing();
}