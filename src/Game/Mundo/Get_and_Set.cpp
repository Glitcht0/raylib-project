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

        if (tx < 0 || tz < 0 || tx >= WORLD_W || tz >= WORLD_H)
            return false; // ignora ponto fora do mapa

        bool blocked = world[tz][tx].flags & TILE_BLOCKED;
        if (blocked)
            return false;
    }

    return true;
}


void World::setShader(Shader s) {
    terrainShader = s;
    if (terrainBuilt) {
        terrainModel.materials[0].shader = terrainShader;
    }
}



