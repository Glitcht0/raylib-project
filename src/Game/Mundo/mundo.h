#pragma once
#include "raylib.h"
#include "raymath.h"
#include "src/core/config.h"
#include <cstdlib> // para rand()
#include <ctime>   // para time()
#include "src/engine/Utils/PerlinNoise.hpp"



#define COR_GRAMA_VERDE (Color){ 36, 76, 10, 255 }
#define COR_AREIA (Color){ 225, 193, 129, 255 }

enum TileType : uint8_t{
    TILE_GRASS,
    TILE_DIRT,
    TILE_WATER,
    TILE_SAND
};

enum TileFlags {
    TILE_BLOCKED = 1 << 0
};

struct Tile {
    TileType type;
    uint8_t flags;
};

struct TileChunk {
    int cx, cz;
    Tile tiles[TILE_CHUNK_SIZE][TILE_CHUNK_SIZE];
    bool dirty;   // precisa rebuildar mesh
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

    std::unordered_map<long long, TileChunk> tileChunks;

    TileChunk& getChunk(int cx, int cz);


    

    bool Get_walkTileWorld(Vector3 pos, float halfSize);
    Vector3 Get_Spaw(float halfSize);
    void updateChunks(Vector3 playerPos);
    void setShader(Shader s);

    

    

private:

    float terrain_elevation = 1.4f, terrain_raio = 1.4f;
    siv::PerlinNoise perlin;
    Vector3 TileToWorld(int x, int z);
    void gerarmundo();
    void generateSand();
    void buildTerrainMesh();
    void generateChunk(TileChunk& chunk);
    void CreateIsland(int zpos, int xpos, int largura, int altura, float raio, float elevacao);

    int countSameNeighbors(int x, int z);
    void applyRules(int zpos, int xpos, int largura, int altura);
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
