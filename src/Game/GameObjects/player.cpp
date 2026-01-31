#include "gameObjects.h"
#include <iostream>
#include <fstream>
#include "libs/json.hpp"
#include "src/Game/Estruturas/Estruturas.h"

using json = nlohmann::json;



Player::Player(CameraObject* camera, World* w) {

    world = w;

    cameraObj = camera;
    position = { 0, 0, 0 };
    velocity = { 0, 0, 0 };
    speed = 3.5f;
    state = IDLE_FRONT;
    animTimer = 0.0f;
    currentFrame = 0;


    // Carrega a imagem gigante
    atlasTexture = LoadTexture("assets/Glitcht/glitcht_animation.dds");
    SetTextureFilter(atlasTexture, TEXTURE_FILTER_BILINEAR);

    // Carrega o arquivo JSON
    std::ifstream f("assets/Glitcht/glitcht_animation.json");
    if (!f.is_open()) {
        std::cerr << "ERRO: Nao foi possivel abrir o JSON da animacao!" << std::endl;
    }
    
    json data;
    try {
        f >> data; // O parse acontece aqui
    } catch (json::parse_error& e) {
        std::cerr << "ERRO de Parse no JSON: " << e.what() << std::endl;
    }

    // ===== 2. CONFIGURAR AS ANIMAÇÕES =====
    // Configura FPS e Textura base
    animIdleFront.atlas = atlasTexture; animIdleFront.fps = 12.0f;
    animRunLeft.atlas   = atlasTexture; animRunLeft.fps = 12.0f;
    animRunRight.atlas  = atlasTexture; animRunRight.fps = 12.0f;
    animWalkFront.atlas = atlasTexture; animWalkFront.fps = 24.0f;
    animIdleLeft.atlas  = atlasTexture; animIdleLeft.fps = 12.0f;
    animIdleRight.atlas = atlasTexture; animIdleRight.fps = 12.0f;

    // Carrega os frames lendo do JSON


    LoadFramesFromJSON(&data, "Parado_frente/quadro",    23, 0, animIdleFront);
    LoadFramesFromJSON(&data, "andando_Esquerda/quadro", 11, 0, animRunLeft);
    LoadFramesFromJSON(&data, "andando_direita/quadro",  11, 0, animRunRight);
    LoadFramesFromJSON(&data, "andando_frente/quadro",   11, 0, animWalkFront);
    LoadFramesFromJSON(&data, "Parado_Esquerda/quadro",  11, 0, animIdleLeft);
    LoadFramesFromJSON(&data, "Parado_direita/quadro",   11, 0, animIdleRight);

    // Começa com Idle
    currentAnim = &animIdleFront;
}



// Função auxiliar para ler o JSON e preencher o vetor de Rectangles
void Player::LoadFramesFromJSON(void* jsonData, const std::string& prefix, int count, int startFrame, SpriteAnimation& anim) {
    json* data = (json*)jsonData;
    

    
    json& framesObj = (*data)["frames"]; 

    for (int i = 0; i < count; i++) {
       
        char buffer[128];  // Monta o nome da chave: ex "Parado_frente/quadro0000.png"

        sprintf(buffer, "%s%04d.png", prefix.c_str(), startFrame + i); 
        std::string key = std::string(buffer);

        if (framesObj.contains(key)) {
            auto& frameData = framesObj[key]["frame"];
            float x = frameData["x"];
            float y = frameData["y"];
            float w = frameData["w"];
            float h = frameData["h"];

             
            anim.frames.push_back( (Rectangle){x, y, w, h} ); // Adiciona na lista da animação
        } else {
            anim.frames.push_back({0,0,0,0}); 
        }
    }
}

Player::~Player() {
    UnloadTexture(atlasTexture);
    
    world->savePlayerPosition(position);
}

// Lógica segura para trocar animação
void Player::SetAnimation(SpriteAnimation* newAnim) {
    if (currentAnim != newAnim) {
        currentAnim = newAnim;
        currentFrame = 0; // Reseta para o frame 0 da nova animação
        animTimer = 0.0f;
    }
}





void Player::update(float dt) {
    // 1. Captura o Input
    Vector2 input = { 0, 0 };
    if (IsKeyDown(KEY_W)) input.y += 1;
    if (IsKeyDown(KEY_S)) input.y -= 1;
    if (IsKeyDown(KEY_A)) input.x -= 1;
    if (IsKeyDown(KEY_D)) input.x += 1;

    // 2. Delega as tarefas
    HandleMovement(input, dt);      // Cuida de andar e colidir
    UpdateAnimationState(input);    // Escolhe o sprite certo
    TickAnimation(dt);              // Roda a animação
}





void Player::draw() {
    if (!currentAnim || currentAnim->frames.empty()) return;
   

    if (currentFrame >= currentAnim->frames.size()) currentFrame = 0;

    Rectangle src = currentAnim->frames[currentFrame];
    

    float targetHeight = 1.3f; // Altura desejada em metros (ex: 1.80m)
    //DrawCylinderWires(position, playerRadius, playerRadius, targetHeight, 16, GREEN);
    
    // Calcula a proporção inversa (Largura / Altura)
    float aspectInv = 1.0f;
    if (src.height > 0) aspectInv = src.width / src.height;
    
    // Agora definimos a largura baseada na altura fixa
    Vector2 sizeVec = { targetHeight * aspectInv, targetHeight };

    float rotToCamera = atan2f(cameraObj->cam.position.x - position.x, cameraObj->cam.position.z - position.z);
    
    DrawSprite3DInclinado(currentAnim->atlas, src, position, sizeVec, rotToCamera, DEG2RAD * -10.0f, WHITE);
}

