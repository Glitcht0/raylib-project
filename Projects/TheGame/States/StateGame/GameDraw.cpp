#include "Projects/TheGame/States/StateGame/StateGame.h"

// =================================
//        ✏️ Desenha o Grid
// =================================
void StateGame::DrawGridXZ(int size, float step)
{
    int half = size / 2;

    // ---------- EIXOS (UMA VEZ SÓ) ----------
    DrawLine3D(
        (Vector3){ -half*step, 0, 0 },
        (Vector3){  half*step, 0, 0 },
        RED     // eixo X
    );

    DrawLine3D(
        (Vector3){ 0, 0, -half*step },
        (Vector3){ 0, 0,  half*step },
        BLUE    // eixo Z
    );

    // ---------- GRID ----------
    for (int i = -half; i <= half; i++)
    {
        if (i == 0) continue; // NÃO redesenha em cima dos eixos

        float pos = i * step;

        Color color = (i % 10 == 0) ? COR_GRID2 : COR_GRID;

        // paralelas ao X
        DrawLine3D(
            (Vector3){ -half*step, 0, pos },
            (Vector3){  half*step, 0, pos },
            color
        );

        // paralelas ao Z
        DrawLine3D(
            (Vector3){ pos, 0, -half*step },
            (Vector3){ pos, 0,  half*step },
            color
        );
    }
}

