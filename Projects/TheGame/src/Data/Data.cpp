#include "Data.h"
#include "Projects/TheGame/config.h"

bool nomeValido(const std::string& s){
    if (s.empty()) return false;
    if (s.find("..") != std::string::npos) return false;
    if (s.find('/') != std::string::npos || s.find('\\') != std::string::npos) return false;
    return true;
}



void carregar_mundos(std::vector<MundoData>* mundos){
    mundos->clear();
    std::filesystem::path base = ResolveGamePath({ "Projects", "TheGame", "saves" });
    if (!std::filesystem::exists(base)) return;

    for (auto& entry : std::filesystem::directory_iterator(base)){
        if (!entry.is_directory()) continue;

        std::string nome = entry.path().filename().string();
        if (!nomeValido(nome)) continue;

        MundoData m;
        m.nomeMundo = nome;

        std::ifstream file(entry.path() / "world.dat", std::ios::binary);
        if (!file) continue;

        size_t len;

        file.read((char*)&len, sizeof(size_t));
        if (len > 256) continue;
        m.nomeMundo.resize(len);
        file.read(&m.nomeMundo[0], len);

        file.read((char*)&len, sizeof(size_t));
        if (len > 256) continue;
        m.seed.resize(len);
        file.read(&m.seed[0], len);

        mundos->push_back(m);
    }
}

void criar_mundo(MundoData* mundo){
    if (!nomeValido(mundo->nomeMundo)) return;

    std::filesystem::path pastaMundo = ResolveGamePath({ "Projects", "TheGame", "saves" }) / mundo->nomeMundo;
    std::filesystem::create_directories(pastaMundo);
    std::filesystem::create_directories(pastaMundo / "chunks");
    std::filesystem::create_directories(pastaMundo / "players");
    std::filesystem::create_directories(pastaMundo / "objects");

    std::ofstream file(pastaMundo / "world.dat", std::ios::binary);
    if (!file) return;

    size_t len = mundo->nomeMundo.size();
    file.write((char*)&len, sizeof(size_t));
    file.write(mundo->nomeMundo.data(), len);

    len = mundo->seed.size();
    file.write((char*)&len, sizeof(size_t));
    file.write(mundo->seed.data(), len);
}

void deletarMundo(MundoData* mundo){
    if (!nomeValido(mundo->nomeMundo)) return;

    std::filesystem::path pasta = ResolveGamePath({ "Projects", "TheGame", "saves" }) / mundo->nomeMundo;
    if (std::filesystem::exists(pasta))
        std::filesystem::remove_all(pasta);
}



std::string mundosToList(const std::vector<MundoData>& mundos){
    std::string out;
    for (size_t i = 0; i < mundos.size(); i++) {
        out += mundos[i].nomeMundo;
        if (i + 1 < mundos.size()) out += ";";
    }
    return out;
}