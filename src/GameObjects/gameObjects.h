#pragma once
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

class GameObject{
public:
    Vector3 position;
    Vector3 rotation;

    bool is_selected = false;
    

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

enum PlayerState {
    IDLE,
    WALK,
    RUN,
    ATTACK,
    INTERACT
};



class CameraObject : public GameObject {
public:


    Camera3D cam;
    float fovy;
    int projection;

    float yaw; //rotação horizontal
    float pitch; //rotação vertical
    float distance;
    Vector3 target;

    Vector3 initialPosition; 
    Vector3 initialTarget; 

    CameraObject(Vector3 pos, Vector3 target, Vector3 up, float fovy = 45.0f, int projection = CAMERA_PERSPECTIVE);
    
    float test1 = 0;
    float teste2 = 0;
    float timeAccumulator = 0.0f;

    void update(float dt) override;
    void draw() override;
};




class Player : public GameObject {
public:
    Vector3 velocity;
    float speed;

    float facingAngle;   // pra onde ele "olha"
    PlayerState state;

    CameraObject* cameraObj;

    Texture2D texture;

    // animação depois
    float animTimer;
    int currentFrame;

    Player(CameraObject* camera);

    ~Player();



    void update(float dt) override;
    void draw() override;
};

