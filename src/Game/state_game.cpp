#include "state_game.h"





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
    
    objects.push_back(new Cube((Vector3){0, 1, 0}, (Vector3){1, 1, 1}, RED)); //Cubo vermelho

    objects.push_back(new Cube((Vector3){0, -0.01f, 0}, (Vector3){10, 0.01f, 10}, LIGHTGRAY)); // Chão

    uiFont = Carregarfonte();
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR);





    

}

void StateGame::onExit() {
    for (GameObject* obj : objects){
        delete obj;
    }   
        

    objects.clear();
    UnloadShader(shader);

    UnloadFont(uiFont);
}



// ===============================================================
//         🗿 Atualiza a câmera, objetos, controles...
// ===============================================================
void StateGame::update(appstate* currentState) {

    // -------------------- Controles edit viewport --------------------
    Vector2 delta = GetMouseDelta();
    
    handleScrollMouse(); //ZOOM
    handleMiddleMouse(delta); // PAN (SHIFT + BOTÃO DO MEIO) & ORBIT (BOTÃO DO MEIO SEM SHIFT)
    
    handleInput(); // Entrada de teclado para mudar modos e selecionar objetos
    mesaEd.update(selectedObject, transformMode); // Atualiza a mesa de edição
    handleTransform(); // Manipula transformação do objeto ativo
    
    updateOBBSelection(); // Seleção de objetos
       
    

    updateCamera(); // Atualiza posição da câmera com base em yaw, pitch e distância
    updateLights(); // Atualiza luzes no shader
    updateObjects(); // Atualiza os objetos
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

    


    // Cubo
    for (GameObject* obj : objects){
        obj->draw();
    }
        

    EndShaderMode();
    EndMode3D();

    DrawText("Glit Engine", 10, 10, 20, WHITE);


    DrawFPS(10, 40);
    mesaEd.draw(selectedObject, transformMode, uiFont);

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

