#include "cartridge.h"
#include <iostream>
#include <fstream>
#include <vector>

std::vector<u8> readFileToVector(std::ifstream &file) {
    file.seekg(0, std::ios::end);
    std::streamsize size {file.tellg()};
    file.seekg(0, std::ios::beg);
    std::vector<u8> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    return buffer;
}

Cartridge::Cartridge(std::ifstream &file) 
        : cartridgeData(readFileToVector(file)) {}

