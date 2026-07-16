#pragma once

class StateMachine;

/**===================================================================
    🧬 Classe base para todos os estados do jogo. Qualquer estado,
    seja de menu, jogo, configuração, deve herdar esta classe e implementar
    os métodos virtuais puros `update()` e `draw()`. Além do `onEnter()` e `onExit()`.
    Mudança de estado com qualquer herdeiro de State: `ChangeState(new StatePróximo());`

    Classe Estado
 ===================================================================*/
class State {
protected:
    StateMachine* machine = nullptr;
    void ChangeState(State* next);

public:
    virtual ~State() = default;

    void SetMachine(StateMachine* m){
        machine = m;
    }

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void update() = 0;
    virtual void draw() = 0;
};


