#include "StateMachine.h"
// 🌱 Recebe um ponteiro para um estado novo, podendo ser qualquer estado herdeiro de state.
// É executado o onExit do estado atual e onEnter do novo estado, além de deletar o atual e mudar o ponteiro para o novo.
void StateMachine::ChangeState(StateID id, std::shared_ptr<StateParams> params) {
    if(current){
        current->onExit();
        delete current;
        current = nullptr;
    }

    if (id == StateID::Exit) {
        return; // current continuará nullptr, a engine vai parar (IsRunning() retorna false)
    }

    // Se o estado existir na fábrica, nós o criamos
    if (factory.find(id) != factory.end()) {
        current = factory[id]();
        current->SetMachine(this);
        current->onEnter(params); // Passa os parâmetros!
    }
}
void StateMachine::Update()
{
    if(current)
        current->update();
}

void StateMachine::Draw()
{
    if(current)
        current->draw();
}