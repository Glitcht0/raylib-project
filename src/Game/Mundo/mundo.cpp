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

    unloadFarChunks(playerPos, 2);
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
        //CarregarAreaInicial(WORLD_W, WORLD_H); // Carrega a área inicial (mesmo tamanho que você usaria para criar a ilha)
    } 
    else {
        printf("--- NENHUM SAVE: GERANDO NOVO MUNDO ---\n");
        CreatIsland(0, 0, 100, 100); // Gera do zero 
        CreatIsland(100, 100, 100, 100);
        CreatIsland(100, 0, 50, 50);

        //InicializaChuncksRender(WORLD_W, WORLD_H);


    }

    mundo_gerado = true;
}



void World::CreatIsland(int xpos, int zpos, int largura, int altura) {
    printf("--- Criando Ilha em (%d, %d) tamanho %dx%d ---\n", xpos, zpos, largura, altura);

    // 1. PREPARAÇÃO DA PRANCHETA (Limpeza Local)
    // Preenchemos a região alvo no 'world' com água antes de desenhar a terra.
    // Isso garante que o Perlin Noise não misture com lixo de memória anterior.
    int zEnd = (zpos + altura > WORLD_H) ? WORLD_H : zpos + altura;
    int xEnd = (xpos + largura > WORLD_W) ? WORLD_W : xpos + largura;

    for (int z = zpos; z < zEnd; z++) {
        for (int x = xpos; x < xEnd; x++) {
            if (z >= 0 && x >= 0) { // Safety check
                world[z][x].type = TILE_WATER;
                world[z][x].flags = TILE_BLOCKED;
            }
        }
    }

    
    CreateTerrain(zpos, xpos, largura, altura, 1.4f, 1.4f); // 2. GERAÇÃO (Desenha na prancheta 'world')

    
    AtualizarESalvarRegiao(xpos, zpos, largura, altura); // 3. CONSOLIDAÇÃO (Copia do 'world' para Chunks e Salva)

    printf("--- Ilha Salva no Disco ---\n");
}


