#include "gameObjects.h"
#include <iostream>


Player::Player(CameraObject* camera, World* w) {

    world = w;

    cameraObj = camera;
    position = { 0, 0, 0 };
    velocity = { 0, 0, 0 };
    speed = 3.5f;
    state = IDLE_FRONT;
    animTimer = 0.0f;
    currentFrame = 0;



    // ===== CARREGA ANIMAÇÃO IDLE =====
    texIdleFront = new Texture2D[23];
    texRunLeft = new Texture2D[11];
    texRunRight = new Texture2D[11];

    for (int i = 0; i < 23; i++) {
        char path[128];
        sprintf(path, "assets/Glitcht/Parado_frente/quadro%04d.png", i);
        texIdleFront[i] = LoadTexture(path);
        SetTextureFilter(texIdleFront[i], TEXTURE_FILTER_BILINEAR);
    }
    for (int i = 0; i < 11; i++) {
        char path[128];
        sprintf(path, "assets/Glitcht/andando_Esquerda/quadro%04d.png", i);
        texRunLeft[i] = LoadTexture(path);
        SetTextureFilter(texRunLeft[i],  TEXTURE_FILTER_BILINEAR);
    }
    for (int i = 0; i < 11; i++) {
        char path[128];
        sprintf(path, "assets/Glitcht/andando_direita/quadro%04d.png", i);
        texRunRight[i] = LoadTexture(path);
        SetTextureFilter(texRunRight[i], TEXTURE_FILTER_BILINEAR);
    }

    // 3. Configurar as Structs de Animação
    animIdleFront.frames = texIdleFront;
    animIdleFront.frameCount = 23;
    animIdleFront.fps = 12.0f;

    animRunLeft.frames = texRunLeft;
    animRunLeft.frameCount = 11;
    animRunLeft.fps = 12.0f; // Correr pode ser mais rápido!

    animRunRight.frames = texRunRight;
    animRunRight.frameCount = 11;
    animRunRight.fps = 12.0f;

    // Começa com Idle
    currentAnim = &animIdleFront;
}


// Lógica segura para trocar animação
void Player::SetAnimation(SpriteAnimation* newAnim) {
    if (currentAnim != newAnim) {
        currentAnim = newAnim;
        currentFrame = 0; // Reseta para o frame 0 da nova animação
        animTimer = 0.0f;
    }
}


Player::~Player() {
    // Descarregar texturas
    for (int i = 0; i < 23; i++) UnloadTexture(texIdleFront[i]);
    for (int i = 0; i < 11; i++) UnloadTexture(texRunLeft[i]);
    for (int i = 0; i < 11; i++) UnloadTexture(texRunRight[i]); // <--- Add

    // Liberar os arrays
    delete[] texIdleFront;
    delete[] texRunLeft;
    delete[] texRunRight; // <--- Add
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
    if (!currentAnim) return;

    DrawBoundingBox(GetBoundingBox(position), GREEN);


    Texture2D tex = currentAnim->frames[currentFrame];
    float size = 1.3f;

    float aspect = (float)tex.height / (float)tex.width;
    Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };


    float rotToCamera = atan2f(cameraObj->cam.position.x - position.x, cameraObj->cam.position.z - position.z);
    DrawSprite3DInclinado( tex, src, position, (Vector2){ size, size * aspect }, rotToCamera, DEG2RAD * -10.0f, WHITE );

    DrawSphere(position, 0.05f, GREEN);
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



BoundingBox Player::GetBoundingBox(Vector3 pos) {
    BoundingBox box;
    box.min = { pos.x - 0.3f, pos.y, pos.z - 0.3f };
    box.max = { pos.x + 0.3f, pos.y + 1.3f, pos.z + 0.3f };
    return box;
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

        BoundingBox playerBoxX = GetBoundingBox(nextPosX);
        bool blockedX = false;

        Vector3 nextPosZ = position;
        nextPosZ.z += move.z * speed * dt;

        BoundingBox playerBoxZ = GetBoundingBox(nextPosZ);
        bool blockedZ = false;


        // === COLISÃO ===
        // Checa colisão X
        if (worldObjects) {
            for (GameObject* obj : *worldObjects) {
                Cube* cube = dynamic_cast<Cube*>(obj);
                if (!cube) continue;

                if (CheckCollisionBoxes(playerBoxX, cube->GetBoundingBox())) {
                    blockedX = true;
                    break;
                }
                
            }
        }

        // Checa colisão Z
        if (worldObjects) {
            for (GameObject* obj : *worldObjects) {
                Cube* cube = dynamic_cast<Cube*>(obj);
                if (!cube) continue;

                if (CheckCollisionBoxes(playerBoxZ, cube->GetBoundingBox())) {
                    blockedZ = true;
                    break;
                }
                
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
    if (Vector2Length(input) == 0) {
        SetAnimation(&animIdleFront);
    } 
    else {
        float angle = atan2f(input.x, input.y); 
        float deg = angle * RAD2DEG;

        // Região "CIMA" (Costas)
        if (deg > -45 && deg < 45) {
             SetAnimation(&animIdleFront); // TODO: animRunBack
        }
        // Região "DIREITA"
        else if (deg >= 45 && deg < 135) {
             SetAnimation(&animRunRight);
        }
        // Região "ESQUERDA"
        else if (deg <= -45 && deg > -135) {
             SetAnimation(&animRunLeft);
        }
        // Região "BAIXO" (Frente)
        else {
             SetAnimation(&animIdleFront); // TODO: animRunFront
        }
    }
}

void Player::TickAnimation(float dt) {
    if (currentAnim) {
        animTimer += dt;
        if (animTimer >= 1.0f / currentAnim->fps) {
            animTimer = 0.0f;
            currentFrame++;
            
            if (currentFrame >= currentAnim->frameCount) {
                currentFrame = 0; 
            }
        }
    }
}