void Player::DrawSprite3DInclinado( Texture2D texture, Rectangle source, Vector3 position, Vector2 size, float rotY, float tiltX, Color tint) {
    rlPushMatrix();

    rlTranslatef(position.x, position.y, position.z);

    rlRotatef(rotY * RAD2DEG, 0, 1, 0);   // vira o personagem
    rlRotatef(tiltX * RAD2DEG, 1, 0, 0);  // inclinação fixa

    float w = size.x * 0.5f;
    float h = size.y;

    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);
    rlColor4ub(tint.r, tint.g, tint.b, tint.a);

    // base no chão
    rlTexCoord2f(source.x / texture.width, (source.y + source.height) / texture.height);
    rlVertex3f(-w, 0, 0);

    rlTexCoord2f((source.x + source.width) / texture.width, (source.y + source.height) / texture.height);
    rlVertex3f(+w, 0, 0);

    rlTexCoord2f((source.x + source.width) / texture.width, source.y / texture.height);
    rlVertex3f(+w, h, 0);

    rlTexCoord2f(source.x / texture.width, source.y / texture.height);
    rlVertex3f(-w, h, 0);

    rlEnd();
    rlSetTexture(0);

    rlPopMatrix();
}



BoundingBox Player::GetBoundingBox() const {
    return GetBoundingBoxAt(position);
}

BoundingBox Player::GetBoundingBoxAt(Vector3 pos) const {
    BoundingBox box;
    box.min = { pos.x - 0.3f, pos.y, pos.z - 0.3f };
    box.max = { pos.x + 0.3f, pos.y + 1.3f, pos.z + 0.3f };
    return box;
}


bool Player::CheckCollisionCircleAABB_XZ( Vector3 center, float radius,const BoundingBox& box){
    float closestX = Clamp(center.x, box.min.x, box.max.x);
    float closestZ = Clamp(center.z, box.min.z, box.max.z);

    float dx = center.x - closestX;
    float dz = center.z - closestZ;

    return (dx*dx + dz*dz) <= (radius * radius);
}




void Player::HandleMovement(Vector2 input, float dt) {
    if (Vector2Length(input) > 0) {
        // === Descobre qual será a proxima possição do personagem ===
        input = Vector2Normalize(input); //Input diagonal: (1,1), length ≈ 1.414 → normalizado = (0.707, 0.707) → agora a velocidade é consistente

        
        Vector3 camForward = Vector3Subtract(cameraObj->cam.target, cameraObj->cam.position);  
        camForward.y = 0.0f;
        camForward = Vector3Normalize(camForward); // Direção que a camera olha

        Vector3 camRight = Vector3CrossProduct(camForward, cameraObj->cam.up);
        camRight.y = 0.0f;
        camRight = Vector3Normalize(camRight); // Direção direita da câmera

        Vector3 move = { 0 };
        move.x = camForward.x * input.y + camRight.x * input.x;
        move.z = camForward.z * input.y + camRight.z * input.x;

        //Novas possiões, pro x e pro Z
        Vector3 nextPosX = position;
        nextPosX.x += move.x * speed * dt;
        bool blockedX = false;

        Vector3 nextPosZ = position;
        nextPosZ.z += move.z * speed * dt;
        bool blockedZ = false;


        // === COLISÃO ===
        // Checa colisão X e Z
        if (worldStructures) {
            // Verifica eixo X
            if (worldStructures->CheckCollision(nextPosX, playerRadius)) {
                blockedX = true;
            }

            // Verifica eixo Z (usando a posição original de X para permitir deslizar)
            Vector3 testZ = position; 
            testZ.z = nextPosZ.z;
            
            if (worldStructures->CheckCollision(testZ, playerRadius)) {
                blockedZ = true;
            }
        }


        //Verifica Tile Bloqueado
        if (!blockedX && !world->Get_walkTileWorld(nextPosX, 0.3f))
            blockedX = true;

        if (!blockedZ && !world->Get_walkTileWorld(nextPosZ, 0.3f))
            blockedZ = true;


        if (!blockedX) position.x = nextPosX.x;
        if (!blockedZ) position.z = nextPosZ.z;
    }
}


void Player::UpdateAnimationState(Vector2 input) {

    if (Vector2Length(input) > 0) {
        input = Vector2Normalize(input);
        lastDir = input;

        // 1️⃣ QUALQUER componente lateral → lado
        if (fabsf(input.x) > 0.2f) {
            if (input.x > 0) {
                SetAnimation(&animRunRight);
                state = RUN_RIGHT;
            } else {
                SetAnimation(&animRunLeft);
                state = RUN_LEFT;
            }
        }
        // 2️⃣ Só frente se for baixo puro
        else if (input.y < -0.2f) {
            SetAnimation(&animWalkFront);
            state = WALK_FRONT;
        }
        // 3️⃣ Só costas se for cima puro
        else {
            SetAnimation(&animIdleFront); // depois animWalkBack
            state = WALK_BACK;
        }
        return;
    }

    // === PARADO ===
    if (fabsf(lastDir.x) > 0.2f) {
        if (lastDir.x > 0) SetAnimation(&animIdleRight);
        else SetAnimation(&animIdleLeft);
    }
    else if (lastDir.y < -0.2f) {
        SetAnimation(&animIdleFront);
    }
    else {
        SetAnimation(&animIdleFront); // idle costas depois
    }

    state = IDLE_FRONT;
}


void Player::TickAnimation(float dt) {
    if (currentAnim && !currentAnim->frames.empty()) {
        animTimer += dt;
        if (animTimer >= 1.0f / currentAnim->fps) {
            animTimer = 0.0f;
            currentFrame++;
            
            // Usa .size() do vetor agora
            if (currentFrame >= currentAnim->frames.size()) {
                currentFrame = 0; 
            }
        }
    }
}