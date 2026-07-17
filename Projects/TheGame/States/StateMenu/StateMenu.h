#pragma once


#include <raylib.h>
#include "libs/raygui.h"
#include "Engine/i18n/i18n.h"
#include "Projects/TheGame/config.h"
#include "Engine/Core/State.h"
#include <cmath>
#include "Projects/TheGame/src/Data/Data.h"
#include <vector>
#include <string>

enum MenuState{
    MENU,
    CONFIG,
    CRIAR,
    CARREGAR
};


class StateMenu : public State {
public:

    StateMenu();
    ~StateMenu();
    void onEnter() override;
    void onExit() override;

    void update() override;
    void draw() override;

    std::string GetNomeMundo();
    
    Font uiFont;

private:
    std::vector<MundoData> mundos;
    std::string nomeMundo;
    void DrawButtonsMenu(float larguraTela, float alturaTela);
    void DrawMundosLista(float larguraTela, float alturaTela);
    void CreateMundo();
    MenuState EstadoMenu = MENU;
};