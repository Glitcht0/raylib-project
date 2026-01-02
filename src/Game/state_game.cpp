#include "state_game.h"





// ==========================
//         Constructor
// ==========================
StateGame::StateGame() {
    
    
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
    SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR); // Suaviza a fonte

    cameraObj = new CameraObject({4,4,4}, {0,1,0}, {0,1,0});

    playerObj = new Player(cameraObj);
    playerObj->position = { 0, 0, 0 };



    

}


// =============================================
//         Executa ao ao Sair no estado
// =============================================
void StateGame::onExit() {
    for (GameObject* obj : objects){
        delete obj;
    }   
        
    delete playerObj;
    playerObj = nullptr;

    delete cameraObj;
    cameraObj = nullptr;


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
       
    
    cameraObj->update(GetFrameTime());
    playerObj->update(GetFrameTime());
    updateLights(); // Atualiza luzes no shader
    updateObjects(); // Atualiza os objetos
}







// =================================
//         🖌️ Desenha a cena
// =================================
void StateGame::draw() {
    BeginDrawing();
    ClearBackground(FUNDO);

    BeginMode3D(cameraObj->cam);

    DrawGridXZ(40, 1.0f);

    BeginShaderMode(shader);

    


    // Cubo
    for (GameObject* obj : objects){
        obj->draw();
    }
    
    cameraObj->draw();
    

    EndShaderMode();
    playerObj->draw();
    EndMode3D();

    DrawText("Glit Engine", 10, 10, 20, WHITE);



    DrawFPS(10, 40);
    mesaEd.draw(selectedObject, transformMode, uiFont);

    EndDrawing();
}




