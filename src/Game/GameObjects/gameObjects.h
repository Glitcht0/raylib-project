#pragma once
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <vector> 
#include <string>
#include "src/Game/Mundo/mundo.h"

enum PlayerState {
    IDLE_FRONT,
    IDLE_LEFT,
    IDLE_RIGHT,
    RUN_LEFT,
    RUN_RIGHT,
    WALK_FRONT,
    WALK_BACK,
    RUN,
    ATTACK,
    INTERACT
};

class structures;

class GameObject{
public:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Color color;

    bool is_selected = false;
    

    virtual ~GameObject(){}
    virtual void update(float dt) {}
    virtual void draw() = 0;
    virtual bool HasCollision() const { return false; }
    virtual BoundingBox GetBoundingBox() const = 0;
};

class Cube : public GameObject {
public:


    Cube(Vector3 pos, Vector3 sz, Color col);

    void update(float dt) override;

    void draw() override;

    bool HasCollision() const override { return true; }


    BoundingBox GetBoundingBox() const override;
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
    
    BoundingBox GetBoundingBox() const override { return { 0 };}

    void update(float dt) override;
    void draw() override;
};



struct SpriteAnimation {
    Texture2D atlas;              // A textura GIGANTE (compartilhada)
    std::vector<Rectangle> frames; // As coordenadas de cada quadro na textura gigante
    float fps;
};

class Player : public GameObject {
public:
    // ... (Variáveis de física continuam iguais) ...
    Vector3 velocity;
    float speed;
    float facingAngle;
    PlayerState state;
    float playerRadius = 0.3f;
    Vector2 lastDir = { 0, -1 };

    CameraObject* cameraObj;
    //std::vector<GameObject*>* worldObjects = nullptr;
    structures* worldStructures = nullptr;

    // Construtor e destrutor
    Player(CameraObject* camera, World* w);
    ~Player();

    // Métodos
    void update(float dt) override;
    void draw() override;
    void SetAnimation(SpriteAnimation* newAnim);
    void DrawSprite3DInclinado(Texture2D texture, Rectangle source, Vector3 position, Vector2 size, float rotY, float tiltX, Color tint);

    // Colisão
    BoundingBox GetBoundingBox() const override;
    BoundingBox GetBoundingBoxAt(Vector3 pos) const;
    bool CheckCollisionCircleAABB_XZ(Vector3 center, float radius, const BoundingBox& box);

private:
    Texture2D atlasTexture; // A imagem "glitcht_animation.png"


    SpriteAnimation animIdleFront;
    SpriteAnimation animRunLeft;
    SpriteAnimation animRunRight;
    SpriteAnimation animWalkFront;
    SpriteAnimation animIdleLeft;
    SpriteAnimation animIdleRight;

    SpriteAnimation* currentAnim = nullptr;
    World* world;

    float animTimer = 0.0f;
    size_t currentFrame = 0;

    void HandleMovement(Vector2 input, float dt);
    void UpdateAnimationState(Vector2 input);
    void TickAnimation(float dt);

    // Helper para carregar os frames do JSON
    // json_data é um tipo genérico (void*) aqui para não sujar o header com bibliotecas externas,
    // mas no .cpp faremos o cast correto.
    void LoadFramesFromJSON(void* jsonData, const std::string& prefix, int count, int startFrame, SpriteAnimation& anim);
};

