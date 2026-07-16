#include "Engine/Core/State.h"
#include "Engine/Core/StateMachine.h"



void State::ChangeState(State* next) {
    if (machine) {
        machine->ChangeState(next);
    }
}
