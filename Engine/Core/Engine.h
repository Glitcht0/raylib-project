#pragma once

#include "StateMachine.h"


/**===================================================================
    🚂 É chamada pelo projeto para gerenciar o loop principal e a
    a maquina de estados, no qual roda o update e draw, além de controlar
    o fechamento da tela.
    
    Função do Loop principal
 ===================================================================*/
void EngineRun();


StateMachine& GetStateMachine();