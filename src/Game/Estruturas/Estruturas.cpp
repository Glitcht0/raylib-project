#include "Estruturas.h"
#include <cmath>
#include <filesystem>
#include <cstdio>

namespace fs = std::filesystem;

structures::structures(std::string worldName) {
    this->nomeMundo = worldName;
    ensureSaveDirectory();
}

structures::~structures() {
    // Salva tudo que está na memória antes de fechar
    for (auto& pair : objectData) {
        saveToDisk(pair.second);
        // Limpa a memória dos ponteiros
        for (GameObject* obj : pair.second.objects) {
            delete obj;
        }
    }
    objectData.clear();
}

void structures::ensureSaveDirectory() {
    std::string path = "saves/" + nomeMundo + "/objects/";
    if (!fs::exists(path)) {
        fs::create_directories(path);
    }
}

// ==========================================================
//                 GERENCIADOR (UPDATE)
// ==========================================================
void structures::update(Vector3 playerPos) {
    int cx = (int)floor(playerPos.x / OBJ_CHUNK_SIZE);
    int cz = (int)floor(playerPos.z / OBJ_CHUNK_SIZE);

    // 1. CARREGAR CHUNKS PERTO
    for (int z = -OBJ_VIEW_DISTANCE; z <= OBJ_VIEW_DISTANCE; z++) {
        for (int x = -OBJ_VIEW_DISTANCE; x <= OBJ_VIEW_DISTANCE; x++) {
            int targetCX = cx + x;
            int targetCZ = cz + z;
            long long key = ObjectKey(targetCX, targetCZ);

            // Se não está carregado, carrega (ou gera)
            if (objectData.find(key) == objectData.end()) {
                loadChunk(targetCX, targetCZ);
            }
        }
    }

    // 2. DESCARREGAR CHUNKS LONGE (Histerese: Distância + 2)
    int unloadDist = OBJ_VIEW_DISTANCE + 2;
    
    // Iterador seguro para remover elementos enquanto percorre
    for (auto it = objectData.begin(); it != objectData.end(); ) {
        ObjectsChunck& chunk = it->second;
        
        int dist = (int)std::max(abs(chunk.cx - cx), abs(chunk.cz - cz));

        if (dist > unloadDist) {
            // Salva no disco e libera memória
            saveToDisk(chunk);
            
            for (GameObject* obj : chunk.objects) {
                delete obj; // Importante: Deletar o objeto da HEAP
            }

            it = objectData.erase(it); // Remove do mapa e avança iterador
        } else {
            ++it;
        }
    }

    // 3. Update individual dos objetos (se eles tiverem lógica, animação, etc)
    float dt = GetFrameTime();
    for (auto& pair : objectData) {
        for (GameObject* obj : pair.second.objects) {
            obj->update(dt); 
        }
    }
}

// ==========================================================
//                     DESENHO
// ==========================================================
void structures::draw() {
    for (auto& pair : objectData) {
        for (GameObject* obj : pair.second.objects) {
            obj->draw();
        }
    }
}

// ==========================================================
//              CARREGAMENTO / GERAÇÃO
// ==========================================================
void structures::loadChunk(int cx, int cz) {
    long long key = ObjectKey(cx, cz);
    ObjectsChunck newChunk;
    newChunk.cx = cx;
    newChunk.cz = cz;

    // Tenta ler do arquivo. Se falhar, gera novos.
    if (!loadFromDisk(cx, cz, newChunk)) {
        generateChunk(cx, cz, newChunk);
    }

    objectData[key] = newChunk;
}

