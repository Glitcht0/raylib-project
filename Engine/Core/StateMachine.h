#pragma once

#include "State.h"


/**===================================================================
    ⛓️​ Classe crida na Engine e passada para o projeto, para gerenciar os
    estados do jogo os quais herdam da classe State.
    possui funções como: `ChangeState(new StatePróximo());` para mudar de estado, `Update()` e `Draw()` para atualizar e desenhar o estado atual.
    
    Classe de gerenciamento de estado
 ===================================================================*/
class StateMachine{
public:
    void ChangeState(State* novo);

    void Update();
    void Draw();
    bool IsRunning() const { return current != nullptr; }

private:
    State* current = nullptr;
};