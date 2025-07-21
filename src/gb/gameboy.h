#pragma once

#include "../cpu/cpu.h"
#include <fstream>
#include <array>
#include <ios>
#include "../constants.h"
#include "../types.h"
#include "../cartridge/cartridge.h"
#include "../addressBus/addressBus.h"
#include "mediator.h"
#include "../timers/timerHandler.h"

//It would be a good idea if this class can communicate to the addressBus what mode the PPU for read-only mode.
//DMA still needs to be done at some point... 
class GameBoy {
    Cartridge cartridge;
    AddressBus addressBus;
    CPU cpu;
    TimerHandler timerHandler;
    Mediator mediator;
public:

    GameBoy(std::ifstream & file);

};

