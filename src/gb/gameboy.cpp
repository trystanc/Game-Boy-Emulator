
#include "gameboy.h"
#include "../ppu/ppu.h"

GameBoy::GameBoy(std::ifstream &file)
    : cartridge{file},
      addressBus{cartridge},
      cpu{addressBus},
      timerHandler{addressBus},
      ppu{addressBus},
      mediator(cpu, cartridge, addressBus, timerHandler, ppu)
{
}

