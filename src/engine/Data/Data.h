#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

struct MundoData{
    std::string nomeMundo;
    std::string seed;
};



void carregar_mundos(std::vector<MundoData> *mundos);
void criar_mundo(MundoData *mundo);
void deletarMundo(MundoData *mundo);


std::string mundosToList(const std::vector<MundoData>& mundos);