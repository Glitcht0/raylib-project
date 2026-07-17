#pragma once
#include "Projects/TheGame/config.h"
#include "Projects/TheGame/States/StateGame/GameObjects/gameObjects.h"
#include "raylib.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <mutex> 
#include <deque>

#include <cmath>
#include <filesystem>
#include <cstdio>
#include <cstdio>
#include <algorithm>

// Configurações locais (ou pegue do config.h se tiver)
#ifndef OBJ_CHUNK_SIZE
#define OBJ_CHUNK_SIZE 16 // Mesmo tamanho do chunk do terreno geralmente
#endif

#define OBJ_VIEW_DISTANCE 3 // Quantos chunks de objetos carregar ao redor

// Struct simples para salvar no disco (Serialização)
struct ObjectSaveData {
    int type;        // 0 = Cubo Teste, 1 = Arvore, etc...
    Vector3 position;
    Vector3 scale;
    Color color;
};

struct ObjectsChunck {
    int cx, cz;
    std::vector<GameObject*> objects;
};

class structures {
public: 
    // Precisamos do nome do mundo para saber a pasta de save
    structures(std::string worldName); 
    ~structures();

    void update(Vector3 playerPos);
    void draw();

    bool CheckCollision(Vector3 position, float radius);
    

private: 
    std::string nomeMundo;
    std::unordered_map<long long, ObjectsChunck> objectData;

    long long ObjectKey(int cx, int cz) { return ((long long)cx << 32) | (unsigned int)cz; }

    // --- Lógica Interna ---
    void ensureSaveDirectory();
    void saveToDisk(const ObjectsChunck& chunk);
    bool loadFromDisk(int cx, int cz, ObjectsChunck& chunk);
    void generateChunk(int cx, int cz, ObjectsChunck& chunk);
    bool CheckCollisionCircleAABB_XZ(Vector3 center, float radius, const BoundingBox& box);
    void loadNearbyObjects(int cx, int cz);
    void unloadObjectsFarAway(int cx, int cz);

    // =========================================================
    // 🧵 SISTEMA DE THREADS 
    // =========================================================
    std::mutex resultMutex;
    std::deque<ObjectsChunck> loadedChunks; // Fila de resultados prontos
    std::vector<long long> chunksBeingProcessed; // Evita pedir o mesmo chunk 2x
    
    void requestChunkLoad(int cx, int cz);
    void processLoadedChunks();
    void loadTask(int cx, int cz); // O que roda na Thread
};