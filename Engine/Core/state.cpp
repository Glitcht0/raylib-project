#include "Engine/Core/State.h"
#include "Engine/Core/StateMachine.h"



void State::ChangeState(StateID next, std::shared_ptr<StateParams> params) {
    if (machine) {
        machine->ChangeState(next, params);
    }
}
