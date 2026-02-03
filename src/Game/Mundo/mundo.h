#pragma once

// =============================================================
// 📦 INCLUDES & DEPENDÊNCIAS
// =============================================================
#include "raylib.h"
#include "raymath.h"
#include "src/core/config.h"
#include "src/engine/Utils/PerlinNoise.hpp"

#include <vector>
#include <string>
#include <unordered_map>
#include <deque>
#include <mutex>
#include <atomic>
#include <cstdlib> 
#include <ctime>   


// =============================================================
// 🎨 DEFINIÇÕES E CONSTANTES
// =============================================================
#define COR_GRAMA_VERDE (Color){ 36, 76, 10, 255 }
#define COR_AREIA       (Color){ 225, 193, 129, 255 }





// =============================================================
// 🧱 ESTRUTURAS DE DADOS (Tipos básicos)
// =============================================================

enum TileType : uint8_t {
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

// Dados Lógicos (o que é salvo no disco)
struct TileChunk {
    int cx, cz;  
    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
    bool built = false;
};

// Dados Visuais (o modelo 3D)
struct Chunk {
    int cx, cz;
    Model model;
    bool built = false;
    bool building = false;
};

struct PlayerData {
    Vector3 position;
    float halfSize;
};












// =============================================================
// 🌍 CLASSE WORLD
// =============================================================
/*

*/
class World {
public:
    // --------------- 🛠️ Construtores e Ciclo de Vida ---------------
    World(std::string nomeM = "default");
    ~World();

    void update(Vector3 playerPos);
    void draw();


    // ---------------📍 Getters & Queries (Acesso a dados) ---------------
    Tile* GetTile(int globalX, int globalZ);
    TileChunk* GetTileChunk(int cx, int cz);
    TileChunk& getOrCreateTileChunk(int chunkX, int chunkZ); // Retorna ref segura
    
    // Auxiliares de posição
    bool    Get_walkTileWorld(Vector3 pos, float halfSize);
    Vector3 Get_Spaw(float halfSize);
    void    getPlayerChunk(Vector3 playerPos, int& cx, int& cz);
    
    // --------------- 💾 Persistência e Jogador ---------------
    Vector3 loadplayer(float halfSize);
    void    savePlayerPosition(Vector3 position);


    // --------------- 🎨 Configuração Visual ---------------
    void setShader(Shader s);

    // --------------- ⚠️ Dados Públicos (Cuidado com acesso direto) --------------- 
    std::string nomeMundo;
    PlayerData jogador;
    
    // ARRAY GIGANTE ESTÁTICO
    Tile world[WORLD_H][WORLD_W]; 

private:
    // =========================================================
    // 🔒 DADOS INTERNOS (Privado)
    // =========================================================

    // --- Estado do Mundo ---
    bool mundo_gerado = false;
    bool terrainBuilt = false;
    
    // --- Renderização (Meshes e Filas) ---
    Shader terrainShader;
    Model terrainModel; // Modelo do terreno estático inicial
    
    std::vector<Chunk> chunks;         // Pool de chunks visuais
    std::vector<int> visibleChunks;    // Índices dos chunks visíveis
    
    std::vector<Model> unloadQueue;    // Modelos para deletar da GPU
    std::deque<std::pair<int, int>> buildQueue;     // Chunks na fila para criar malha

    // --- Dados Lógicos (Hash Map) ---
    // A chave é um long long (int cx, int cz combinados)
    std::unordered_map<long long, TileChunk> chunkData; 
    std::vector<long long> tileVisibleChunks;

    // --- Geração Procedural (Perlin) ---
    siv::PerlinNoise perlin;
    float terrain_elevation = 1.4f;
    float terrain_raio = 1.4f;
    double scale = 0.03;
    int octaves = 5;

    // =========================================================
    // 🧵 SISTEMA DE THREADS (Async Loading)
    // =========================================================
    std::mutex resultMutex;                 // Protege as listas de resultado
    std::deque<TileChunk> loadedChunks;     // Chunks prontos vindos da thread
    std::vector<long long> chunksBeingProcessed; // Chunks que já estão sendo carregados

    // =========================================================
    // ⚙️ FUNÇÕES INTERNAS (Helpers)
    // =========================================================

    // --- Gerenciamento de Chunks ---
    void updateChunks(Vector3 playerPos);     // Decide quem carrega/descarrega
    void unloadFarChunks(Vector3 playerPos, int maxUnloads = 1);  // Remove quem está longe
    
    void processUnloadQueue(int maxPerFrame = 1);
    void processBuildQueue(int maxPerFrame);
    
    int  getOrCreateMeshChunk(int chunkX, int chunkZ);
    void ensureChunkMeshBuilt(int chunkIndex);
    void buildChunkMesh(Chunk& chunk);
    

    // --- Geração de Terreno ---
    void gerarmundo(); // Orquestrador principal
    void CreatIsland(int xpos, int zpos, int largura, int altura);
    void CreateTerrain(int zpos, int xpos, int largura, int altura, float raio, float elevacao);
    void applyRules(int zpos, int xpos, int largura, int altura); // Autotile/Suavização
    void LimparBufferComAgua();
    void AtualizarESalvarRegiao(int xpos, int zpos, int largura, int altura);
    
    // Auxiliares de Geração
    int      countSameNeighbors(int x, int z);
    TileType mostCommonNeighbor(int x, int z);
    void     InicializaChuncksRender(int largura, int altura);
    void     CarregarAreaInicial(int largura, int altura);

    // --- Sistema de Arquivos e Threads ---
    void requestChunkLoad(int cx, int cz);
    void processLoadedChunks();
    void generateOrLoadTask(int cx, int cz); // Executado dentro da thread
    void generateSingleChunk(TileChunk& tc);

    // IO Disco
    void ensureSaveDirectories();
    bool saveChunkToDisk(const TileChunk& tc);
    bool loadChunkFromDisk(int cx, int cz, TileChunk& outChunk);

    // --- Utilitários Inline ---
    inline long long ChunkKey(int cx, int cz) { 
        return ((long long)cx << 32) | (unsigned int)cz; 
    }
};