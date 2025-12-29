#pragma once
#include "raylib.h"

class GameObject{
public:
    Vector3 position;

    virtual ~GameObject(){}
    virtual void update(float dt) {}
    virtual void draw() = 0;
};



class Cube : public GameObject {
public:
    Vector3 size;
    Color color;

    Cube(Vector3 pos, Vector3 sz, Color col);

    void update(float dt) override;

    void draw() override;
};
