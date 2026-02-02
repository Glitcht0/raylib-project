#include "Data.h"



void carregar_mundos(std::vector<MundoData> *mundos)
{
    mundos->clear();

    const std::string pasta = "saves";

    if (!std::filesystem::exists(pasta)) return;

    for (const auto& entry : std::filesystem::directory_iterator(pasta))
    {
        if (entry.is_directory())
        {
            MundoData m;
            m.nomeMundo = entry.path().filename().string();
            m.seed = ""; // ainda nao existe

            std::ifstream file(entry.path().string() + "/world.dat", std::ios::binary);
            if (file){
                size_t len;
                file.read((char*)&len, sizeof(size_t));
                m.nomeMundo.resize(len);
                file.read(&m.nomeMundo[0], len);

                file.read((char*)&len, sizeof(size_t));
                m.seed.resize(len);
                file.read(&m.seed[0], len);
            }

            mundos->push_back(m);
        }
    }
}


void deletarMundo(MundoData *mundo){
    const std::string pasta = "saves/" + mundo->nomeMundo;

    if (std::filesystem::exists(pasta)) {
        std::filesystem::remove_all(pasta);
    }
}

void criar_mundo(MundoData *mundo){
    const std::string pastaMundo = "saves/" + mundo->nomeMundo;

    std::filesystem::create_directories(pastaMundo);

    // arquivo binário
    std::ofstream file(pastaMundo + "/world.dat", std::ios::binary);
    if (!file) return;

    //Mede o tamanho
    size_t lenNome = mundo->nomeMundo.size();
    size_t lenSeed = mundo->seed.size();

    
    file.write((char*)&lenNome, sizeof(size_t));
    file.write(mundo->nomeMundo.c_str(), lenNome);

    file.write((char*)&lenSeed, sizeof(size_t));
    file.write(mundo->seed.c_str(), lenSeed);

    file.close();

}



std::string mundosToList(const std::vector<MundoData>& mundos){
    std::string out;
    for (size_t i = 0; i < mundos.size(); i++) {
        out += mundos[i].nomeMundo;
        if (i + 1 < mundos.size()) out += ";";
    }
    return out;
}