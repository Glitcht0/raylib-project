#pragma once


#include <raylib.h>
#include "libs/raygui.h"
#include "Engine/i18n/i18n.h"
#include "Engine/Core/State.h"
#include <string>

/** ======================================================
 *  📝 Templete de Estados do Programa
 *  
 * ======================================================*/




class StateGame : public State {
public:

    StateGame(std::string nome);
    ~StateGame();
    void onEnter() override;
    void onExit() override;

    void update() override;
    void draw() override;
    
    Font uiFont;

private:
    std::string nomeMundo;
    void DrawButtonsMenu(float larguraTela, float alturaTela);
};