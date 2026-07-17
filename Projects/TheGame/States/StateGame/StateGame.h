#pragma once


#include <raylib.h>
#include "libs/raygui.h"
#include "Engine/i18n/i18n.h"
#include "Engine/Core/State.h"
#include <string>



#include "raymath.h"
#include "Projects/TheGame/config.h"


#include "Projects/TheGame/States/StateGame/GameObjects/gameObjects.h"
#include <vector>
#include "libs/rlights.h"
#include "Projects/TheGame/src/Ui/Ui.h"

#include "Engine/Utils/Colisores.h"

#include "Projects/TheGame/States/StateGame/Mundo/mundo.h"

#include "Engine/Utils/sistema.h"
#include "Projects/TheGame/States/StateGame/Estruturas/Estruturas.h"
#include "Projects/TheGame/src/UI/PauseMenu.h"

#define FUNDO (Color){ 63, 63, 63, 255 }
#define COR_GRID (Color){ 80, 80, 80, 255 }
#define COR_GRID2 (Color){ 95, 95, 95, 255 }



class StateGame : public State {
public:

    StateGame(std::string nomeM = "default");
    ~StateGame();
    void onEnter() override;
    void onExit() override;

    void update() override;
    void draw() override;
    Font uiFont;
    

   


private:
    GameMode transformMode = GameMode::GAME;

    std::string nomeMundo;

    CameraObject* cameraObj;
    Player* playerObj;
    World world;
    structures* estruturas;
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
    bool AtivatMenu = false;

    Vector3 startRotation; // rotação inicial do objeto
    bool axisLockActive = false; // se alguma tecla de travamento está sendo usada
    
    

    Vector3 GetMouseWorldPosCameraPlane();


};
