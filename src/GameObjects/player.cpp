#include "gameObjects.h"



Player::Player(CameraObject* camera) {

    cameraObj = camera;
    position = { 0, 0, 0 };
    velocity = { 0, 0, 0 };
    speed = 3.0f;
    state = IDLE;
    animTimer = 0.0f;
    currentFrame = 0;
    //assets/player.png C:\Users\Mrco\Desktop\Raylib\assets\sprites\quadro0000.png
    texture = LoadTexture("assets/sprites/quadro0000.png");
}

Player::~Player() {
    UnloadTexture(texture);
}


void Player::update(float dt) {
    Vector2 input = {0, 0};

    if (IsKeyDown(KEY_W)) input.y += 1;
    if (IsKeyDown(KEY_S)) input.y -= 1;
    if (IsKeyDown(KEY_A)) input.x += 1;
    if (IsKeyDown(KEY_D)) input.x -= 1;

    if (Vector2Length(input) > 0) {
        input = Vector2Normalize(input);

        position.x += input.x * speed * dt;
        position.z += input.y * speed * dt;

        facingAngle = atan2f(input.x, input.y);
        state = WALK;
    } else {
        state = IDLE;
    }
}




void Player::draw() {
    //DrawSphere({ position.x, position.y + 0.9f, position.z }, 0.3f, WHITE);
    //DrawCylinder( { position.x, position.y + 0.45f, position.z }, 0.15f, 0.12f, 0.8f, 8, BLACK);


    float size = 1.3f;

    Vector3 drawPos = position;
    drawPos.y += size * 0.5f;

    DrawBillboard(cameraObj->cam, texture, drawPos, size, WHITE);

    //DrawCylinder(position, 0.25f, 0.25f, 0.6f, 12, Fade(RED, 0.5f)); // Colisor

    DrawSphere(position, 0.05f, GREEN); // Posição real

}
