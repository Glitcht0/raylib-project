#pragma once


#include <raylib.h>
#include "libs/raygui.h"
#include "Engine/i18n/i18n.h"
#include "Engine/Core/State.h"

/** ======================================================
 *  📝 TEMPLATE DE ESTADO
 *  Todo estado deve herdar de 'State' e implementar os 4 métodos principais.
 * ====================================================== */
class State1 : public State {
public:

    State1();
    ~State1();
    void onEnter(std::shared_ptr<StateParams> params = nullptr) override;
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
    void onEnter(std::shared_ptr<StateParams> params = nullptr) override;
    void onExit() override;

    void update() override;
    void draw() override;
    
    Font uiFont;

private:
    void DrawButtonsMenu(float larguraTela, float alturaTela);
};