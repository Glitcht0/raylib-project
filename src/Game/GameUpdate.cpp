#include "src/Game/state_game.h"

/* =====================================================================

    🎇atualiza todos os objetos do jogo, câmera, luzes, mesa de edição ✨

  ===================================================================== */

void StateGame::updateAll(){
    mesaEd->update(selectedObject, transformMode); // Atualiza a mesa de edição e elementos de Ui de debug
    
    
    cameraObj->update(GetFrameTime());
    playerObj->update(GetFrameTime());
    
    world.update(playerObj->position);
    estruturas->update(playerObj->position);
    updateLights(); // Atualiza luzes no shader
    updateObjects(); // Atualiza os objetos
}


// ---------- CONVERSÃO ESFÉRICA → CARTESIANA ----------
// ---------- Atualiza posição da câmera com base em yaw, pitch e distância ----------
void StateGame::updateCamera(){
    cameraObj->cam.position.x = cameraObj->cam.target.x + distance * cosf(pitch) * cosf(yaw);
    cameraObj->cam.position.y = cameraObj->cam.target.y + distance * sinf(pitch);
    cameraObj->cam.position.z = cameraObj->cam.target.z + distance * cosf(pitch) * sinf(yaw);
}



void StateGame::updateLights(){
    for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(shader, lights[i]);

    float intensity = 1.0f;

    for (int i = 0; i < MAX_LIGHTS; i++){lights[i].color = (Color){
        (unsigned char)(255 * intensity),
        (unsigned char)(255 * intensity),
        (unsigned char)(255 * intensity),255 };
    }
    UpdateLightValues(shader, lights[0]);

    float cameraPos[3] = { cameraObj->cam.position.x, cameraObj->cam.position.y, cameraObj->cam.position.z };
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
}


void StateGame::updateObjects(){
    float dt = GetFrameTime();
    for (GameObject* obj : objects){
        obj->update(dt);
    }
}

// ===============================================================