#include "mundo.h"


Vector3 World::Get_Spaw(float halfSize){

    int z = 0, x = 0;


    for (z=20; z<WORLD_H;z++){
        for (x = 20; x < WORLD_W; x++){
            Vector3 pos = { x + 0.5f, 0.0f, z+ 0.5f };

            if (Get_walkTileWorld(pos, halfSize)) {
                return pos;
            }
        }
    }

    return { 0.5f, 0.0f, 0.5f };
}



bool World::Get_walkTileWorld(Vector3 pos, float halfSize) {
    Vector3 checks[4] = {
        { pos.x - halfSize, 0, pos.z - halfSize },
        { pos.x + halfSize, 0, pos.z - halfSize },
        { pos.x - halfSize, 0, pos.z + halfSize },
        { pos.x + halfSize, 0, pos.z + halfSize },
    };

    for (int i = 0; i < 4; i++) {
        int tx = (int)floor(checks[i].x);
        int tz = (int)floor(checks[i].z);

        // CORREÇÃO: Usar GetTile em vez de world[tz][tx]
        // Isso garante que estamos lendo do Chunk, e não da array antiga.
        Tile* tile = GetTile(tx, tz);

        // Se o tile for nulo (fora do mundo carregado) ou for bloqueado:
        if (tile == nullptr) {
            return false; // Ou true, dependendo se você quer bloquear bordas infinitas
        }

        if (tile->flags & TILE_BLOCKED) {
            return false;
        }
    }

    return true;
}

void World::setShader(Shader s) {
    terrainShader = s;
    if (terrainBuilt) {
        terrainModel.materials[0].shader = terrainShader;
    }
}




// Retorna ponteiro para o TileChunk. Se não existir, cria um vazio.
TileChunk* World::GetTileChunk(int cx, int cz) {
    long long key = ChunkKey(cx, cz);
    
    // Se o chunk ainda não existe no mapa, ele é criado automaticamente pelo operator []
    return &chunkData[key];
}

// Retorna ponteiro para um Tile específico.
Tile* World::GetTile(int globalX, int globalZ) {

    int cx = (int)floor((float)globalX / CHUNK_SIZE);
    int cz = (int)floor((float)globalZ / CHUNK_SIZE);

    // Ajuste para coordenadas locais dentro do chunk (lida com negativos corretamente)
    int localX = globalX % CHUNK_SIZE;
    int localZ = globalZ % CHUNK_SIZE;

    if (localX < 0) localX += CHUNK_SIZE;
    if (localZ < 0) localZ += CHUNK_SIZE;

    TileChunk* tc = GetTileChunk(cx, cz);
    
    if (!tc) return nullptr;

    return &tc->tiles[localZ][localX];
}