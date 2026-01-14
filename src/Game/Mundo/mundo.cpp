#include "mundo.h"


World::World(): perlin(time(nullptr)) {
    gerarmundo();
    //time(nullptr)
}

World::~World() {
    for (Chunk& c : chunks) {
        if (c.built) UnloadModel(c.model);
    }
}



void World::update(Vector3 playerPos) {
    if(!mundo_gerado){
        return;
    }
    unloadFarChunks(playerPos);
    updateChunks(playerPos);


}


void World::draw() {
    if(!mundo_gerado)
        return;

   
    for (int index : visibleChunks) {
        DrawModel(chunks[index].model, {0,0,0}, 1.0f, WHITE);
    }
}




void World::updateChunks(Vector3 playerPos) {
    int cx = (int)floor(playerPos.x / CHUNK_SIZE);
    int cz = (int)floor(playerPos.z / CHUNK_SIZE);

    tileVisibleChunks.clear();
    visibleChunks.clear();

    // percorre a área visível ao redor do player
    for (int dz = -VIEW_DISTANCE; dz <= VIEW_DISTANCE; dz++) {
        for (int dx = -VIEW_DISTANCE; dx <= VIEW_DISTANCE; dx++) {
            int chunkX = cx + dx;
            int chunkZ = cz + dz;

            // pega ou cria TileChunk dinamicamente
            TileChunk* tilechunk = GetTileChunk(chunkX, chunkZ);
            
            if (!tilechunk->built){
                
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    for (int x = 0; x < CHUNK_SIZE; x++) {
                        tilechunk->tiles[z][x].type = TILE_WATER;
                        tilechunk->tiles[z][x].flags |= TILE_BLOCKED;
                    }
                }
                tilechunk->built = true;
            }
            tileVisibleChunks.push_back(tilechunk);

            // ====== Mesh Chunk =====

            int chunkIndex = -1; // Vamos procurar o índice

            // 1. Procura se o chunk já existe
            for (size_t i = 0; i < chunks.size(); i++) {
                if (chunks[i].cx == chunkX && chunks[i].cz == chunkZ) { 
                    chunkIndex = i; 
                    break; 
                }
            }

            // 2. Se não existe, cria um novo
            if (chunkIndex == -1) {
                Chunk c;
                c.cx = chunkX;
                c.cz = chunkZ;
                c.built = false;
                
                chunks.push_back(c); // Aqui o vetor pode realocar, mas não tem problema
                
                // O índice do novo elemento é o tamanho - 1
                chunkIndex = chunks.size() - 1; 
            }

            // 3. Garante que a mesh está construída
            // Note que acessamos chunks[chunkIndex] diretamente
            if (!chunks[chunkIndex].built)
                buildChunkMesh(chunks[chunkIndex]);

            // 4. Adiciona O ÍNDICE na lista visível (Seguro contra crash)
            visibleChunks.push_back(chunkIndex);
        }
    }
}




// Em mundo.cpp -> unloadFarChunks

void World::unloadFarChunks(Vector3 playerPos) {
    int cx = (int)floor(playerPos.x / CHUNK_SIZE);
    int cz = (int)floor(playerPos.z / CHUNK_SIZE);
    
    int deleteDistance = VIEW_DISTANCE + 4;
    // --------------------------
    
    for (size_t i = 0; i < chunks.size(); ) {
        Chunk& c = chunks[i];

        int distX = abs(c.cx - cx);
        int distZ = abs(c.cz - cz);

        // Debug visual para entender o que o código "pensa"
        // (Isso vai floodar o console se você tiver muitos chunks, use com cautela ou só se não funcionar)
        // TraceLog(LOG_INFO, "Chunk [%d, %d] Dist: %d/%d (Limite: %d)", c.cx, c.cz, distX, distZ, deleteDistance);

        if (distX > deleteDistance || distZ > deleteDistance) {
            
            if (c.built) {
                // AQUI VAI APARECER O LOG IGUAL AO DO SEU CUBO
                UnloadModel(c.model); 
            }

            // Remove do vetor
            chunks[i] = chunks.back();
            chunks.pop_back();
            
        } else {
            i++; 
        }
    }
}