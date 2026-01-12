#pragma once
#include "raylib.h"
#include "raymath.h"
#include "src/core/config.h"
#include <cstdlib> // para rand()
#include <ctime>   // para time()
#include "src/engine/Utils/PerlinNoise.hpp"

static const int WORLD_W = 200;
static const int WORLD_H = 200;

static const int CHUNK_SIZE = 16;
static const int VIEW_DISTANCE = 10;

#define COR_GRAMA_VERDE (Color){ 36, 76, 10, 255 }
#define COR_AREIA (Color){ 225, 193, 129, 255 }

enum TileType {
    TILE_GRASS,
    TILE_DIRT,
    TILE_WATER,
    TILE_SAND
};

struct Tile {
    TileType type;
    bool blocked;
};


struct Chunk {
    int cx, cz;        // coordenada do chunk no grid
    Model model;
    bool built = false;
};

class World {
public:

    World();
    ~World();
    void update(Vector3 playerPos);
    void draw();
    Tile world[WORLD_H][WORLD_W];

    

    bool Get_walkTileWorld(Vector3 pos, float halfSize);
    Vector3 Get_Spaw(float halfSize);
    void updateChunks(Vector3 playerPos);
    void setShader(Shader s);

    

    

private:

    
    siv::PerlinNoise perlin;
    Vector3 TileToWorld(int x, int z);
    void gerarmundo();
    void generateSand();
    void buildTerrainMesh();

    int countSameNeighbors(int x, int z);
    void applyRules();
    TileType mostCommonNeighbor(int x, int z);

    double scale = 0.03; //escala
    int octaves = 5; //detalhes

    Model terrainModel;
    bool terrainBuilt = false;
    Shader terrainShader;

    std::vector<Chunk> chunks;         // todos
    std::vector<Chunk*> visibleChunks; // só visíveis

    void buildChunkMesh(Chunk& chunk);



};
