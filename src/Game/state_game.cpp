#include "state_game.h"
#include <iostream>




// ==========================
//         Constructor
// ==========================
StateGame::StateGame() {
    camera.position = (Vector3){ 4.0f, 4.0f, 4.0f };
    camera.target   = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy     = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    distance = 6.0f;
    yaw = 135.0f * DEG2RAD;
    pitch = 30.0f * DEG2RAD;
    
}

// =============================================
//         Executa ao ao entrar no estado
// =============================================
void StateGame::onEnter() {
    shader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
        TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.5f, 0.5f, 0.5f, 1.0f }, SHADER_UNIFORM_VEC4);

    //lights[0] = CreateLight(LIGHT_POINT, (Vector3){ -2, 3, -2 }, Vector3Zero(), YELLOW, shader);
    //lights[1] = CreateLight(LIGHT_POINT, (Vector3){ 2, 3, 2 }, Vector3Zero(), RED, shader);
    
    //lights[3] = CreateLight(LIGHT_POINT, (Vector3){ 2, 3, -2 }, Vector3Zero(), BLUE, shader);
    lights[0] = CreateLight(LIGHT_POINT, (Vector3){ -10, 5, -7 }, Vector3Zero(), WHITE, shader);
    lights[2] = CreateLight(LIGHT_POINT, (Vector3){ 15, 3, 7 }, Vector3Zero(), WHITE, shader);

}

void StateGame::onExit() {
    UnloadShader(shader);
}


// ===============================================================
//         🎥 Atualiza a câmera com controle orbital e pan
// ===============================================================
void StateGame::update(appstate* currentState) {
    // ---------- ZOOM ----------
    float scroll = GetMouseWheelMove();
    if (scroll != 0.0f) {
        distance *= (1.0f - scroll * 0.1f);
        distance = Clamp(distance, 0.5f, 50.0f);
    }

    Vector2 delta = GetMouseDelta();

    // ---------- PAN (SHIFT + BOTÃO DO MEIO) ----------
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) &&
        (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))) {

        float panSpeed = 0.002f * distance; // proporcional à distância

        Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        Vector3 right   = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
        Vector3 up      = Vector3CrossProduct(right, forward);

        Vector3 pan =
            Vector3Add(
                Vector3Scale(right, -delta.x * panSpeed),
                Vector3Scale(up,    delta.y * panSpeed)
            );

        camera.target = Vector3Add(camera.target, pan);
    }

    // ---------- ORBIT (BOTÃO DO MEIO SEM SHIFT) ----------
    else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        float sensitivity = 0.005f;

        yaw   += delta.x * sensitivity;
        pitch += delta.y * sensitivity;

        pitch = Clamp(pitch, -PI/2 + 0.01f, PI/2 - 0.01f);
    }

    // ---------- CONVERSÃO ESFÉRICA → CARTESIANA ----------
    camera.position.x = camera.target.x + distance * cosf(pitch) * cosf(yaw);
    camera.position.y = camera.target.y + distance * sinf(pitch);
    camera.position.z = camera.target.z + distance * cosf(pitch) * sinf(yaw);


    // ---------- Atualiza luzes no shader ----------
    for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(shader, lights[i]);


    float intensity = 1.0f;

    for (int i = 0; i < MAX_LIGHTS; i++){
        lights[i].color = (Color){
            (unsigned char)(255 * intensity),
            (unsigned char)(255 * intensity),
            (unsigned char)(255 * intensity),255 };
    }
    UpdateLightValues(shader, lights[0]);
    
    float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);


}







// =================================
//         🖌️ Desenha a cena
// =================================
void StateGame::draw() {
    BeginDrawing();
    ClearBackground(FUNDO);

    BeginMode3D(camera);

    DrawGridXZ(40, 1.0f);


    

    BeginShaderMode(shader);

    // Chão
    DrawCube((Vector3){0,-0.01f,0}, 10, 0.01f, 10, LIGHTGRAY);


    // Cubo
    DrawCube((Vector3){ 0.0f, 1.0f, 0.0f }, 1.0f, 1.0f, 1.0f, RED );

    // Contorno do cubo
    //DrawCubeWires((Vector3){ 0.0f, 1.0f, 0.0f },1.0f, 1.0f, 1.0f, BLACK);
    
    EndShaderMode();
    EndMode3D();

    DrawText("Cubo 3D simples", 10, 10, 20, WHITE);
    DrawFPS(10, 40);

    EndDrawing();
}




// =================================
//        ✏️ Desenha o Grid
// =================================
void StateGame::DrawGridXZ(int size, float step)
{
    int half = size / 2;

    // ---------- EIXOS (UMA VEZ SÓ) ----------
    DrawLine3D(
        (Vector3){ -half*step, 0, 0 },
        (Vector3){  half*step, 0, 0 },
        RED     // eixo X
    );

    DrawLine3D(
        (Vector3){ 0, 0, -half*step },
        (Vector3){ 0, 0,  half*step },
        BLUE    // eixo Z
    );

    // ---------- GRID ----------
    for (int i = -half; i <= half; i++)
    {
        if (i == 0) continue; // NÃO redesenha em cima dos eixos

        float pos = i * step;

        Color color = (i % 10 == 0) ? COR_GRID2 : COR_GRID;

        // paralelas ao X
        DrawLine3D(
            (Vector3){ -half*step, 0, pos },
            (Vector3){  half*step, 0, pos },
            color
        );

        // paralelas ao Z
        DrawLine3D(
            (Vector3){ pos, 0, -half*step },
            (Vector3){ pos, 0,  half*step },
            color
        );
    }
}

