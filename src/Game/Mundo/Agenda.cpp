#include "mundo.h"




// Agenda construção de malha
void World::ensureChunkMeshBuilt(int chunkIndex) {
    Chunk& c = chunks[chunkIndex];

    if (!c.built && !c.building) {
        buildQueue.push_back(chunkIndex);
        c.building = true; // apenas marca como agendado
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
        int index = buildQueue.back();
        buildQueue.pop_back();

        Chunk& c = chunks[index];

        buildChunkMesh(c);   // cria Model
        c.built = true;      // AGORA SIM
        c.building = false;
    }
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


