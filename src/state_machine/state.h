#pragma once
#include "state_machine.h"

class State {
public:
    virtual ~State() = default;

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void update(appstate* currentState) = 0;
    virtual void draw() = 0;
};
