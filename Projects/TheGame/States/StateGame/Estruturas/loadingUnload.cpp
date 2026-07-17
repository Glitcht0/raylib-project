
#include "Estruturas.h"
#include "Engine/ThreadPool/ThreadPoll.h"

namespace fs = std::filesystem;

// ==========================================================
//              SISTEMA DE THREADS (IMPLEMENTAÇÃO)
// ==========================================================

// 1. Pedido (Main Thread)
void structures::requestChunkLoad(int cx, int cz) {
    long long key = ObjectKey(cx, cz);

    std::lock_guard<std::mutex> lock(resultMutex);
    
    // Verifica se já está carregando para não duplicar
    for (long long k : chunksBeingProcessed) {
        if (k == key) return; 
    }
    chunksBeingProcessed.push_back(key);

    // Manda pra piscina!
    ThreadPool::Get().enqueue([this, cx, cz]() {
        this->loadTask(cx, cz);
    });
}

// 2. Tarefa Pesada (Background Thread)
void structures::loadTask(int cx, int cz) {
    ObjectsChunck newChunk;
    newChunk.cx = cx;
    newChunk.cz = cz;

    // Tenta ler do disco. Se falhar, gera novos.
    if (!loadFromDisk(cx, cz, newChunk)) {
        generateChunk(cx, cz, newChunk);
        // Opcional: salvar logo após gerar
        // saveToDisk(newChunk); 
    }

    // Entrega o resultado protegido por mutex
    {
        std::lock_guard<std::mutex> lock(resultMutex);
        loadedChunks.push_back(newChunk);
    }
}

// 3. Recebimento (Main Thread)
void structures::processLoadedChunks() {
    std::lock_guard<std::mutex> lock(resultMutex);

    while (!loadedChunks.empty()) {
        ObjectsChunck chunk = loadedChunks.front();
        loadedChunks.pop_front();

        // Insere no mapa principal
        long long key = ObjectKey(chunk.cx, chunk.cz);
        objectData[key] = chunk;

        // Remove da lista de "processando"
        for (auto it = chunksBeingProcessed.begin(); it != chunksBeingProcessed.end(); ) {
            if (*it == key) {
                it = chunksBeingProcessed.erase(it);
            } else {
                ++it;
            }
        }
    }
}




// ==========================================================
//                  SISTEMA DE ARQUIVOS (I/O)
// ==========================================================
void structures::saveToDisk(const ObjectsChunck& chunk) {


    std::filesystem::path path = ResolveGamePath({ "Projects", "TheGame", "saves", nomeMundo, "objects" }) / ("chunk_" + std::to_string(chunk.cx) + "_" + std::to_string(chunk.cz) + ".dat");
    FILE* f = fopen(path.string().c_str(), "wb");
    if (!f) return;

    // Cabeçalho: Quantos objetos tem neste arquivo?
    int count = (int)chunk.objects.size();
    fwrite(&count, sizeof(int), 1, f);

    // Salva cada objeto (se count for 0, esse loop nem roda, o que é correto)
    for (GameObject* obj : chunk.objects) {
        ObjectSaveData data;
        data.type = 0; 
        data.position = obj->position;
        data.scale = obj->scale;
        data.color = obj->color;

        fwrite(&data, sizeof(ObjectSaveData), 1, f);
    }

    fclose(f);
}

bool structures::loadFromDisk(int cx, int cz, ObjectsChunck& chunk) {
    std::filesystem::path path = ResolveGamePath({ "Projects", "TheGame", "saves", nomeMundo, "objects" }) / ("chunk_" + std::to_string(cx) + "_" + std::to_string(cz) + ".dat");
    
    FILE* f = fopen(path.string().c_str(), "rb");
    if (!f) return false; // Arquivo não existe -> vai gerar novo

    int count = 0;
    fread(&count, sizeof(int), 1, f);

    for (int i = 0; i < count; i++) {
        ObjectSaveData data;
        fread(&data, sizeof(ObjectSaveData), 1, f);

        // Factory simples: recria o objeto baseado no tipo
        if (data.type == 0) {
            GameObject* obj = new Cube(data.position, data.scale, data.color);
            chunk.objects.push_back(obj);
        }
        // else if (data.type == 1) ... new Tree(...)
    }

    fclose(f);
    return true;
}


void structures::ensureSaveDirectory() {
    std::filesystem::path path = ResolveGamePath({ "Projects", "TheGame", "saves", nomeMundo, "objects" });
    if (!fs::exists(path)) {
        fs::create_directories(path);
    }
}




void structures::loadNearbyObjects(int cx, int cz){
        for (int z = -OBJ_VIEW_DISTANCE; z <= OBJ_VIEW_DISTANCE; z++) {
        for (int x = -OBJ_VIEW_DISTANCE; x <= OBJ_VIEW_DISTANCE; x++) {
            int targetCX = cx + x;
            int targetCZ = cz + z;
            long long key = ObjectKey(targetCX, targetCZ);

            // Se não tem no mapa, pede pra carregar
            if (objectData.find(key) == objectData.end()) {
                requestChunkLoad(targetCX, targetCZ); // <--- MUDANÇA AQUI
            }
        }
    }
}

void structures::unloadObjectsFarAway(int cx, int cz){
    int unloadDist = OBJ_VIEW_DISTANCE + 2;
    
    for (auto it = objectData.begin(); it != objectData.end(); ) {
        ObjectsChunck& chunk = it->second;
        int dist = (int)std::max(abs(chunk.cx - cx), abs(chunk.cz - cz));

        if (dist > unloadDist) {
            saveToDisk(chunk);
            for (GameObject* obj : chunk.objects) delete obj;
            it = objectData.erase(it);
        } else {
            ++it;
        }
    }
    
}