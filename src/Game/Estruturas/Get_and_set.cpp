#include "Estruturas.h"



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
