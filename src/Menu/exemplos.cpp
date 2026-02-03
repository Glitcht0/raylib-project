#include "state_menu.h"

void DrawButtonsDemo(void){
    static bool checked = false;

    GuiGroupBox((Rectangle){ 20, 20, 180, 160 }, "Buttons");

    GuiCheckBox((Rectangle){ 30, 50, 15, 15 }, "CheckBox", &checked);

    if (GuiButton((Rectangle){ 30, 80, 140, 30 }, "Normal Button")) {}

    GuiSetState(STATE_DISABLED);
    GuiButton((Rectangle){ 30, 120, 140, 30 }, "Disabled");
    GuiSetState(STATE_NORMAL);
}


void DrawInputsDemo(void){
    static int spinnerValue = 5;
    static bool spinnerEdit = false;

    static int valueBoxValue = 10;
    static bool valueBoxEdit = false;

    static char text[64] = "Hello";
    static bool textEdit = false;

    GuiGroupBox((Rectangle){ 220, 20, 200, 200 }, "Inputs");

    if (GuiSpinner((Rectangle){ 230, 50, 140, 30 }, NULL, &spinnerValue, 0, 100, spinnerEdit))
        spinnerEdit = !spinnerEdit;

    if (GuiValueBox((Rectangle){ 230, 90, 140, 30 }, NULL, &valueBoxValue, 0, 100, valueBoxEdit))
        valueBoxEdit = !valueBoxEdit;

    if (GuiTextBox((Rectangle){ 230, 130, 140, 30 }, text, 64, textEdit))
        textEdit = !textEdit;
}


void DrawListsDemo(void){
    static int listScroll = 0;
    static int listActive = -1;

    static int comboActive = 0;

    GuiGroupBox((Rectangle){ 450, 20, 200, 220 }, "Lists");

    GuiListView((Rectangle){ 460, 50, 160, 100 },
        "Apple;Banana;Orange;Pineapple;Grape;Mango",
        &listScroll, &listActive);

    GuiComboBox((Rectangle){ 460, 160, 160, 30 },
        "Red;Green;Blue;Yellow", &comboActive);
}



void DrawSlidersDemo(void){
    static float slider = 50;
    static float progress = 0.4f;
    static int toggle = 0;

    GuiGroupBox((Rectangle){ 20, 220, 300, 200 }, "Sliders");

    GuiSlider((Rectangle){ 30, 260, 200, 20 }, "Min", "Max", &slider, 0, 100);
    GuiProgressBar((Rectangle){ 30, 300, 200, 20 }, NULL, NULL, &progress, 0, 1);

    GuiToggleSlider((Rectangle){ 30, 340, 200, 30 }, "OFF;ON", &toggle);
}

void DrawWindowsDemo(void){
    static bool showMsg = false;

    GuiGroupBox((Rectangle){ 340, 260, 300, 160 }, "Windows");

    if (GuiButton((Rectangle){ 350, 290, 140, 30 }, "Open MessageBox"))
        showMsg = true;

    if (showMsg)
    {
        int res = GuiMessageBox(
            (Rectangle){ 380, 330, 240, 120 },
            "Confirm",
            "Close this window?",
            "Yes;No"
        );

        if (res >= 0) showMsg = false;
    }
}