#pragma once
#include "src/core/config.h"
#include "raylib.h"
#include "src/Game/GameObjects/gameObjects.h"
#include <cstring>   // strlen
#include <cstdio>    // snprintf
#include <cstdlib>   // atof
#include "src/engine/i18n/i18n.h"
#include "src/Game/Mundo/mundo.h"

void DrawMiniMap(World* world, Vector2 pos, float size);


enum class UIField {
    NONE,
    POS_X, POS_Y, POS_Z,
    ROT_X, ROT_Y, ROT_Z
};

class floatInput {
private:
    Rectangle rect;
    const char* label;
    float* value;
    bool focused = false;
    char buffer[32] = {0};
    bool editing = false;

public:
    floatInput(const char* label) : label(label), value(nullptr) {}

    void bind(float* v);

    void setRect(Rectangle r) { rect = r; }

    void update(bool& uiCapturedMouse);

    void draw(Font font);
};



class edit_table{
private:
    Vector2 position;
    Vector2 size;
    float largura = 200, altura = 500;


    floatInput posX{"X:"};
    floatInput posY{"Y:"};
    floatInput posZ{"Z:"};

    floatInput rotX{"Rot X:"};
    floatInput rotY{"Rot Y:"};
    floatInput rotZ{"Rot Z:"};

    
    Rectangle rectPosX;
    Rectangle rectPosY;
    Rectangle rectPosZ;
    
    


    bool uiCapturedMouse = false;

    World* world;
    
    

public:
    edit_table(World* w);
    void update(GameObject* activeObject, GameMode mode);
    UIField focusedField = UIField::NONE;

    bool is_ativated = false;
    

    bool isMouseCaptured() const { return uiCapturedMouse; }


    void draw(GameObject* activeObject, GameMode mode, Font uiFont);

};



