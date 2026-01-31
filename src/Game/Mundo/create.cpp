#include "mundo.h"







void World::CreateTerrain(int zpos, int xpos, int largura, int altura, float raio, float elevacao){
    // ===== CLAMP DOS LIMITES DO MUNDO =====
    int zEnd = zpos + altura;
    int xEnd = xpos + largura;

    if (zpos < 0) zpos = 0;
    if (xpos < 0) xpos = 0;

    if (zEnd > WORLD_H) zEnd = WORLD_H; 
    if (xEnd > WORLD_W) xEnd = WORLD_W;



    // ===== GERA A ILHA =====
    float cx = xpos + largura * 0.5f;
    float cz = zpos + altura * 0.5f;

    for (int z = zpos; z < zEnd; z++) {
        for (int x = xpos; x < xEnd; x++) {


            double n = perlin.octave2D_01( x * scale, z * scale, octaves);



            float dx = (x - cx) / (largura * 0.5f * raio);
            float dz = (z - cz) / (altura  * 0.5f * raio);

            float dist = sqrtf(dx*dx + dz*dz);
            float mask = 1.0f - Clamp(dist, 0.0f, 1.0f);

            mask = powf(mask, elevacao);  // Numero maior, pico no centro, menor, mais plano

            // Altura final
            n = n * mask;

            // === Definição dos Tiles ===
            if (n < 0.15) {
                world[z][x].type = TILE_WATER;
                world[z][x].flags |= TILE_BLOCKED;

            }
            else if (n < 0.20) {
                world[z][x].type = TILE_SAND;
                world[z][x].flags &= ~TILE_BLOCKED;

            }
            else if (n < 0.25) {
                world[z][x].type = TILE_DIRT;
                world[z][x].flags &= ~TILE_BLOCKED;

            }
            else if (n<= 1.0) {
                world[z][x].type = TILE_GRASS;
                world[z][x].flags &= ~TILE_BLOCKED;

            }
            else {
                world[z][x].type = TILE_WATER;
                world[z][x].flags |= TILE_BLOCKED;

            }
            
            // 🔹 RUÍDO SUTIL NA PRAIA (AQUI)
            //if (world[z][x].type == TILE_SAND && n < 0.09) {
            //    world[z][x].type = TILE_WATER;
            //    world[z][x].blocked = true;
            //}
            
        }
    }

    applyRules(zpos, xpos, largura, altura);
    applyRules(zpos, xpos, largura, altura);
    applyRules(zpos, xpos, largura, altura);
    applyRules(zpos, xpos, largura, altura);
}







// Controi as meshs das Chunks
void World::buildChunkMesh(Chunk& chunk) {
    Mesh mesh = { 0 };

    int tiles = CHUNK_SIZE * CHUNK_SIZE;
    int vertexCount = tiles * 6;

    mesh.vertexCount = vertexCount;
    mesh.triangleCount = vertexCount / 3;

    mesh.vertices = (float*)MemAlloc(vertexCount * 3 * sizeof(float));
    mesh.colors   = (unsigned char*)MemAlloc(vertexCount * 4 * sizeof(unsigned char));
    mesh.normals  = (float*)MemAlloc(vertexCount * 3 * sizeof(float));
    int v = 0, c = 0, n = 0; // ⬅️ declare antes de usar



    TileChunk* tileChunk = GetTileChunk(chunk.cx, chunk.cz);

    if (!tileChunk) return; // segurança
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            Tile& tile = tileChunk->tiles[z][x]; 

        
            Vector3 p = { (chunk.cx * CHUNK_SIZE + x) + 0.5f, 0.0f, (chunk.cz * CHUNK_SIZE + z) + 0.5f };

            Color col;
            switch (tile.type) {
                case TILE_GRASS: col = COR_GRAMA_VERDE; break;
                case TILE_DIRT:  col = BROWN; break;
                case TILE_WATER: col = BLUE; break;
                case TILE_SAND:  col = COR_AREIA; break;
            }

            Vector3 v0 = { p.x - 0.5f, 0, p.z - 0.5f };
            Vector3 v1 = { p.x + 0.5f, 0, p.z - 0.5f };
            Vector3 v2 = { p.x - 0.5f, 0, p.z + 0.5f };
            Vector3 v3 = { p.x + 0.5f, 0, p.z + 0.5f };

            Vector3 quad[6] = { v0, v2, v1, v1, v2, v3 };

            for (int i = 0; i < 6; i++) {
                mesh.vertices[v++] = quad[i].x;
                mesh.vertices[v++] = quad[i].y;
                mesh.vertices[v++] = quad[i].z;

                mesh.colors[c++] = col.r;
                mesh.colors[c++] = col.g;
                mesh.colors[c++] = col.b;
                mesh.colors[c++] = col.a;

                mesh.normals[n++] = 0;
                mesh.normals[n++] = 1;
                mesh.normals[n++] = 0;
            }
        }
    }

    UploadMesh(&mesh, false);
    chunk.model = LoadModelFromMesh(mesh);
    chunk.built = true;

    if (terrainShader.id != 0)
        chunk.model.materials[0].shader = terrainShader;
}



