#include "state_game.h"


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
    if (mesaEd.isMouseCaptured()) return;

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