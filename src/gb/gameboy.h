#pragma once

#include "../cpu/cpu.h"
#include <fstream>
#include <array>
#include <ios>
#include "../constants.h"
#include "../types.h"
#include "../cartridge/cartridge.h"
#include "../addressBus/addressBus.h"

class GameBoy {

    Cartridge m_cartridge;
    AddressBus m_addressBus;

public:
    CPU m_cpu;
    GameBoy(std::ifstream & file);

};

