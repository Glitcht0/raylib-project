#pragma once
#include "src/core/state.h"
#include "src/core/state_machine.h"
#include "raylib.h"
#include "raymath.h"
#include "src/core/config.h"
#include "src/Game/GameObjects/gameObjects.h"
#include <vector>
#include "libs/rlights.h"
#include "src/engine/Ui/Ui.h"
#include "src/engine/Utils/Colisores.h"
#include "src/Game/Mundo/mundo.h"
#include "src/engine/Utils/sistema.h"  

#include <string>

#define FUNDO (Color){ 63, 63, 63, 255 }
#define COR_GRID (Color){ 80, 80, 80, 255 }
#define COR_GRID2 (Color){ 95, 95, 95, 255 }





class StateGame : public State {
public:

    StateGame(std::string nomeM = "default");
    void onEnter() override;
    void onExit() override;

    void update(appstate* currentState) override;
    void draw() override;
    Font uiFont;

   


private:
    GameMode transformMode = GameMode::GAME;

    std::string nomeMundo;

    CameraObject* cameraObj;
    Player* playerObj;
    World world;
    edit_table* mesaEd;
    Shader shader;
    Light lights[MAX_LIGHTS] = { 0 };

    
    

    std::vector<GameObject*> objects; // todos os objetos na cena

    int ambientLoc;

    float yaw   = 0.0f; //rotação horizontal
    float pitch = 0.0f; //rotação vertical
    float distance = 6.0f;

    
    void DrawGridXZ(int size, float step);


    void handleMiddleMouse(Vector2 delta);
    void handleScrollMouse();
    void handleInput();
    void handleTransform();

    void updateAll();
    void updateOBBSelection();
    void updateCamera();
    void updateLights();
    void updateObjects();


    //Dados pra movimentação de objetos
    Vector3 moveStartMouseWorld; //posição no mundo do objeto
    Vector3 moveStartObjectPos; //posição inicial do objeto
    Vector3 moveStartObjectRotation; // rotação inicial do objeto

    Vector2 prevMousePos = { 0,0 };
    GameObject* activeObject = nullptr;
    GameObject* selectedObject = nullptr;
    bool lockAxisY = false;

    Vector3 startRotation; // rotação inicial do objeto
    bool axisLockActive = false; // se alguma tecla de travamento está sendo usada
    
    

    Vector3 GetMouseWorldPosCameraPlane();


};
