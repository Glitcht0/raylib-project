#pragma once
#include "raylib.h"
#include "raymath.h"
#include "src/core/config.h"
#include <cstdlib> // para rand()
#include <ctime>   // para time()
#include "src/engine/Utils/PerlinNoise.hpp"
#include <unordered_map>
#include <vector>



#include <thread>
#include <mutex>
#include <atomic>
#include <deque>
#include <string>


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
    bool building = false;
};

struct PlayerData {
    Vector3 position;
    float halfSize;
};

class World {
public:

    World(std::string nomeM = "default");
    ~World();
    void update(Vector3 playerPos);
    void draw();

    Tile world[WORLD_H][WORLD_W];

    std::string nomeMundo;

    


    TileChunk& getChunk(int cx, int cz);
    Tile* GetTile(int globalX, int globalZ);
    TileChunk* GetTileChunk(int cx, int cz);
    bool Get_walkTileWorld(Vector3 pos, float halfSize);
    Vector3 Get_Spaw(float halfSize);
    Vector3 loadplayer(float halfSize);
    void getPlayerChunk(Vector3 playerPos, int& cx, int& cz);
    int getOrCreateMeshChunk(int chunkX, int chunkZ);
    //void ensureChunkMeshBuilt(int chunkIndex);
    TileChunk& getOrCreateTileChunk(int chunkX, int chunkZ);

    void updateChunks(Vector3 playerPos);
    void updateTileChunks(Vector3 playerPos);
    void unloadFarChunks(Vector3 playerPos);
    void savePlayerPosition(Vector3 position);

    void setShader(Shader s);

    PlayerData jogador;

    

    

private:
    bool mundo_gerado = false;
    bool terrainBuilt = false;


    float terrain_elevation = 1.4f, terrain_raio = 1.4f;
    double scale = 0.03; //escala
    int octaves = 5; //detalhes
    siv::PerlinNoise perlin;
    
    


   
    void gerarmundo();
    void CreatIsland(int xpos, int zpos, int largura, int altura);
    void CreateTerrain(int zpos, int xpos, int largura, int altura, float raio, float elevacao);
    void InicializaChuncksRender(int largura, int altura);
    void CopiarTileParaMapa(int largura, int altura);

    void buildChunkMesh(Chunk& chunk);
    void processUnloadQueue(int maxPerFrame = 1);
    void processBuildQueue(int maxPerFrame);
    void ensureChunkMeshBuilt(int chunkIndex);
    void ensureSaveDirectories();

    

    

    void CarregarAreaInicial(int largura, int altura);


    int countSameNeighbors(int x, int z);
    void applyRules(int zpos, int xpos, int largura, int altura);
    TileType mostCommonNeighbor(int x, int z);



    Model terrainModel;
    Shader terrainShader;


    // 🏞️ Mapa para render
    std::vector<Chunk> chunks;         // todos
    std::vector<int> visibleChunks;    // só visíveis

    std::vector<Model> unloadQueue;
    std::vector<int> buildQueue;

    // 🧊 Mapa principal de TileChunks
    std::unordered_map<long long, TileChunk> chunkData;
    std::vector<long long> tileVisibleChunks;   // só visíveis


    

    

    long long ChunkKey(int cx, int cz) { return ((long long)cx << 32) | (unsigned int)cz; }



    
    // ===== 🧵 SISTEMA DE THREADS (MODIFICADO) =====
    
    // Mutex apenas para proteger a lista de resultados (loadedChunks) e de pedidos ativos
    std::mutex resultMutex;

    // Fila de retorno: A thread coloca aqui, o Main pega aqui
    std::deque<TileChunk> loadedChunks; 
    
    // Conjunto para rastrear o que JÁ ESTÁ sendo carregado (para não spammar a ThreadPool)
    std::vector<long long> chunksBeingProcessed; 

    // Funções atualizadas
    void requestChunkLoad(int cx, int cz);
    void processLoadedChunks();
    
    // Geração/Load Interno (usado pelas threads)
    void generateOrLoadTask(int cx, int cz); 

    // Salvar e Carregar
    bool saveChunkToDisk(const TileChunk& tc);
    bool loadChunkFromDisk(int cx, int cz, TileChunk& outChunk);
    void generateSingleChunk(TileChunk& tc);

};
