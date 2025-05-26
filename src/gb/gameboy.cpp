    #include <iostream>
    #include "gameboy.h"
    
    void GameBoy::loadRom(std::ifstream& file){
        file.seekg(0, std::ios::end);
        std::streamsize size {file.tellg()};
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*> (ram.data() + 0), size);
    }