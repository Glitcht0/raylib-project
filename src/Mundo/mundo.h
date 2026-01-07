#pragma once
#include "raylib.h"
#include "src/config.h"
#include <cstdlib> // para rand()
#include <ctime>   // para time()
#include "src/Utils/PerlinNoise.hpp"

static const int WORLD_W = 40;
static const int WORLD_H = 40;

#define COR_GRAMA_VERDE (Color){ 36, 76, 10, 255 }

enum TileType {
    TILE_GRASS,
    TILE_DIRT,
    TILE_WATER
};

struct Tile {
    TileType type;
    bool blocked;
};




class World {
public:

    World();
    void update();
    void draw();
    Tile world[WORLD_H][WORLD_W];

    bool Get_walkTileWorld(Vector3 pos, float halfSize);
    Vector3 Get_Spaw(float halfSize);
    

    

private:

    
    siv::PerlinNoise perlin;
    Vector3 TileToWorld(int x, int z);
    void gerarmundo();

    int countSameNeighbors(int x, int z);
    void applyRules();
    TileType mostCommonNeighbor(int x, int z);

    double scale = 0.03; //escala
    int octaves = 5; //detalhes

};
