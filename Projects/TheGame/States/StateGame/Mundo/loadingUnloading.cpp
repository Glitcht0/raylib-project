#include "mundo.h"
#include "Engine/ThreadPool/ThreadPoll.h" // <--- Inclua a Pool aqui
#include <algorithm> // para std::find
#include <filesystem>
namespace fs = std::filesystem;



// ====================================================================
// Pedir para carregar um Chunk (Executado na Main Thread)
// ====================================================================
void World::requestChunkLoad(int cx, int cz) {
    long long key = ChunkKey(cx, cz);

    // 1. Verifica se já está carregando esse chunk para não duplicar tarefas
    //    (Isso evita pedir o mesmo chunk 60 vezes por segundo)
    {
        std::lock_guard<std::mutex> lock(resultMutex);
        for (long long k : chunksBeingProcessed) {
            if (k == key) return; // Já está na fila, sai fora
        }
        chunksBeingProcessed.push_back(key); // Marca como "em progresso"
    }

    // 2. Envia a tarefa para a ThreadPool Global
    ThreadPool::Get().enqueue([this, cx, cz]() {
        this->generateOrLoadTask(cx, cz);
    });
}

// ====================================================================
// A Tarefa Pesada (Executado por uma Thread da Pool)
// ====================================================================
void World::generateOrLoadTask(int cx, int cz) {
    TileChunk tc;
    tc.cx = cx;
    tc.cz = cz;
    tc.built = true;

    // Tenta carregar do disco, se falhar, gera
    if (!loadChunkFromDisk(cx, cz, tc)) {
        generateSingleChunk(tc);
        // Opcional: Salvar logo após gerar
        // saveChunkToDisk(tc); 
    }

    // ENTREGAR O RESULTADO
    // Precisamos bloquear o mutex pois 'loadedChunks' é lido pelo Main
    {
        std::lock_guard<std::mutex> lock(resultMutex);
        loadedChunks.push_back(tc);
    }
}

// ====================================================================
// Receber os Chunks Prontos (Executado na Main Thread no update)
// ====================================================================
void World::processLoadedChunks() {
    std::lock_guard<std::mutex> lock(resultMutex);
    
    while (!loadedChunks.empty()) {
        TileChunk tc = loadedChunks.front();
        loadedChunks.pop_front();

        // 1. Adiciona ao mapa oficial do jogo
        long long key = ChunkKey(tc.cx, tc.cz);
        chunkData[key] = tc;

        // 2. Remove da lista de "em processamento"
        // (Isso permite que o chunk seja pedido de novo no futuro se for descarregado)
        for (auto it = chunksBeingProcessed.begin(); it != chunksBeingProcessed.end(); ) {
            if (*it == key) {
                it = chunksBeingProcessed.erase(it);
            } else {
                ++it;
            }
        }
    }
}


// ====================================================================
//       Salva e Carrega Chunks do Disco
// ====================================================================
bool World::saveChunkToDisk(const TileChunk& tc) {
    std::filesystem::path filename = ResolveGamePath({ "Projects", "TheGame", "saves", nomeMundo, "chunks" }) / ("chunk_" + std::to_string(tc.cx) + "_" + std::to_string(tc.cz) + ".dat");
    
    // Garanta que a pasta "Projects/TheGame/saves" exista antes (faça isso no main ou init)
    FILE* f = fopen(filename.string().c_str(), "wb");
    if (!f){
        printf("ERRO: Nao foi possivel salvar o chunk em: %s (Verifique se a pasta existe)\n", filename.string().c_str());
        return false;
    } 

    // Salva a struct inteira de uma vez
    fwrite(tc.tiles, sizeof(Tile), CHUNK_SIZE * CHUNK_SIZE, f);
    fclose(f);
    return true;
}



