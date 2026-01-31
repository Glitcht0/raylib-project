#include "Estruturas.h"




structures::structures(std::string worldName) {
    this->nomeMundo = worldName;
    ensureSaveDirectory();
}

structures::~structures() {
    // Salva tudo que está na memória antes de fechar
    for (auto& pair : objectData) {
        saveToDisk(pair.second);
        // Limpa a memória dos ponteiros
        for (GameObject* obj : pair.second.objects) {
            delete obj;
        }
    }
    objectData.clear();
}


// ==========================================================
//                 GERENCIADOR (UPDATE)
// ==========================================================
void structures::update(Vector3 playerPos) {
    // 1. Processa o que chegou das threads (INTEGRAÇÃO NA MAIN THREAD)
    processLoadedChunks();

    int cx = (int)floor(playerPos.x / OBJ_CHUNK_SIZE);
    int cz = (int)floor(playerPos.z / OBJ_CHUNK_SIZE);

    // 2. CARREGAR CHUNKS PERTO (Async)
    loadNearbyObjects(cx, cz);

    // 3. DESCARREGAR CHUNKS LONGE 
    unloadObjectsFarAway(cx, cz);


    // 4. Update dos objetos
    float dt = GetFrameTime();
    for (auto& pair : objectData) {
        for (GameObject* obj : pair.second.objects) {
            obj->update(dt); 
        }
    }
}

// ==========================================================
//                     DESENHO
// ==========================================================
void structures::draw() {
    for (auto& pair : objectData) {
        for (GameObject* obj : pair.second.objects) {
            obj->draw();
        }
    }
}



