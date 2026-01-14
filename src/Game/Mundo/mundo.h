#pragma once
#include "raylib.h"
#include "raymath.h"
#include "src/core/config.h"
#include <cstdlib> // para rand()
#include <ctime>   // para time()
#include "src/engine/Utils/PerlinNoise.hpp"
#include <unordered_map>
#include <vector>


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
    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
    bool built = false;
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

    std::unordered_map<long long, TileChunk> chunkData;


    TileChunk& getChunk(int cx, int cz);
    Tile* GetTile(int globalX, int globalZ);
    TileChunk* GetTileChunk(int cx, int cz);
    bool Get_walkTileWorld(Vector3 pos, float halfSize);
    Vector3 Get_Spaw(float halfSize);

    void updateChunks(Vector3 playerPos);
    void updateTileChunks(Vector3 playerPos);
    void unloadFarChunks(Vector3 playerPos);

    void setShader(Shader s);

    

    

private:
    bool mundo_gerado = false;
    bool terrainBuilt = false;


    float terrain_elevation = 1.4f, terrain_raio = 1.4f;
    double scale = 0.03; //escala
    int octaves = 5; //detalhes
    siv::PerlinNoise perlin;
    
    


   
    void gerarmundo();
    void CreateIsland(int zpos, int xpos, int largura, int altura, float raio, float elevacao);

    void buildChunkMesh(Chunk& chunk);
    void buildTileChunk(TileChunk& chunk);


    int countSameNeighbors(int x, int z);
    void applyRules(int zpos, int xpos, int largura, int altura);
    TileType mostCommonNeighbor(int x, int z);



    Model terrainModel;
    Shader terrainShader;

    std::vector<Chunk> chunks;         // todos
    std::vector<int> visibleChunks; // só visíveis

    std::vector<TileChunk> tileChunks;         // todos
    std::vector<TileChunk*> tileVisibleChunks; // só visíveis

    

    long long ChunkKey(int cx, int cz) { return ((long long)cx << 32) | (unsigned int)cz; }



};
