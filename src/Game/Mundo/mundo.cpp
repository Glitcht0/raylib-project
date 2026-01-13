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
    updateChunks(playerPos);
}


void World::draw() {
    for (Chunk* chunk : visibleChunks) {
        DrawModel(chunk->model, {0,0,0}, 1.0f, WHITE);
    }
}




void World::updateChunks(Vector3 playerPos) {
    int cx = (int)floor(playerPos.x / CHUNK_SIZE);
    int cz = (int)floor(playerPos.z / CHUNK_SIZE);

    visibleChunks.clear();

    for (Chunk& chunk : chunks) {
        if (abs(chunk.cx - cx) <= VIEW_DISTANCE &&
            abs(chunk.cz - cz) <= VIEW_DISTANCE) {

            if (!chunk.built) {
                buildChunkMesh(chunk); // builda na primeira necessidade
            }

            visibleChunks.push_back(&chunk);
        }
    }
}


TileChunk& World::getChunk(int cx, int cz) {
    auto key = std::make_pair(cx, cz);

    if (tileChunks.count(key) == 0) {
        TileChunk chunk;
        chunk.cx = cx;
        chunk.cz = cz;
        chunk.dirty = true;

        // 1️⃣ base: tudo água
        for (int z = 0; z < TILE_CHUNK_SIZE; z++)
        for (int x = 0; x < TILE_CHUNK_SIZE; x++) {
            chunk.tiles[z][x].type = TILE_WATER;
            chunk.tiles[z][x].flags = TILE_BLOCKED;
        }

        // 2️⃣ geração procedural LOCAL
        generateChunk(chunk);

        tileChunks[key] = chunk;
    }

    return tileChunks[key];
}



void World::gerarmundo() {

    for (int z = 0; z < WORLD_H; z++) {
        for (int x = 0; x < WORLD_W; x++) {
            world[z][x].type = TILE_WATER;
            world[z][x].flags = 0;
            world[z][x].flags |= TILE_BLOCKED;


        }
    }

    CreateIsland(100, 100, 200, 200, 1.4f, 1.4f);


    


    // ===== Criar chunks ====
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


    //generateSand();
    //buildTerrainMesh();

}









