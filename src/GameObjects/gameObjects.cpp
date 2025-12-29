#include "gameObjects.h"





Cube::Cube(Vector3 pos, Vector3 sz, Color col) {
    position = pos;
    size = sz;
    color = col;
}


void Cube::update(float dt){
    // exemplo
    position.y = 1.0f;
}


void Cube::draw(){
    DrawCube(position, size.x, size.y, size.z, color);
}