#include "mundo.h"


World::World(std::string nomeM): perlin(time(nullptr)) {
    

    nomeMundo = nomeM;
    
    ensureSaveDirectories();
    chunkData.clear();
    gerarmundo();

    // Inicia a thread de carregamento
    threadRunning = true;
    chunkLoaderThread = std::thread(&World::loaderThreadLoop, this);

    

}

World::~World() {
    // 1. Salvar tudo antes de sair (opcional, mas recomendado)
    for (auto& pair : chunkData) {
        saveChunkToDisk(pair.second);
    }

    // 2. Parar a thread
    threadRunning = false;
    if (chunkLoaderThread.joinable()) {
        chunkLoaderThread.join();
    }
    
    // Limpeza das meshes
    for (Chunk& c : chunks) {
        if (c.built) UnloadModel(c.model);
    }
}


void World::update(Vector3 playerPos) {

    processUnloadQueue(1); 
    processBuildQueue(1); 

    processLoadedChunks();

    unloadFarChunks(playerPos);
    updateChunks(playerPos);


}


void World::draw() {
    for (int index : visibleChunks) {
        if (chunks[index].built) { 
            DrawModel(chunks[index].model, {0,0,0}, 1.0f, WHITE);
        }
    }
}



void World::updateChunks(Vector3 playerPos) {
    int cx, cz;
    getPlayerChunk(playerPos, cx, cz);
 
    tileVisibleChunks.clear(); // 🧊 Tiles
    visibleChunks.clear(); // 🏞️ Mesh

    for (int dz = -VIEW_DISTANCE; dz <= VIEW_DISTANCE; dz++) {
        for (int dx = -VIEW_DISTANCE; dx <= VIEW_DISTANCE; dx++) {
            int chunkX = cx + dx;
            int chunkZ = cz + dz;
            long long key = ChunkKey(chunkX, chunkZ);

            // Verifica se já temos os DADOS
            if (chunkData.find(key) == chunkData.end()) {
                
                requestChunkLoad(chunkX, chunkZ); // NÃO TEMOS! Pede para a thread carregar/gerar
            } 
            else {
                
                tileVisibleChunks.push_back(key); // JÁ TEMOS! Pode renderizar e criar Mesh

                TileChunk& tc = chunkData[key];
                if (tc.built){}
                

                
                // Só cria a Mesh visual se os dados lógicos já existirem
                int meshIndex = getOrCreateMeshChunk(chunkX, chunkZ);
                ensureChunkMeshBuilt(meshIndex);
                visibleChunks.push_back(meshIndex);
            }
        }
    }
}




void World::gerarmundo() {

    // ===== Gerar Matrix do mundo ====
    for (int z = 0; z < WORLD_H; z++) {
        for (int x = 0; x < WORLD_W; x++) {
            world[z][x].type = TILE_WATER;
            world[z][x].flags = 0;
            world[z][x].flags |= TILE_BLOCKED;


        }
    }

    CreateIsland(0, 0, 200, 200, 1.4f, 1.4f);


    


    // ===== 🏞️ Criar chunks pra render ====
    int chunksX = (WORLD_W + CHUNK_SIZE - 1) / CHUNK_SIZE;
    int chunksZ = (WORLD_H + CHUNK_SIZE - 1) / CHUNK_SIZE;

    chunks.reserve(chunksX * chunksZ);

    for (int cz = 0; cz < chunksZ; cz++) {
        for (int cx = 0; cx < chunksX; cx++) {
            Chunk c;
            c.cx = cx;
            c.cz = cz;
            c.built = false;
            chunks.push_back(c);
        }
    }


    // ===== 🧊 Criar TileChunks e copiar do world =====
    //tileChunks.clear();
    chunkData.clear();

    chunksX = (WORLD_W + CHUNK_SIZE - 1) / CHUNK_SIZE;
    chunksZ = (WORLD_H + CHUNK_SIZE - 1) / CHUNK_SIZE;

    for (int cz = 0; cz < chunksZ; cz++) {
        for (int cx = 0; cx < chunksX; cx++) {
            TileChunk tc;
            tc.cx = cx;
            tc.cz = cz;
            tc.built = true; // Marque como true, pois já estamos gerando aqui

            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    int wx = cx * CHUNK_SIZE + x;
                    int wz = cz * CHUNK_SIZE + z;

                    // Se estiver dentro do mundo, copia do world[][]
                    if (wx < WORLD_W && wz < WORLD_H) {
                        tc.tiles[z][x] = world[wz][wx];
                    } else {
                        tc.tiles[z][x].type = TILE_WATER;
                        tc.tiles[z][x].flags = TILE_BLOCKED;
                    }
                }
            }

            // Adiciona tanto no vetor (para compatibilidade com código antigo)
            //tileChunks.push_back(tc);
            
            // E TAMBÉM no mapa (para o GetTile funcionar)
            long long key = ChunkKey(cx, cz);
            chunkData[key] = tc;
            // -----------------------------
        }
    }
    mundo_gerado = true;

}





// 🏞️ Descarrega chunks muito distantes do player, somente as meshs (Chunks)
void World::unloadFarChunks(Vector3 playerPos) {
    int cx = (int)floor(playerPos.x / CHUNK_SIZE);
    int cz = (int)floor(playerPos.z / CHUNK_SIZE);

    int deleteDistance = VIEW_DISTANCE + 4;

    for (size_t i = 0; i < chunks.size(); ) {
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
        } 
        else {
            i++;
        }
    }
}


