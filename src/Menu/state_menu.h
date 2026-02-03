#pragma once
#include "src/core/state.h"
#include "src/core/state_machine.h"
#include "raylib.h"
#include "src/core/config.h"
#include <vector>
#include "libs/raygui.h"
#include "src/engine/i18n/i18n.h"
#include "src/engine/Data/Data.h"
#include <cmath>


enum MenuState{
    MENU,
    CONFIG,
    CRIAR,
    CARREGAR
};

void DrawButtonsDemo(void);

void DrawInputsDemo(void);
void DrawListsDemo(void);
void DrawSlidersDemo(void);
void DrawWindowsDemo(void);



class StateMenu : public State {
public:

    StateMenu();
    ~StateMenu();
    void onEnter() override;
    void onExit() override;

    void update(appstate* currentState) override;
    void draw() override;
    std::string GetNomeMundo();
    Font uiFont;

private:
    std::vector<MundoData> mundos;
    appstate* estadoAtual;
    void DrawButtonsMenu(float larguraTela, float alturaTela);
    void DrawMundosLista(float larguraTela, float alturaTela);
    void CreateMundo();
    MenuState EstadoMenu = MENU;
    
    std::string NomeMundo;

};