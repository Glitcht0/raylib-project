#include "StateMachine.h"
// 🌱 Recebe um ponteiro para um estado novo, podendo ser qualquer estado herdeiro de state.
// É executado o onExit do estado atual e onEnter do novo estado, além de deletar o atual e mudar o ponteiro para o novo.
void StateMachine::ChangeState(State* novo){
    if(current){
        current->onExit();
        delete current;
    }

    current = novo;

    if(current)
    {
        current->SetMachine(this);
        current->onEnter();
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