#include "mundo.h"







void World::CreateIsland(int zpos, int xpos, int largura, int altura, float raio, float elevacao){
    // ===== CLAMP DOS LIMITES DO MUNDO =====
    int zEnd = zpos + altura;
    int xEnd = xpos + largura;

    if (zpos < 0) zpos = 0;
    if (xpos < 0) xpos = 0;
    if (zEnd > WORLD_H) zEnd = WORLD_H;
    if (xEnd > WORLD_W) xEnd = WORLD_W;


    // ===== GERA A ILHA =====
    float cx = xpos + largura * 0.5f;
    float cz = zpos + altura * 0.5f;

    for (int z = zpos; z < zEnd; z++) {
        for (int x = xpos; x < xEnd; x++) {


            double n = perlin.octave2D_01( x * scale, z * scale, octaves);



            float dx = (x - cx) / (largura * 0.5f * raio);
            float dz = (z - cz) / (altura  * 0.5f * raio);

            float dist = sqrtf(dx*dx + dz*dz);
            float mask = 1.0f - Clamp(dist, 0.0f, 1.0f);

            mask = powf(mask, elevacao);  // Numero maior, pico no centro, menor, mais plano

            // Altura final
            n = n * mask;

            // === Definição dos Tiles ===
            if (n < 0.15) {
                world[z][x].type = TILE_WATER;
                world[z][x].flags |= TILE_BLOCKED;

            }
            else if (n < 0.20) {
                world[z][x].type = TILE_SAND;
                world[z][x].flags &= ~TILE_BLOCKED;

            }
            else if (n < 0.25) {
                world[z][x].type = TILE_DIRT;
                world[z][x].flags &= ~TILE_BLOCKED;

            }
            else if (n<= 1.0) {
                world[z][x].type = TILE_GRASS;
                world[z][x].flags &= ~TILE_BLOCKED;

            }
            else {
                world[z][x].type = TILE_WATER;
                world[z][x].flags |= TILE_BLOCKED;

            }
            
            // 🔹 RUÍDO SUTIL NA PRAIA (AQUI)
            //if (world[z][x].type == TILE_SAND && n < 0.09) {
            //    world[z][x].type = TILE_WATER;
            //    world[z][x].blocked = true;
            //}
            
        }
    }

    applyRules(zpos, xpos, largura, altura);
    applyRules(zpos, xpos, largura, altura);
    applyRules(zpos, xpos, largura, altura);
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
    int v = 0, c = 0, n = 0; // ⬅️ declare antes de usar



    TileChunk* tileChunk = GetTileChunk(chunk.cx, chunk.cz);

    if (!tileChunk) return; // segurança
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            Tile& tile = tileChunk->tiles[z][x]; 

        
            Vector3 p = { (chunk.cx * CHUNK_SIZE + x) + 0.5f, 0.0f, (chunk.cz * CHUNK_SIZE + z) + 0.5f };

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



void World::processUnloadQueue(int maxPerFrame) {
    for (int i = 0; i < maxPerFrame && !unloadQueue.empty(); i++) {
        Model m = unloadQueue.back();
        unloadQueue.pop_back();

        UnloadModel(m);
    }
}

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



// Agenda construção de malha
void World::ensureChunkMeshBuilt(int chunkIndex) {
    Chunk& c = chunks[chunkIndex];

    if (!c.built && !c.building) {
        buildQueue.push_back(chunkIndex);
        c.building = true; // apenas marca como agendado
    }
}

