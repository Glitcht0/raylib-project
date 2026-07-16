#pragma once


#include <raylib.h>
#include "libs/raygui.h"
#include "Engine/i18n/i18n.h"
#include "Engine/Core/State.h"
#include <string>


class StateMenu : public State {
public:

    StateMenu();
    ~StateMenu();
    void onEnter() override;
    void onExit() override;

    void update() override;
    void draw() override;
    
    Font uiFont;

private:
    std::string nomeMundo;
    void DrawButtonsMenu(float larguraTela, float alturaTela);
};