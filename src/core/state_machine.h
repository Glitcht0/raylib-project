#pragma once
#include <string>

enum appstate {
    STATE_INIT,
    STATE_MENU,
    STATE_CONFIG,
    STATE_GAME,
    STATE_EXIT
};






void principal_loop();

void mensagemBox(appstate *currentState);