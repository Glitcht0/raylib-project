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


void World::generateChunk(TileChunk& chunk) {
    for (int z = 0; z < TILE_CHUNK_SIZE; z++)
    for (int x = 0; x < TILE_CHUNK_SIZE; x++) {

       double n = perlin.octave2D_01( x * scale, z * scale, octaves);



        float dx = (x - cx) / (largura * 0.5f * terrain_raio);
        float dz = (z - cz) / (altura  * 0.5f * terrain_raio);

        float dist = sqrtf(dx*dx + dz*dz);
        float mask = 1.0f - Clamp(dist, 0.0f, 1.0f);

        mask = powf(mask, terrain_elevation);  // Numero maior, pico no centro, menor, mais plano

        // Altura final
        n = n * mask;

        // === Definição dos Tiles ===
        if (n < 0.15) {
            chunk.tiles[z][x].type = TILE_WATER;
            chunk.tiles[z][x].flags |= TILE_BLOCKED;

        }
        else if (n < 0.20) {
            chunk.tiles[z][x].type = TILE_SAND;
            chunk.tiles[z][x].flags &= ~TILE_BLOCKED;

        }
        else if (n < 0.25) {
            chunk.tiles[z][x].type = TILE_DIRT;
            chunk.tiles[z][x].flags &= ~TILE_BLOCKED;

        }
        else if (n<= 1.0) {
            chunk.tiles[z][x].type = TILE_GRASS;
            chunk.tiles[z][x].flags &= ~TILE_BLOCKED;

        }
        else {
            chunk.tiles[z][x].type = TILE_WATER;
            chunk.tiles[z][x].flags |= TILE_BLOCKED;

        }
    }
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

            if (wx < 0 || wx >= WORLD_W || wz < 0 || wz >= WORLD_H)
                continue;

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
