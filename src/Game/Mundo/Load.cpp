#include "mundo.h"

// ============================================================================
//        ⬆️ Carrega a posição do jogador do disco
// ============================================================================
Vector3 World::loadplayer(float halfSize){
    Vector3 posicao;
    std::string filename = "saves/" + nomeMundo + "/player"  + ".dat";
    
    FILE* f = fopen(filename.c_str(), "rb");
    if (!f) {
        posicao = Get_Spaw(halfSize);
        jogador.position = posicao;
        // Arquivo não existe

    }else{
        fread(&posicao, sizeof(Vector3), 1, f);
        printf("Posição do player carregada do disco: (%.2f, %.2f, %.2f)\n", posicao.x, posicao.y, posicao.z);
        jogador.position = posicao;
        
    }

    fclose(f);
    return posicao;

}


// ============================================================================
//        ⬇️ Salva a posição do jogador no disco
// ============================================================================
void World::savePlayerPosition(Vector3 position){
    jogador.position = position;
    std::string filename = "saves/" + nomeMundo + "/player"  + ".dat";
    
    FILE* f = fopen(filename.c_str(), "wb");
    if (!f) {
        printf("Erro ao salvar a posição do player no disco.\n");
        return; // Erro ao abrir o arquivo
    }

    fwrite(&jogador.position, sizeof(Vector3), 1, f);
    fclose(f);
    printf("Posição do player salva no disco: (%.2f, %.2f, %.2f)\n", jogador.position.x, jogador.position.y, jogador.position.z);

}