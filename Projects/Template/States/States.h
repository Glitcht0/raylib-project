#pragma once


#include <raylib.h>
#include "libs/raygui.h"
#include "Engine/i18n/i18n.h"
#include "Engine/Core/State.h"

/** ======================================================
 *  📝 Templete de Estados do Programa
 *  
 * ======================================================
 */

class State1 : public State {
public:

    State1();
    ~State1();
    void onEnter() override;
    void onExit() override;

    void update() override;
    void draw() override;
    
    Font uiFont;

private:
    void DrawButtonsMenu(float larguraTela, float alturaTela);
};


class State2 : public State {
public:

    State2();
    ~State2();
    void onEnter() override;
    void onExit() override;

    void update() override;
    void draw() override;
    
    Font uiFont;

private:
    void DrawButtonsMenu(float larguraTela, float alturaTela);
};