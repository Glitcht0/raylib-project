#include "mundo.h"


World::World(): perlin(time(nullptr)) {
    gerarmundo();
    //time(nullptr)
}



void World::update() {
    
}


void World::draw() {

    for (int z = 0; z < WORLD_H; z++) {
        for (int x = 0; x < WORLD_W; x++) {

            Vector3 pos = TileToWorld(x, z);

            Color color = COR_GRAMA;
            if (world[z][x].type == TILE_DIRT)  color = BROWN;
            if (world[z][x].type == TILE_GRASS)  color = COR_GRAMA_VERDE;
            if (world[z][x].type == TILE_WATER) color = BLUE;
            if (world[z][x].type == TILE_SAND) color = COR_AREIA;

            DrawCube(pos, 1.0f, 0.05f, 1.0f, color);
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

            float islandRadius = 1.85f; // Tamanho da ilha

            float dx = (x - cx) / (cx * islandRadius);
            float dz = (z - cz) / (cz * islandRadius);

            float dist = sqrtf(dx*dx + dz*dz);

            float mask = 1.0f - Clamp(dist, 0.0f, 1.0f);
            mask = powf(mask, 1.5f);

            // Altura final
            n = n * mask;


            if (n < 0.15) {
                world[z][x].type = TILE_WATER;
                world[z][x].blocked = true;

            }
            else if (n < 0.20) {
                world[z][x].type = TILE_SAND;
                world[z][x].blocked = false;
            }
            else if (n < 0.35) {
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

    //generateSand();



}


Vector3 World::TileToWorld(int x, int z) {
    return (Vector3){ (float)x + 0.5f, 0.0f, (float)z + 0.5f };
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

        if (tx < 0 || tz < 0 || tx >= WORLD_W || tz >= WORLD_H)
            continue; // ignora ponto fora do mapa


        if (world[tz][tx].blocked)
            return false;
    }

    return true;
}



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