void structures::generateChunk(int cx, int cz, ObjectsChunck& chunk) {
    // GERAÇÃO ALEATÓRIA DE TESTE (Cubos Vermelhos)
    
    // Chance de ter objetos no chunk (ex: 50%)
    if (GetRandomValue(0, 100) > 50) return; 

    int qtd = GetRandomValue(1, 4); // 1 a 4 objetos

    for (int i = 0; i < qtd; i++) {
        // Posição local dentro do chunk (0 a 15)
        float lx = (float)GetRandomValue(0, OBJ_CHUNK_SIZE - 1);
        float lz = (float)GetRandomValue(0, OBJ_CHUNK_SIZE - 1);

        // Posição Global
        Vector3 pos = {
            (cx * OBJ_CHUNK_SIZE) + lx + 0.5f,
            0.5f, // Altura flutuando um pouco
            (cz * OBJ_CHUNK_SIZE) + lz + 0.5f
        };

        // Cria o cubo e adiciona na lista
        // (Assumindo que você tem uma classe Cube herdando de GameObject)
        GameObject* obj = new Cube(pos, {1,1,1}, RED); 
        chunk.objects.push_back(obj);
    }
}

bool structures::CheckCollision(Vector3 position, float radius) {
    int cx = (int)floor(position.x / OBJ_CHUNK_SIZE);
    int cz = (int)floor(position.z / OBJ_CHUNK_SIZE);

    // Verifica o chunk atual e os 8 vizinhos (3x3 grid)
    for (int z = -1; z <= 1; z++) {
        for (int x = -1; x <= 1; x++) {
            long long key = ObjectKey(cx + x, cz + z);

            // Se o chunk existe na memória
            if (objectData.find(key) != objectData.end()) {
                ObjectsChunck& chunk = objectData[key];

                for (GameObject* obj : chunk.objects) {
                    if (!obj->HasCollision()) continue;

                    BoundingBox box = obj->GetBoundingBox();
                    
                    // Usa a função auxiliar
                    if (CheckCollisionCircleAABB_XZ(position, radius, box)) {
                        return true; // Colidiu!
                    }
                }
            }
        }
    }
    return false; // Caminho livre
}

// 🆕 Helper copiado/adaptado do Player para cá
bool structures::CheckCollisionCircleAABB_XZ(Vector3 center, float radius, const BoundingBox& box) {
    float closestX = Clamp(center.x, box.min.x, box.max.x);
    float closestZ = Clamp(center.z, box.min.z, box.max.z);

    float dx = center.x - closestX;
    float dz = center.z - closestZ;

    return (dx * dx + dz * dz) <= (radius * radius);
}

// ==========================================================
//                  SISTEMA DE ARQUIVOS (I/O)
// ==========================================================
void structures::saveToDisk(const ObjectsChunck& chunk) {
    if (chunk.objects.empty()) return; // Não salva chunks vazios pra economizar espaço

    std::string path = "saves/" + nomeMundo + "/objects/chunk_" + 
                       std::to_string(chunk.cx) + "_" + std::to_string(chunk.cz) + ".dat";
    
    FILE* f = fopen(path.c_str(), "wb");
    if (!f) return;

    // Cabeçalho: Quantos objetos tem neste arquivo?
    int count = (int)chunk.objects.size();
    fwrite(&count, sizeof(int), 1, f);

    // Salva cada objeto
    for (GameObject* obj : chunk.objects) {
        ObjectSaveData data;
        
        // Aqui você precisa identificar o tipo. 
        // Como exemplo simples, vou assumir que tudo é cubo (tipo 0)
        // No futuro, você pode usar um dynamic_cast ou um método virtual obj->getType()
        data.type = 0; 
        data.position = obj->position;
        data.scale = obj->scale;
        data.color = obj->color;

        fwrite(&data, sizeof(ObjectSaveData), 1, f);
    }

    fclose(f);
}

bool structures::loadFromDisk(int cx, int cz, ObjectsChunck& chunk) {
    std::string path = "saves/" + nomeMundo + "/objects/chunk_" + 
                       std::to_string(cx) + "_" + std::to_string(cz) + ".dat";
    
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) return false; // Arquivo não existe -> vai gerar novo

    int count = 0;
    fread(&count, sizeof(int), 1, f);

    for (int i = 0; i < count; i++) {
        ObjectSaveData data;
        fread(&data, sizeof(ObjectSaveData), 1, f);

        // Factory simples: recria o objeto baseado no tipo
        if (data.type == 0) {
            GameObject* obj = new Cube(data.position, data.scale, data.color);
            chunk.objects.push_back(obj);
        }
        // else if (data.type == 1) ... new Tree(...)
    }

    fclose(f);
    return true;
}