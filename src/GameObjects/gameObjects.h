#pragma once
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <vector> 
#include "src/Mundo/mundo.h"

enum PlayerState {
    IDLE_FRONT,
    WALK_RIGHT,
    WALK_LEFT,
    RUN,
    ATTACK,
    INTERACT
};

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

    BoundingBox GetBoundingBox();
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
    


    void update(float dt) override;
    void draw() override;
};



// No header (Player.h ou gameObjects.h)
struct SpriteAnimation {
    Texture2D* frames; // Ponteiro para o array de texturas
    int frameCount;    // Quantos frames essa animação tem
    float fps;         // Velocidade específica dessa animação
};


class Player : public GameObject {
public:
    Vector3 velocity;
    float speed;

    float facingAngle;   // pra onde ele "olha"
    PlayerState state;

    CameraObject* cameraObj;
    std::vector<GameObject*>* worldObjects = nullptr;


    BoundingBox GetBoundingBox(Vector3 pos);
    Player(CameraObject* camera, World* w);

    ~Player();



    void update(float dt) override;
    void draw() override;
    void SetAnimation(SpriteAnimation* newAnim);

    void DrawSprite3DInclinado( Texture2D texture, Rectangle source, Vector3 position, Vector2 size, float rotY, float tiltX, Color tint);

private:
    Texture2D* texIdleFront;
    Texture2D* texRunLeft;
    Texture2D* texRunRight;

    // Nossas definições de animação
    SpriteAnimation animIdleFront;
    SpriteAnimation animRunLeft;
    SpriteAnimation animRunRight;

    // Ponteiro para a animação atual
    SpriteAnimation* currentAnim = nullptr;

    World* world;

    float animTimer = 0.0f;
    int currentFrame = 0;

    void HandleMovement(Vector2 input, float dt);
    void UpdateAnimationState(Vector2 input);
    void TickAnimation(float dt);

};

