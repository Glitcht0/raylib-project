#pragma once
#include "src/core/config.h"
#include "src/Game/GameObjects/gameObjects.h"
#include "raylib.h"
#include <unordered_map>
#include <vector>




struct ObjectsChunck {
    std::vector<GameObject*> objects;
};


class structures {
public: 
    structures();
    ~structures();


    void update(Vector3 playerPos);
    void draw();






private: 

    std::unordered_map<long long, ObjectsChunck> objectData;
    long long ObjectKey(int cx, int cz) { return ((long long)cx << 32) | (unsigned int)cz; }


};