#pragma once

#include "../cpu/cpu.h"
#include <fstream>
#include <array>
#include <ios>
#include "../constants.h"
#include "../types.h"
#include "../cartridge/cartridge.h"
#include "../addressBus/addressBus.h"
#include "time.h"

class GameBoy {

    Cartridge cartridge;
    AddressBus addressBus;
    CPU cpu;
public:

    GameBoy(std::ifstream & file);

};

