#include "state_game.h"



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
        if (mesaEd.is_ativated){
            mesaEd.is_ativated = false;
        } else{
            mesaEd.is_ativated = true;
        }
        
        
    }



}




// Manipula transformação do objeto ativo
void StateGame::handleTransform() {
    if (!activeObject) return;

    Vector3 mouseWorld = GetMouseWorldPosCameraPlane();

    // MOVIMENTAR
    if (transformMode == GameMode::MOVE) {
        Vector3 delta = Vector3Subtract(mouseWorld, moveStartMouseWorld);
        activeObject->position = Vector3Add(moveStartObjectPos, delta);

    // ROTACIONAR
    } else if (transformMode == GameMode::ROTATE) {
        Vector2 mouseDeltaScreen = GetMousePosition() - prevMousePos;
        float rotSpeed = 0.3f;

        // Detecta eixo travado
        bool lockX = IsKeyDown(KEY_X);
        bool lockY = IsKeyDown(KEY_Y);
        bool lockZ = IsKeyDown(KEY_Z);

        if (!lockX) activeObject->rotation.x -= mouseDeltaScreen.y * rotSpeed;
        if (!lockY) activeObject->rotation.y += mouseDeltaScreen.x * rotSpeed;
        if (!lockZ) activeObject->rotation.z += mouseDeltaScreen.x * rotSpeed; // opcional

        prevMousePos = GetMousePosition();
    }


    // CONFIRMAR
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        transformMode = GameMode::EDIT;
        activeObject = nullptr;
    }

    // CANCELAR
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) || IsKeyPressed(KEY_ESCAPE)) {
        activeObject->position = moveStartObjectPos;
        activeObject->rotation = moveStartObjectRotation;
        transformMode = GameMode::EDIT;
        activeObject = nullptr;
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
