#include "mundo.h"





void World::generateSand() {
    for (int z = 1; z < WORLD_H - 1; z++) {
        for (int x = 1; x < WORLD_W - 1; x++) {

            if (world[z][x].type != TILE_GRASS &&
                world[z][x].type != TILE_DIRT)
                continue;

            bool nearWater = false;

            for (int dz = -1; dz <= 1; dz++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (world[z + dz][x + dx].type == TILE_WATER) {
                        nearWater = true;
                        break;
                    }
                }
            }

            if (nearWater)
                world[z][x].type = TILE_SAND;
        }
    }
}



Vector3 World::TileToWorld(int x, int z) {
    return (Vector3){ (float)x + 0.5f, 0.0f, (float)z + 0.5f };
}



void World::buildTerrainMesh() {
    Mesh mesh = { 0 };

    int tileCount = WORLD_W * WORLD_H;
    int vertexCount = tileCount * 6;

    mesh.vertexCount = vertexCount;
    mesh.triangleCount = vertexCount / 3;

    mesh.vertices = (float*)MemAlloc(vertexCount * 3 * sizeof(float));
    mesh.colors   = (unsigned char*)MemAlloc(vertexCount * 4 * sizeof(unsigned char));
    mesh.normals = (float*)MemAlloc(vertexCount * 3 * sizeof(float));

    int v = 0;
    int c = 0;
    int n = 0;

    for (int z = 0; z < WORLD_H; z++) {
        for (int x = 0; x < WORLD_W; x++) {

            Vector3 p = TileToWorld(x, z);

            Color col;
            switch (world[z][x].type) {
                case TILE_GRASS: col = COR_GRAMA_VERDE; break;
                case TILE_DIRT:  col = BROWN; break;
                case TILE_WATER: col = BLUE; break;
                case TILE_SAND:  col = COR_AREIA; break;
            }

            Vector3 v0 = { p.x - 0.5f, 0.0f, p.z - 0.5f };
            Vector3 v1 = { p.x + 0.5f, 0.0f, p.z - 0.5f };
            Vector3 v2 = { p.x - 0.5f, 0.0f, p.z + 0.5f };
            Vector3 v3 = { p.x + 0.5f, 0.0f, p.z + 0.5f };

            Vector3 quad[6] = { v0, v2, v1,  v1, v2, v3 };


            for (int i = 0; i < 6; i++) {
                mesh.vertices[v++] = quad[i].x;
                mesh.vertices[v++] = quad[i].y;
                mesh.vertices[v++] = quad[i].z;

                mesh.colors[c++] = col.r;
                mesh.colors[c++] = col.g;
                mesh.colors[c++] = col.b;
                mesh.colors[c++] = col.a;

                mesh.normals[n++] = 0.0f;
                mesh.normals[n++] = 1.0f;
                mesh.normals[n++] = 0.0f;
            }
        }
    }

    UploadMesh(&mesh, false);
    terrainModel = LoadModelFromMesh(mesh);
    terrainBuilt = true;
}