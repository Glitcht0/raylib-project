#include "state_game.h"


/* =====================================================================

    🎮 Funções de controle de entrada do jogo e manipulação de objetos 🕹️

  ===================================================================== */

void StateGame::handleInput(){
    // Selecionar objeto para mover
    if (IsKeyPressed(KEY_G)) {
        for (GameObject* obj : objects) {
            if (obj->is_selected) {
                activeObject = obj;
                moveStartObjectPos = obj->position;
                moveStartMouseWorld = GetMouseWorldPosCameraPlane();
                transformMode = GameMode::MOVE;
                break;
            }
        }
    }


    // Selecionar objeto para rotacionar
    if (IsKeyPressed(KEY_R)) {
        for (GameObject* obj : objects) {
            if (obj->is_selected) {
                activeObject = obj;
                moveStartObjectPos = obj->position;
                moveStartObjectRotation = obj->rotation;
                moveStartMouseWorld = GetMouseWorldPosCameraPlane();
                transformMode = GameMode::ROTATE;
                break;
            }
        }
    }

    if (IsKeyPressed(KEY_N)) {
        if (mesaEd->is_ativated){
            mesaEd->is_ativated = false;
        } else{
            mesaEd->is_ativated = true;
        }
        
        
    }

    if (IsKeyPressed(KEY_M)) {
        if (transformMode == GameMode::EDIT){
            transformMode = GameMode::GAME;
        } else{
            transformMode = GameMode::EDIT;
        }
        
        
    }



}





// ===============================================================
// Obtém a posição no mundo do mouse projetada no plano XZ (Y=0)
// ===============================================================
Vector3 StateGame::GetMouseWorldPosCameraPlane() {
    Ray ray = GetMouseRay(GetMousePosition(), cameraObj->cam);

    Vector3 planePoint  = moveStartObjectPos;
    // plano perpendicular à câmera (como o Blender faz)
    Vector3 planeNormal = Vector3Normalize(Vector3Subtract(cameraObj->cam.target, cameraObj->cam.position));

    Vector3 hit = moveStartObjectPos;
    RayPlaneIntersection(ray, planePoint, planeNormal, &hit);

    return hit;
}