// ===== 🏞️ Criar chunks sem malha pra render ====
void World::InicializaChuncksRender(int largura, int altura){
    int chunksX = (largura + CHUNK_SIZE - 1) / CHUNK_SIZE;
    int chunksZ = (altura + CHUNK_SIZE - 1) / CHUNK_SIZE;

    chunks.reserve(chunksX * chunksZ);

    for (int cz = 0; cz < chunksZ; cz++) {
        for (int cx = 0; cx < chunksX; cx++) {
            Chunk c;
            c.cx = cx;
            c.cz = cz;
            c.built = false;
            chunks.push_back(c);
        }
    }
}



// Função inteligente que pega apenas um pedaço do 'world' e salva nos chunks
void World::AtualizarESalvarRegiao(int xpos, int zpos, int largura, int altura) {
    
    // 1. Calcula quais chunks foram afetados por essa ilha
    int startCX = xpos / CHUNK_SIZE;
    int startCZ = zpos / CHUNK_SIZE;
    int endCX   = (xpos + largura + CHUNK_SIZE - 1) / CHUNK_SIZE; // Arredonda pra cima
    int endCZ   = (zpos + altura + CHUNK_SIZE - 1) / CHUNK_SIZE;

    // 2. Percorre APENAS os chunks envolvidos
    for (int cz = startCZ; cz < endCZ; cz++) {
        for (int cx = startCX; cx < endCX; cx++) {
            
            
            TileChunk& tc = getOrCreateTileChunk(cx, cz); // Pega ou cria o chunk na memória
        
            
            bool mudouAlgo = false;  // Flag para saber se precisamos salvar este chunk no disco

            // 3. Atualiza os tiles DENTRO deste chunk
            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    
                    // Coordenada Global do Tile
                    int worldX = cx * CHUNK_SIZE + x;
                    int worldZ = cz * CHUNK_SIZE + z;

                    // Verifica se esse tile está dentro da região da ilha que acabamos de gerar
                    if (worldX >= xpos && worldX < xpos + largura &&
                        worldZ >= zpos && worldZ < zpos + altura) {
                        
                        // Verifica limites globais (pra não estourar o array world)
                        if (worldX < WORLD_W && worldZ < WORLD_H) {
                            // COPIA DA PRANCHETA PARA O CHUNK
                            tc.tiles[z][x] = world[worldZ][worldX];
                            mudouAlgo = true;
                        }
                    }
                }
            }

            // 4. Se o chunk foi modificado, salva no disco imediatamente
            if (mudouAlgo) {
                saveChunkToDisk(tc);
                // Opcional: Se já houver mesh visual gerada, marcar para recriar
                // chunks[getOrCreateMeshChunk(cx, cz)].built = false; 
            }
        }
    }
}


