#pragma once
#include <memory>
#include <string>

class StateMachine;


enum class StateID {
    None,
    Exit, // Para substituir o ChangeState(nullptr)
    Menu,
    Game,
    State1,
    State2,
    State3,
};

// Estrutura base vazia
struct StateParams {
    virtual ~StateParams() = default;
};

// O pacote específico que o Menu vai enviar para o Jogo
struct GameParams : public StateParams {
    std::string parametro;
};


/**===================================================================
    🧬 Classe base para todos os estados do jogo. Qualquer estado,
    seja de menu, jogo, configuração, deve herdar esta classe e implementar
    os métodos virtuais puros `update()` e `draw()`. Além do `onEnter()` e `onExit()`.
    Mudança de estado com qualquer herdeiro de State: `ChangeState(new StatePróximo());` 
    ou `ChangeState(nullptr);` para sair do loop principal da Engine.

    Classe Estado
 ===================================================================*/
class State {
protected:
    StateMachine* machine = nullptr;
    void ChangeState(StateID next, std::shared_ptr<StateParams> params = nullptr);

public:
    virtual ~State() = default;

    void SetMachine(StateMachine* m){ machine = m; }

    virtual void onEnter(std::shared_ptr<StateParams> params = nullptr) {}
    virtual void onExit() {}

    virtual void update() = 0;
    virtual void draw() = 0;
};


