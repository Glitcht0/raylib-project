#include "mundo.h"


void World::applyRules(int zpos, int xpos, int largura, int altura) {


    // ===== CLAMP DOS LIMITES DO MUNDO =====
    int zEnd = zpos + altura;
    int xEnd = xpos + largura;

    if (zpos < 0) zpos = 0;
    if (xpos < 0) xpos = 0;
    if (zEnd > WORLD_H) zEnd = WORLD_H;
    if (xEnd > WORLD_W) xEnd = WORLD_W;


    for (int z = zpos; z < zEnd; z++) {
        for (int x = xpos; x < xEnd; x++) {

            int neighbors = countSameNeighbors(x, z);

            // regra: tile isolado ou quase isolado
            if (neighbors <= 1) {
                TileType t = mostCommonNeighbor(x, z);
                world[z][x].type = t;

                if (t == TILE_WATER)
                    world[z][x].flags |= TILE_BLOCKED;    // seta
                else
                    world[z][x].flags &= ~TILE_BLOCKED;   // limpa
            }

        }
    }

}



TileType World::mostCommonNeighbor(int x, int z) {
    int grass = 0, dirt = 0, water = 0;

    auto count = [&](int nx, int nz) {
        switch (world[nz][nx].type) {
        case TILE_GRASS: grass++; break;
        case TILE_DIRT:  dirt++;  break;
        case TILE_WATER: water++; break;
        default: break; // IGNORA SAND
        }
    };

    if (x > 0) count(x - 1, z);
    if (x < WORLD_W - 1) count(x + 1, z);
    if (z > 0) count(x, z - 1);
    if (z < WORLD_H - 1) count(x, z + 1);

    if (water >= grass && water >= dirt) return TILE_WATER;
    if (grass >= dirt) return TILE_GRASS;
    return TILE_DIRT;
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
