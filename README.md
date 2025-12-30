# Raylib-CPP-Starter-Template-for-VSCODE-V2
Raylib C++ Starter Template for Visual Studio Code on Windows.
This demo project contains a bouncing ball raylib example program.
It works with raylib version 5.0. Tested on both Windows 10 and Windows 11.

# How to use this template
1. Double click on the main.code-workspace file. This will open the template in VS Code.
2. From the Explorer Window of VS Code navigate to the src folder and double click on the main.cpp file.
3. Press F5 on the keyboard to compile and run the program.

# What's changed
The template now uses folders for better organizion of the files. So, all the source code now lives in the src folder.

# Video Tutorial

<p align="center">
  <img src="preview.jpg" alt="" width="800">
</p>

<p align="center">
🎥 <a href="https://www.youtube.com/watch?v=PaAcVk5jUd8">Video Tutorial on YouTube</a>
</p>

<br>
<br>
<p align="center">
| 📺 <a href="https://www.youtube.com/channel/UC3ivOTE5EgpmF2DHLBmWIWg">My YouTube Channel</a>
| 🌍 <a href="http://www.programmingwithnick.com">My Website</a> | <br>
</p>







git add .
git commit -m "Virou Blender"





git remote add origin https://github.com/Glitcht0/raylib-project


git branch -M main         
git push -u origin main     

















#include <raylib.h>
#include "ball.h"

int main() 
{
    const Color darkGreen = {20, 160, 133, 255};
    
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;
    
    Ball ball;
    
    InitWindow(screenWidth, screenHeight, "My first RAYLIB program!");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        ball.Update();
        
        BeginDrawing();
            ClearBackground(darkGreen);
            ball.Draw();
        EndDrawing();
    }
    
    CloseWindow();
}


#include <raylib.h>



int main() {
    int ballx = 400;
    int bally = 400;
    Color cor = {30, 42, 50, 255};
    
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(800, 800, "My fist raylib game");
    SetTargetFPS(60);

    //Game Loop
    while(WindowShouldClose() == false) {

        // 1. Event handling

        // 2. updating Positions

        if(IsKeyDown(KEY_RIGHT)){
            ballx += 3;
        } else if(IsKeyDown(KEY_LEFT)){
            ballx -=3;
        } else if(IsKeyDown(KEY_UP)){
            bally -= 3;
        }else if(IsKeyDown(KEY_DOWN)){
            bally += 3;
        }


        // 3. Drawing
        BeginDrawing();
        ClearBackground(cor);
        
        DrawCircle(ballx, bally, 20, WHITE);
        EndDrawing();

        
    }

    CloseWindow();
    return 0;

}










#include "raylib.h"

int main(void)
{
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
            DrawPlane(
                (Vector3){ 0.0f, 0.0f, 0.0f },
                (Vector2){ 10.0f, 10.0f },
                LIGHTGRAY
            );

            // Cubo
            DrawCube(
                (Vector3){ 0.0f, 1.0f, 0.0f },
                1.0f, 1.0f, 1.0f,
                RED
            );

            // Contorno do cubo
            DrawCubeWires(
                (Vector3){ 0.0f, 1.0f, 0.0f },
                1.0f, 1.0f, 1.0f,
                BLACK
            );

        EndMode3D();

        DrawText("Cubo 3D simples", 10, 10, 20, DARKGRAY);
        DrawFPS(10, 40);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}




📐 Primitivas 2D (formas básicas)
Retângulos
DrawRectangle(int x, int y, int width, int height, Color color);
DrawRectangleV(Vector2 pos, Vector2 size, Color color);
DrawRectangleRec(Rectangle rec, Color color);
DrawRectangleLines(int x, int y, int width, int height, Color color);
DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color);
DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color);
DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, float lineThick, Color color);

Círculos
DrawCircle(int centerX, int centerY, float radius, Color color);
DrawCircleV(Vector2 center, float radius, Color color);
DrawCircleLines(int centerX, int centerY, float radius, Color color);
DrawCircleSector(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color);
DrawCircleGradient(int centerX, int centerY, float radius, Color color1, Color color2);

Linhas
DrawLine(int startX, int startY, int endX, int endY, Color color);
DrawLineV(Vector2 start, Vector2 end, Color color);
DrawLineEx(Vector2 start, Vector2 end, float thick, Color color);
DrawLineBezier(Vector2 start, Vector2 end, float thick, Color color);

Triângulos / Polígonos
DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);
DrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color);
DrawPolyLinesEx(Vector2 center, int sides, float radius, float rotation, float lineThick, Color color);

🖼️ Texturas e imagens (sprites)
DrawTexture(Texture2D texture, int x, int y, Color tint);
DrawTextureV(Texture2D texture, Vector2 position, Color tint);
DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
DrawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);
DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);


👉 DrawTexturePro é o mais poderoso (rota, escala, origem customizada).

🔤 Texto / Fontes
DrawText(const char *text, int x, int y, int fontSize, Color color);
DrawTextEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint);
DrawTextPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint);
DrawFPS(int posX, int posY);

📦 Helpers de 2D
Grade
DrawGrid(int slices, float spacing);

Fundo
ClearBackground(Color color);

🧠 Importante (pipeline 2D)

Tudo isso funciona entre:

BeginDrawing();
ClearBackground(RAYWHITE);

// DESENHO 2D AQUI

EndDrawing();


Se você estiver em 3D:

BeginDrawing();
ClearBackground(RAYWHITE);

BeginMode3D(camera);
// desenho 3D
EndMode3D();

// desenho 2D por cima (HUD, UI)
DrawText("UI", 10, 10, 20, WHITE);

EndDrawing();