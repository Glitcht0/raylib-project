
#include "raylib.h"
#include "src/core/state_machine.h"

int main(void)
{
    principal_loop();

    /*
    // Janela
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(800, 600, "Raylib 3D - Exemplo simples");
    SetTargetFPS(60);

    // Camera 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 4.0f, 4.0f, 4.0f };
    camera.target   = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy     = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Loop principal
    while (!WindowShouldClose())
    {
        // Atualiza câmera (mouse gira)
        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

            // Chão
            DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 10.0f, 10.0f }, LIGHTGRAY );

            // Cubo
            DrawCube((Vector3){ 0.0f, 1.0f, 0.0f }, 1.0f, 1.0f, 1.0f, RED );

            // Contorno do cubo
            DrawCubeWires((Vector3){ 0.0f, 1.0f, 0.0f },1.0f, 1.0f, 1.0f, BLACK);

        EndMode3D();

        DrawText("Cubo 3D simples", 10, 10, 20, DARKGRAY);
        DrawFPS(10, 40);

        EndDrawing();
    }

    CloseWindow();*/
    
    return 0;
}