void World::CarregarAreaInicial(int largura, int altura) {
    chunkData.clear(); // Limpa dados antigos

    int chunksX = (largura + CHUNK_SIZE - 1) / CHUNK_SIZE;
    int chunksZ = (altura + CHUNK_SIZE - 1) / CHUNK_SIZE;

    printf("Carregando area inicial (%dx%d chunks)...\n", chunksX, chunksZ);

    for (int cz = 0; cz < chunksZ; cz++) {
        for (int cx = 0; cx < chunksX; cx++) {
            
            TileChunk tc;
            // Tenta carregar do disco
            if (loadChunkFromDisk(cx, cz, tc)) {
                // Se achou, salva no mapa principal da memória
                long long key = ChunkKey(cx, cz);
                chunkData[key] = tc;
            } else {
                // Se o save existe mas esse chunk específico sumiu, gera um vazio/água
                // (opcional: ou você pode chamar generateSingleChunk(tc) aqui)
                tc.cx = cx; tc.cz = cz; tc.built = true;
                generateSingleChunk(tc); 
                chunkData[ChunkKey(cx, cz)] = tc;
            }
        }
    }
    
    // Inicializa a estrutura de visualização (chunks sem malha)
    InicializaChuncksRender(largura, altura);
}




void World::applyRules(int zpos, int xpos, int largura, int altura) {


    // ===== CLAMP DOS LIMITES DO MUNDO =====
    int zEnd = zpos + altura;
    int xEnd = xpos + largura;

    if (zpos < 0) zpos = 0;
    if (xpos < 0) xpos = 0;
    if (zEnd > WORLD_H) zEnd = WORLD_H;
    if (xEnd > WORLD_W) xEnd = WORLD_W;


    for (int z = zpos; z < zEnd; z++) {
        for (int x = xpos; x < xEnd; x++) {

            int neighbors = countSameNeighbors(x, z);

            // regra: tile isolado ou quase isolado
            if (neighbors <= 1) {
                TileType t = mostCommonNeighbor(x, z);
                world[z][x].type = t;

                if (t == TILE_WATER)
                    world[z][x].flags |= TILE_BLOCKED;    // seta
                else
                    world[z][x].flags &= ~TILE_BLOCKED;   // limpa
            }

        }
    }

}



TileType World::mostCommonNeighbor(int x, int z) {
    int grass = 0, dirt = 0, water = 0;

    auto count = [&](int nx, int nz) {
        switch (world[nz][nx].type) {
        case TILE_GRASS: grass++; break;
        case TILE_DIRT:  dirt++;  break;
        case TILE_WATER: water++; break;
        default: break; // IGNORA SAND
        }
    };

    if (x > 0) count(x - 1, z);
    if (x < WORLD_W - 1) count(x + 1, z);
    if (z > 0) count(x, z - 1);
    if (z < WORLD_H - 1) count(x, z + 1);

    if (water >= grass && water >= dirt) return TILE_WATER;
    if (grass >= dirt) return TILE_GRASS;
    return TILE_DIRT;
}


void World::LimparBufferComAgua() {
    for (int z = 0; z < WORLD_H; z++) {
        for (int x = 0; x < WORLD_W; x++) {
            world[z][x].type = TILE_WATER;
            world[z][x].flags = TILE_BLOCKED;
        }
    }
}


int World::countSameNeighbors(int x, int z) {
    TileType t = world[z][x].type;
    int count = 0;

    if (x > 0 && world[z][x - 1].type == t) count++;
    if (x < WORLD_W - 1 && world[z][x + 1].type == t) count++;
    if (z > 0 && world[z - 1][x].type == t) count++;
    if (z < WORLD_H - 1 && world[z + 1][x].type == t) count++;

    return count;
}
