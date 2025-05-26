#pragma once

#include "../cpu/cpu.h"
#include <fstream>
#include <array>
#include <ios>
#include "../constants.h"
#include "../types.h"

class GameBoy {
    CPU m_cpu;
    std::array<u8, constants::ramSize> ram;

    public:
    GameBoy() : ram{}, m_cpu(ram) {
        
    }

    void loadRom(std::ifstream& file);

};

