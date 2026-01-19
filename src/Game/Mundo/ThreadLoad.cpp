#include "mundo.h"

#include <filesystem>
namespace fs = std::filesystem;

// ====================================================================
// 🧵 SISTEMA DE THREADS PARA CARREGAMENTO ASSÍNCRONO DE CHUNKS
// ====================================================================
void World::loaderThreadLoop() {
    while (threadRunning) {
        ChunkRequest req;
        bool hasRequest = false;

        // 🔒 Bloqueia para verificar se tem trabalho
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (!pendingRequests.empty()) {
                req = pendingRequests.front();
                pendingRequests.pop_front();
                hasRequest = true;
            }
        }

        if (hasRequest) {
            TileChunk tc;
            tc.cx = req.cx;
            tc.cz = req.cz;
            tc.built = true;

            // 1. Tenta carregar do disco
            if (!loadChunkFromDisk(req.cx, req.cz, tc)) {
                // 2. Se falhar (arquivo não existe), GERA o terreno
                generateSingleChunk(tc);
                
                // Opcional: Salvar imediatamente para não gerar de novo
                // saveChunkToDisk(tc); 
            }

            // 🔒 Devolve para a Main Thread
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                loadedChunks.push_back(tc);
            }
        } 
        else {
            // Dorme um pouco para não fritar a CPU se não tiver trabalho
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}

// ====================================================================
// Processa chunks carregados pela thread e os insere no mapa principal
// ====================================================================
void World::processLoadedChunks() {
    std::lock_guard<std::mutex> lock(queueMutex);
    
    while (!loadedChunks.empty()) {
        TileChunk tc = loadedChunks.front();
        loadedChunks.pop_front();

        // Insere no mapa principal (Agora o chunk existe oficialmente no jogo)
        long long key = ChunkKey(tc.cx, tc.cz);
        chunkData[key] = tc;
    }
}


// ====================================================================
// Pede para a thread carregar/gerar um chunk
// ====================================================================
void World::requestChunkLoad(int cx, int cz) {
    std::lock_guard<std::mutex> lock(queueMutex);
    
    // Verifica se já não foi pedido (para não pedir 2x o mesmo chunk)
    for (const auto& req : pendingRequests) {
        if (req.cx == cx && req.cz == cz) return;
    }
    
    // Verifica se já não está na fila de entrega (acabou de carregar mas update não rodou ainda)
    for (const auto& tc : loadedChunks) {
        if (tc.cx == cx && tc.cz == cz) return;
    }

    pendingRequests.push_back({cx, cz});
}




// ====================================================================
//       Salva e Carrega Chunks do Disco
// ====================================================================
bool World::saveChunkToDisk(const TileChunk& tc) {
    std::string filename = "saves/" + nomeMundo + "/chunks/chunk_" + std::to_string(tc.cx) + "_" + std::to_string(tc.cz) + ".dat";
    
    // Garanta que a pasta "saves" exista antes (faça isso no main ou init)
    FILE* f = fopen(filename.c_str(), "wb");
    if (!f) return false;

    // Salva a struct inteira de uma vez
    fwrite(tc.tiles, sizeof(Tile), CHUNK_SIZE * CHUNK_SIZE, f);
    fclose(f);
    return true;
}



bool World::loadChunkFromDisk(int cx, int cz, TileChunk& outChunk) {
    std::string filename = "saves/" + nomeMundo + "/chunks/chunk_" + std::to_string(cx) + "_" + std::to_string(cz) + ".dat";
    
    FILE* f = fopen(filename.c_str(), "rb");
    if (!f) return false; // Arquivo não existe

    fread(outChunk.tiles, sizeof(Tile), CHUNK_SIZE * CHUNK_SIZE, f);
    fclose(f);
    
    outChunk.cx = cx;
    outChunk.cz = cz;
    outChunk.built = true;
    printf("Chunk (%d, %d) carregado do disco.\n", cx, cz);
    return true;
}


// ====================================================================
// Garante que as pastas de salvamento existam
// ====================================================================
void World::ensureSaveDirectories() {
    fs::path base = "saves";
    fs::path worldPath = base / nomeMundo;
    printf("Verificando pasta de save: %s\n", nomeMundo.c_str());

    // create_directories retorna false se a pasta já existir, 
    // mas não dá erro. É seguro e limpo.
    try {
        fs::create_directories(worldPath);
        printf("Pasta de save criada: %s\n", worldPath.string().c_str());
    } catch (const fs::filesystem_error& e) {
        printf("Erro ao criar pasta de save: %s\n", e.what());
    }
}


// ====================================================================
// Gera um chunk simples (TODO: implementar geração real)
// ====================================================================
void World::generateSingleChunk(TileChunk& tc) {
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            
            tc.tiles[z][x].type = TILE_WATER;
            tc.tiles[z][x].flags |= TILE_BLOCKED;
     
        }
    }

}




