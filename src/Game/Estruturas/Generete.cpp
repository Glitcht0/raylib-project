#include "Estruturas.h"


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