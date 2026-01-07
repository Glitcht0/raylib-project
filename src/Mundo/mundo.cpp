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

            DrawCube(pos, 1.0f, 0.05f, 1.0f, color);
        }
    }
    
}


void World::gerarmundo() {
    for (int z = 0; z < WORLD_H; z++) {
        for (int x = 0; x < WORLD_W; x++) {

            double n = perlin.octave2D_01( x * scale, z * scale, octaves);

            if (n < 0.3) {
                world[z][x].type = TILE_WATER;
                world[z][x].blocked = true;
            }
            else if (n < 0.5) {
                world[z][x].type = TILE_DIRT;
                world[z][x].blocked = false;
            }
            else {
                world[z][x].type = TILE_GRASS;
                world[z][x].blocked = false;
            }
        }
    }
    
    applyRules();
        
}


Vector3 World::TileToWorld(int x, int z) {
    return (Vector3){ (float)x + 0.5f, 0.0f, (float)z + 0.5f };
}



void World::applyRules() {
    for (int z = 0; z < WORLD_H; z++) {
        for (int x = 0; x < WORLD_W; x++) {

            int neighbors = countSameNeighbors(x, z);

            // regra: tile isolado ou quase isolado
            if (neighbors <= 1) {
                // converte para o tipo mais comum ao redor
                TileType t = mostCommonNeighbor(x, z);
                world[z][x].type = t;
                world[z][x].blocked = (t == TILE_WATER);
            }
        }
    }
}

TileType World::mostCommonNeighbor(int x, int z) {
    int countGrass = 0, countDirt = 0, countWater = 0;

    auto count = [&](int nx, int nz) {
        switch (world[nz][nx].type) {
        case TILE_GRASS: countGrass++; break;
        case TILE_DIRT:  countDirt++;  break;
        case TILE_WATER: countWater++; break;
        }
    };

    if (x > 0) count(x - 1, z);
    if (x < WORLD_W - 1) count(x + 1, z);
    if (z > 0) count(x, z - 1);
    if (z < WORLD_H - 1) count(x, z + 1);

    if (countGrass >= countDirt && countGrass >= countWater) return TILE_GRASS;
    if (countDirt >= countWater) return TILE_DIRT;
    return TILE_WATER;
}




int World::countSameNeighbors(int x, int z) {
    TileType t = world[z][x].type;
    int count = 0;

    if (x > 0 && world[z][x - 1].type == t) count++;
    if (x < WORLD_W - 1 && world[z][x + 1].type == t) count++;
    if (z > 0 && world[z - 1][x].type == t) count++;
    if (z < WORLD_H - 1 && world[z + 1][x].type == t) count++;

    return count;
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


    for (z=0; z<WORLD_H;z++){
        for (x = 0; x < WORLD_W; x++){
            Vector3 pos = { x + 0.5f, 0.0f, z+ 0.5f };

            if (Get_walkTileWorld(pos, halfSize)) {
                return pos;
            }
        }
    }

    return { 0.5f, 0.0f, 0.5f };
}