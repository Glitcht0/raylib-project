#include "mundo.h"
#include <filesystem>


World::World(std::string nomeM): perlin(time(nullptr)) {
    

    nomeMundo = nomeM;
    
    ensureSaveDirectories();
    chunkData.clear();
    gerarmundo();



    

}

World::~World() {
    // 1. Salvar tudo antes de sair (opcional, mas recomendado)
    for (auto& pair : chunkData) {
        saveChunkToDisk(pair.second);
    }


    
    // Limpeza das meshes
    for (Chunk& c : chunks) {
        if (c.built) UnloadModel(c.model);
    }

    
}


void World::update(Vector3 playerPos) {

    processUnloadQueue(1); 
    processBuildQueue(1); 

    processLoadedChunks();

    unloadFarChunks(playerPos);
    updateChunks(playerPos);


}


void World::draw() {
    for (int index : visibleChunks) {
        if (chunks[index].built) { 
            DrawModel(chunks[index].model, {0,0,0}, 1.0f, WHITE);
        }
    }
}


// ===== Atualiza quais chunks devem estar carregados com base na posição do player ====
void World::updateChunks(Vector3 playerPos) {
    int cx, cz;
    getPlayerChunk(playerPos, cx, cz);
 
    tileVisibleChunks.clear(); // 🧊 Tiles
    visibleChunks.clear(); // 🏞️ Mesh

    for (int dz = -VIEW_DISTANCE; dz <= VIEW_DISTANCE; dz++) {
        for (int dx = -VIEW_DISTANCE; dx <= VIEW_DISTANCE; dx++) {
            int chunkX = cx + dx;
            int chunkZ = cz + dz;
            long long key = ChunkKey(chunkX, chunkZ);

            // Verifica se já temos os DADOS
            if (chunkData.find(key) == chunkData.end()) {
                requestChunkLoad(chunkX, chunkZ); // NÃO TEMOS! Pede para a thread carregar/gerar
            } 
            else {
                
                tileVisibleChunks.push_back(key); // JÁ TEMOS! Pode renderizar e criar Mesh

                TileChunk& tc = chunkData[key];
                if (tc.built){}
                

                
                // Só cria a Mesh visual se os dados lógicos já existirem
                int meshIndex = getOrCreateMeshChunk(chunkX, chunkZ);
                ensureChunkMeshBuilt(meshIndex);
                visibleChunks.push_back(meshIndex);
            }
        }
    }
}


// ===== Gera o mundo, ou carrega do disco se existir save =====
void World::gerarmundo() {
    std::string savePath = "saves/" + nomeMundo + "/chunks/";
    bool saveEncontrado = false;


    if (std::filesystem::exists(savePath)) {
        if (!std::filesystem::is_empty(savePath)) { // 2. Verifica se a pasta NÃO está vazia (tem algum arquivo .dat)
             saveEncontrado = true;
        }
    }

    if (saveEncontrado) {
        printf("--- SAVE ENCONTRADO: CARREGANDO DO DISCO ---\n");
        CarregarAreaInicial(WORLD_W, WORLD_H); // Carrega a área inicial (mesmo tamanho que você usaria para criar a ilha)
    } 
    else {
        printf("--- NENHUM SAVE: GERANDO NOVO MUNDO ---\n");
        CreatIsland(0, 0, WORLD_W, WORLD_H); // Gera do zero (código antigo)
    }

    mundo_gerado = true;
}



void World::CreatIsland(int xpos, int zpos, int largura, int altura){

    // ===== Gerar Matrix do mundo incial, ao redor do player ====
    for (int z = 0; z < altura; z++) {
        for (int x = 0; x < largura; x++) {
            world[z][x].type = TILE_WATER;
            world[z][x].flags = 0;
            world[z][x].flags |= TILE_BLOCKED;


        }
    }

    
    CreateTerrain(xpos, zpos, WORLD_W, WORLD_H, 1.4f, 1.4f);

    // ===== 🏞️ Criar chunks pra render ====
    InicializaChuncksRender(WORLD_W, WORLD_H);

    // ===== 🧊 Criar TileChunks e copiar do world =====
    CopiarTileParaMapa(WORLD_W, WORLD_H);

}


