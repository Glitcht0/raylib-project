#include "src/Game/state_game.h"


/* =====================================================================

    ⛓️‍💥 Funções pertenccentes ao modo de edição, aonde libera contem os 
    controles da vizualização livre 🛠️

  ===================================================================== */

void StateGame::handleMiddleMouse(Vector2 delta){

    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))) {

        float panSpeed = 0.002f * distance; // proporcional à distância

        Vector3 forward = Vector3Normalize(Vector3Subtract(cameraObj->cam.target, cameraObj->cam.position));
        Vector3 right   = Vector3Normalize(Vector3CrossProduct(forward, cameraObj->cam.up));
        Vector3 up      = Vector3CrossProduct(right, forward);

        Vector3 pan = Vector3Add( Vector3Scale(right, -delta.x * panSpeed), Vector3Scale(up,    delta.y * panSpeed) );

        cameraObj->target = Vector3Add(cameraObj->target, pan);

    }else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        float sensitivity = 0.005f;

        cameraObj->yaw   += delta.x * sensitivity;
        cameraObj->pitch += delta.y * sensitivity;

        cameraObj->pitch = Clamp(cameraObj->pitch, -PI/2 + 0.01f, PI/2 - 0.01f);
    }
}


void StateGame::handleScrollMouse(){
    float scroll = GetMouseWheelMove();
    if (scroll != 0.0f) {
        cameraObj->distance *= (1.0f - scroll * 0.1f);
        cameraObj->distance = Clamp(cameraObj->distance, 0.5f, 50.0f);
    }
}


void StateGame::updateOBBSelection(){
    if (transformMode != GameMode::EDIT) return;

    // UI capturou o mouse → não mexe na seleção
    if (mesaEd->isMouseCaptured()) return;

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;

    Ray ray = GetMouseRay(GetMousePosition(), cameraObj->cam);

    GameObject* selected = nullptr;

    for (GameObject* obj : objects) {
        Cube* cube = dynamic_cast<Cube*>(obj);
        if (!cube) continue;

        if (RayIntersectsOBB(ray, cube->position, cube->rotation, cube->size)) {
            selected = cube;
            break;
        }
    }

    // 🔹 CASO 1: clicou em um objeto
    if (selected) {
        selectedObject = selected;

        for (GameObject* obj : objects) {
            obj->is_selected = (obj == selected);
        }
    }
    // 🔹 CASO 2: clicou no vazio → DESELECIONA
    else {
        selectedObject = nullptr;

        for (GameObject* obj : objects) {
            obj->is_selected = false;
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
