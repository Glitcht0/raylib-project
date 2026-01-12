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




void World::gerarmundo() {
    for (int z = 0; z < WORLD_H; z++) {
        for (int x = 0; x < WORLD_W; x++) {

            double n = perlin.octave2D_01( x * scale, z * scale, octaves);

            // === MÁSCARA DE ILHA ===
            float cx = WORLD_W * 0.5f;
            float cz = WORLD_H * 0.5f;

            float islandRadius = 1.4f; // Tamanho da ilha

            float dx = (x - cx) / (cx * islandRadius);
            float dz = (z - cz) / (cz * islandRadius);

            float dist = sqrtf(dx*dx + dz*dz);

            float mask = 1.0f - Clamp(dist, 0.0f, 1.0f);
            mask = powf(mask, 1.4f);  // Numero maior, pico no centro, menor, mais plano

            // Altura final
            n = n * mask;

            // === Definição dos Tiles ===
            if (n < 0.15) {
                world[z][x].type = TILE_WATER;
                world[z][x].blocked = true;

            }
            else if (n < 0.20) {
                world[z][x].type = TILE_SAND;
                world[z][x].blocked = false;
            }
            else if (n < 0.25) {
                world[z][x].type = TILE_DIRT;
                world[z][x].blocked = false;
            }
            else {
                world[z][x].type = TILE_GRASS;
                world[z][x].blocked = false;
            }
            
            // 🔹 RUÍDO SUTIL NA PRAIA (AQUI)
            //if (world[z][x].type == TILE_SAND && n < 0.09) {
            //    world[z][x].type = TILE_WATER;
            //    world[z][x].blocked = true;
            //}
            
        }
    }


    
    applyRules();
    applyRules();
    applyRules();

    // ===== Dividir em chunks ====
    int chunksX = WORLD_W / CHUNK_SIZE;
    int chunksZ = WORLD_H / CHUNK_SIZE;
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












// Controi as meshs das Chunks
void World::buildChunkMesh(Chunk& chunk) {
    Mesh mesh = { 0 };

    int tiles = CHUNK_SIZE * CHUNK_SIZE;
    int vertexCount = tiles * 6;

    mesh.vertexCount = vertexCount;
    mesh.triangleCount = vertexCount / 3;

    mesh.vertices = (float*)MemAlloc(vertexCount * 3 * sizeof(float));
    mesh.colors   = (unsigned char*)MemAlloc(vertexCount * 4 * sizeof(unsigned char));
    mesh.normals  = (float*)MemAlloc(vertexCount * 3 * sizeof(float));

    int v = 0, c = 0, n = 0;

    int startX = chunk.cx * CHUNK_SIZE;
    int startZ = chunk.cz * CHUNK_SIZE;

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {

            int wx = startX + x;
            int wz = startZ + z;

            Tile& tile = world[wz][wx];

            Vector3 p = { wx + 0.5f, 0.0f, wz + 0.5f };

            Color col;
            switch (tile.type) {
                case TILE_GRASS: col = COR_GRAMA_VERDE; break;
                case TILE_DIRT:  col = BROWN; break;
                case TILE_WATER: col = BLUE; break;
                case TILE_SAND:  col = COR_AREIA; break;
            }

            Vector3 v0 = { p.x - 0.5f, 0, p.z - 0.5f };
            Vector3 v1 = { p.x + 0.5f, 0, p.z - 0.5f };
            Vector3 v2 = { p.x - 0.5f, 0, p.z + 0.5f };
            Vector3 v3 = { p.x + 0.5f, 0, p.z + 0.5f };

            Vector3 quad[6] = { v0, v2, v1, v1, v2, v3 };

            for (int i = 0; i < 6; i++) {
                mesh.vertices[v++] = quad[i].x;
                mesh.vertices[v++] = quad[i].y;
                mesh.vertices[v++] = quad[i].z;

                mesh.colors[c++] = col.r;
                mesh.colors[c++] = col.g;
                mesh.colors[c++] = col.b;
                mesh.colors[c++] = col.a;

                mesh.normals[n++] = 0;
                mesh.normals[n++] = 1;
                mesh.normals[n++] = 0;
            }
        }
    }

    UploadMesh(&mesh, false);
    chunk.model = LoadModelFromMesh(mesh);
    chunk.built = true;

    if (terrainShader.id != 0)
        chunk.model.materials[0].shader = terrainShader;
}
