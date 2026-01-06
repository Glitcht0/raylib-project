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

BoundingBox Cube::GetBoundingBox() {
    BoundingBox box;
    box.min = {
        position.x - size.x * 0.5f,
        position.y - size.y * 0.5f,
        position.z - size.z * 0.5f
    };
    box.max = {
        position.x + size.x * 0.5f,
        position.y + size.y * 0.5f,
        position.z + size.z * 0.5f
    };
    return box;
}



// ====================
//     📷 Camera (Vector3 pos, Vector3 target, Vector3 up, float fovy, int projection)
// ====================
CameraObject::CameraObject(Vector3 pos, Vector3 target, Vector3 up, float fovy, int projection) {
    
    this->position = pos;      // posição do GameObject
    this->target = target;     // alvo para orbitar
    this->distance = Vector3Distance(pos, target); // distância inicial
    this->yaw = 185.0f  * DEG2RAD; // Rotação da camera
    this->pitch = 35.0f * DEG2RAD;

    initialPosition.x = target.x + distance * cosf(pitch) * cosf(yaw);
    initialPosition.y = target.y + distance * sinf(pitch);
    initialPosition.z = target.z + distance * cosf(pitch) * sinf(yaw);

    position = initialPosition;
    
    cam.position = position;
    cam.target = target;
    cam.up = up;
    this->fovy = fovy;
    this->projection = projection;
    cam.fovy = fovy;
    cam.projection = projection;

    
    initialTarget = target;
}



void CameraObject::update(float dt) {
    // Converte yaw/pitch/distance para posição da câmera
    cam.position.x = target.x + distance * cosf(pitch) * cosf(yaw);
    cam.position.y = target.y + distance * sinf(pitch);
    cam.position.z = target.z + distance * cosf(pitch) * sinf(yaw);

    // Sincroniza posição do GameObject, caso queira manipulá-la visualmente
    position = cam.position;

    cam.target = target;
    cam.fovy = fovy;
    cam.projection = projection;


}

void CameraObject::draw() {
    
    Vector3 direction = Vector3Normalize(Vector3Subtract(initialTarget, initialPosition)); // 1. Calcula a direção (Vetor unitário apontando para o alvo)

    Vector3 endPoint = Vector3Add(initialPosition, Vector3Scale(direction, 0.5f)); // 2. Define um ponto um pouco à frente para ser a "ponta" da câmera

    DrawCylinderEx(initialPosition, endPoint, 0.05f, 0.3f, 4, DARKGRAY); // 3. Desenha um cone que conecta a posição inicial ao ponto à frente
    
    DrawSphere(initialPosition, 0.1f, DARKGRAY); // (Opcional) Uma esfera pequena na base para marcar a posição exata
    
    DrawLine3D(initialPosition, initialTarget, Fade(GREEN, 0.5f));  // (Opcional) Uma linha até o alvo real para debug
    
}