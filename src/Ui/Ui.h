#pragma once
#include "src/config.h"
#include "raylib.h"
#include "src/GameObjects/gameObjects.h"
#include <cstring>   // strlen
#include <cstdio>    // snprintf
#include <cstdlib>   // atof
#include "src/i18n/i18n.h"


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
    float largura = 200, altura = 400;


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
    
    

public:
    edit_table();
    void update(GameObject* activeObject, GameMode mode);
    UIField focusedField = UIField::NONE;

    bool is_ativated = false;
    

    bool isMouseCaptured() const { return uiCapturedMouse; }


    void draw(GameObject* activeObject, GameMode mode, Font uiFont);

};



