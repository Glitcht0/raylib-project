#include "gameObjects.h"





Cube::Cube(Vector3 pos, Vector3 sz, Color col) {
    position = pos;
    size = sz;
    color = col;
    rotation = { 0, 0, 0};
}


void Cube::update(float dt){
    // exemplo
    //position.y = 1.0f;
    //rotation.y += 60.0f * dt;
    //rotation.z += 40.0f * dt;
}


void Cube::draw(){

    rlPushMatrix();

    rlTranslatef(position.x, position.y, position.z);

    rlRotatef(rotation.y, 0, 1, 0);
    rlRotatef(rotation.x, 1, 0, 0);
    rlRotatef(rotation.z, 0, 0, 1);
    
    DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, size.x, size.y, size.z, color);


    if (is_selected) {
        DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, size.x + 0.01f, size.y + 0.01f, size.z + 0.01f, ORANGE);
    }

    rlPopMatrix();
}