bool World::loadChunkFromDisk(int cx, int cz, TileChunk& outChunk) {
    std::filesystem::path filename = ResolveGamePath({ "Projects", "TheGame", "saves", nomeMundo, "chunks" }) / ("chunk_" + std::to_string(cx) + "_" + std::to_string(cz) + ".dat");
    
    FILE* f = fopen(filename.string().c_str(), "rb");
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
    // Define os caminhos
    fs::path base = ResolveGamePath({ "Projects", "TheGame", "saves" });
    fs::path worldPath = base / nomeMundo;
    fs::path chunksPath = worldPath / "chunks";   
    fs::path playersPath = worldPath / "players"; 

    printf("Verificando estrutura de pastas para: %s\n", nomeMundo.c_str());

    try {
        // create_directories cria toda a árvore necessária se não existir
        if (fs::create_directories(chunksPath)) {
            printf("Pasta criada: %s\n", chunksPath.string().c_str());
        }
        
        if (fs::create_directories(playersPath)) {
            printf("Pasta criada: %s\n", playersPath.string().c_str());
        }

    } catch (const fs::filesystem_error& e) {
        printf("ERRO CRITICO ao criar pastas de save: %s\n", e.what());
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








// Agenda construção de malha
void World::ensureChunkMeshBuilt(int chunkIndex) {
    Chunk& c = chunks[chunkIndex];

    if (!c.built && !c.building) {
        buildQueue.push_back({c.cx, c.cz});
        c.building = true; 
    }
}




// ===== Descarrega modelos agendados =====
void World::processUnloadQueue(int maxPerFrame) {
    for (int i = 0; i < maxPerFrame && !unloadQueue.empty(); i++) {
        Model m = unloadQueue.back();
        unloadQueue.pop_back();

        UnloadModel(m);
    }
}

// ===== Constroi modelos agendados =====
void World::processBuildQueue(int maxPerFrame) {
    for (int i = 0; i < maxPerFrame && !buildQueue.empty(); i++) {
        std::pair<int, int> coords = buildQueue.front();
        buildQueue.pop_front();

        // Precisamos encontrar a chunk no vetor agora (pois o índice pode ter mudado)
        // Isso é uma busca linear, mas como é só 1 ou 2 por frame, não tem impacto na performance
        Chunk* targetChunk = nullptr;
        for (auto& c : chunks) {
            if (c.cx == coords.first && c.cz == coords.second) {
                targetChunk = &c;
                break;
            }
        }

        // Se a chunk ainda existe (não foi descarregada antes de ser construída)
        if (targetChunk) {
            buildChunkMesh(*targetChunk);   // cria Model
            targetChunk->built = true;      
            targetChunk->building = false;
        }
    }
}




// 🏞️ Descarrega chunks muito distantes do player, somente as meshs (Chunks)
void World::unloadFarChunks(Vector3 playerPos, int maxUnloads) {
    int cx = (int)floor(playerPos.x / CHUNK_SIZE);
    int cz = (int)floor(playerPos.z / CHUNK_SIZE);

    int deleteDistance = VIEW_DISTANCE + 4;
    int unloadsCount = 0;

    for (size_t i = 0; i < chunks.size(); ) {
        if (unloadsCount >= maxUnloads)break;
        Chunk& c = chunks[i];

        int distX = abs(c.cx - cx);
        int distZ = abs(c.cz - cz);

        if (distX > deleteDistance || distZ > deleteDistance) {

            long long key = ChunkKey(c.cx, c.cz);
            if (chunkData.count(key)) {
                saveChunkToDisk(chunkData[key]); // <--- SALVA NO DISCO
                chunkData.erase(key);            // <--- TIRA DA RAM
            }

            // 🔥 joga pra fila
            if (c.built) {
                unloadQueue.push_back(c.model);

            }

            // remove da lógica imediatamente
            chunks[i] = chunks.back();
            chunks.pop_back();
            unloadsCount++;
        } 
        else {
            i++;
        }
    }
}


// ============================================================================
//        ⬆️ Carrega a posição do jogador do disco
// ============================================================================
Vector3 World::loadplayer(float halfSize){
    Vector3 posicao;
    std::filesystem::path filename = ResolveGamePath({ "Projects", "TheGame", "saves", nomeMundo, "players" }) / "player.dat";
    
    FILE* f = fopen(filename.string().c_str(), "rb");
    if (!f) {
        posicao = Get_Spaw(halfSize);
        jogador.position = posicao;
        // Arquivo não existe

    }else{
        fread(&posicao, sizeof(Vector3), 1, f);
        printf("Posição do player carregada do disco: (%.2f, %.2f, %.2f)\n", posicao.x, posicao.y, posicao.z);
        jogador.position = posicao;
        
    }

    fclose(f);
    return posicao;

}


// ============================================================================
//        ⬇️ Salva a posição do jogador no disco
// ============================================================================
void World::savePlayerPosition(Vector3 position){
    jogador.position = position;
    std::filesystem::path filename = ResolveGamePath({ "Projects", "TheGame", "saves", nomeMundo, "players" }) / "player.dat";
    
    FILE* f = fopen(filename.string().c_str(), "wb");
    if (!f) {
        printf("Erro ao salvar a posição do player no disco.\n");
        return; // Erro ao abrir o arquivo
    }

    fwrite(&jogador.position, sizeof(Vector3), 1, f);
    fclose(f);
    printf("Posição do player salva no disco: (%.2f, %.2f, %.2f)\n", jogador.position.x, jogador.position.y, jogador.position.z);

}