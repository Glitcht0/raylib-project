#pragma once

#include "State.h"
#include <map>
#include <functional>
#include <memory>


/**===================================================================
    ⛓️​ Classe crida na Engine e passada para o projeto, para gerenciar os
    estados do jogo os quais herdam da classe State.
    possui funções como: `ChangeState(new StatePróximo());` para mudar de estado, `Update()` e `Draw()` para atualizar e desenhar o estado atual.
    
    Classe de gerenciamento de estado
 ===================================================================*/
class StateMachine {
public:
    // Método de registro de estado que a main() vai usar para ensinar a engine os estados existentes
    template <typename T>
    void RegisterState(StateID id) {
        factory[id] = []() { return new T(); };
    }

    void ChangeState(StateID id, std::shared_ptr<StateParams> params = nullptr);

    void Update();
    void Draw();
    bool IsRunning() const { return current != nullptr; }

private:
    State* current = nullptr;
    std::map<StateID, std::function<State*()>> factory;
};