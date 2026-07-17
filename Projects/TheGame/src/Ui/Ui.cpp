#include "Ui.h"


edit_table::edit_table(World* w){
    world = w;
    position = Vector2{ LARGURA_TELA - largura -5, 5 };
    size = Vector2{ largura, altura };

    float x = position.x + 10;
    float y = position.y + 70;

    posX.setRect({ x, y, 180, 18 }); y += 22;
    posY.setRect({ x, y, 180, 18 }); y += 22;
    posZ.setRect({ x, y, 180, 18 }); y += 50;

    rotX.setRect({ x, y, 180, 18 }); y += 22;
    rotY.setRect({ x, y, 180, 18 }); y += 22;
    rotZ.setRect({ x, y, 180, 18 });
}

void edit_table::update(GameObject* activeObject, GameMode mode) {
    

    if (!is_ativated || !activeObject || mode != GameMode::EDIT)
        return;
    uiCapturedMouse = false;

    // 🔹 bind SEMPRE
    posX.bind(&activeObject->position.x);
    posY.bind(&activeObject->position.y);
    posZ.bind(&activeObject->position.z);
    rotX.bind(&activeObject->rotation.x);
    rotY.bind(&activeObject->rotation.y);
    rotZ.bind(&activeObject->rotation.z);

    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    Rectangle panelRect = { position.x, position.y, size.x, size.y };

    if (click && CheckCollisionPointRec(mouse, panelRect)) {
        uiCapturedMouse = true;
    }

    // 🔹 update SEMPRE
    posX.update(uiCapturedMouse);
    posY.update(uiCapturedMouse);
    posZ.update(uiCapturedMouse);
    rotX.update(uiCapturedMouse);
    rotY.update(uiCapturedMouse);
    rotZ.update(uiCapturedMouse);
}




void edit_table::draw(GameObject* activeObject, GameMode mode, Font uiFont){
    if (!is_ativated) return;

    


    

    DrawRectangleV(position, size, COR_MESA);

    // --- MINIMAPA ---
    float mapSize = largura - 20;
    Vector2 mapPos = { position.x + 10, position.y + altura - mapSize - 10};

    DrawTextEx(uiFont, "MiniMap",  (Vector2){ mapPos.x, mapPos.y - 18 }, 14, 1, WHITE);

    DrawMiniMap(world, mapPos, mapSize);

    

    float x = position.x + 10;
    float y = position.y + 10;

    DrawTextEx(uiFont, T("EDIT_BAR"), (Vector2){ x, y }, 18, 1, WHITE);
    

    if (!activeObject) return;

    y += 30;
    DrawTextEx(uiFont, "Position", (Vector2){ x, y }, 14, 1, WHITE);

    posX.draw(uiFont);

    posY.draw(uiFont);

    posZ.draw(uiFont);
    y += 100;

    
    DrawTextEx(uiFont, "Rotation", (Vector2){ x, y }, 14, 1, WHITE);



    rotX.draw(uiFont);
    rotY.draw(uiFont);
    rotZ.draw(uiFont);

    
  

    //DrawRectangleV(Vector2{x-2, y-1}, Vector2{ largura - 20, 15}, COR_INPUT);
    //DrawTextEx(uiFont, TextFormat("Z: %.2f", activeObject->rotation.z), (Vector2){ x, y }, 14, 1, WHITE);
}


void floatInput::draw(Font font) {
    Color bg = focused ? Color{80, 80, 120, 255} : COR_INPUT;
    DrawRectangleRec(rect, bg);

    char valueTxt[32] = "0.00";
    char finalTxt[64];

    if (value) {
        if (editing)
            strncpy(valueTxt, buffer, sizeof(valueTxt));
        else
            snprintf(valueTxt, sizeof(valueTxt), "%.2f", *value);
    }

    snprintf(finalTxt, sizeof(finalTxt), "%s %s", label, valueTxt);

    DrawTextEx( font, finalTxt, { rect.x + 4, rect.y + 2 }, 14, 1, WHITE );



}


void floatInput::update(bool& uiCapturedMouse) {
    if (!value) return;

    Vector2 m = GetMousePosition();

    // clique → foco
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        focused = CheckCollisionPointRec(m, rect);
        editing = focused;
        if (focused) uiCapturedMouse = true;
    }

    if (!editing) return;

    // captura caracteres
    int key = GetCharPressed();
    while (key > 0) {
        char c = (char)key;

        if ((c >= '0' && c <= '9') || c == '.' || c == '-') {
            int len = strlen(buffer);
            if (len < (int)sizeof(buffer) - 1) {
                buffer[len] = c;
                buffer[len + 1] = '\0';
            }
        }
        key = GetCharPressed();
    }

    // backspace
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(buffer);
        if (len > 0)
            buffer[len - 1] = '\0';
    }

    // confirma
    if (IsKeyPressed(KEY_ENTER)) {
        *value = (float)atof(buffer);
        editing = false;
        focused = false;
    }

    // cancela
    if (IsKeyPressed(KEY_ESCAPE)) {
        snprintf(buffer, sizeof(buffer), "%.2f", *value);
        editing = false;
        focused = false;
    }
}


void floatInput::bind(float* v) {
    if (value != v) {
        value = v;
        if (value) snprintf(buffer, sizeof(buffer), "%.2f", *value);
    }
}




void DrawMiniMap(World* world, Vector2 pos, float size) {
    if (!world) return;

    // Se seu mundo for infinito, você talvez queira mudar isso para renderizar 
    // ao redor do player. Por enquanto, mantive fixo no tamanho do mundo.
    int h = WORLD_H;
    int w = WORLD_W;

    float tileSize = size / (float)w; 

    for (int z = 0; z < h; z++) {
        for (int x = 0; x < w; x++) {

            // --- CORREÇÃO AQUI ---
            // Em vez de acessar a array, pedimos o Tile ao gerenciador de chunks
            Tile* tile = world->GetTile(x, z);

            // Se tile for nullptr, significa que o chunk não foi gerado/carregado ainda.
            // Pulamos para não crachar o jogo.
            Color c = DARKGRAY;
            if (tile == nullptr) {
                c = BLUE;
            }else{
                switch (tile->type) {
                    case TILE_GRASS: c = GREEN; break;
                    case TILE_DIRT:  c = BROWN; break;
                    case TILE_WATER: c = BLUE;  break;
                    case TILE_SAND:  c = YELLOW; break;
                    default: c = MAGENTA; break; // Debug para tipos desconhecidos
                }
            }

            

            

            // Sua lógica de rotação original (mantive igual)
            int dx = z;
            int dz = w - 1 - x;

            // Dica: DrawRectangleRec é ligeiramente mais rápido que DrawRectangle
            DrawRectangle(pos.x + dx * tileSize, pos.y + dz * tileSize, tileSize + 1, tileSize + 1, c);
        }
    }

    // Borda do minimapa
    DrawRectangleLines(pos.x, pos.y, size, size, WHITE);
}