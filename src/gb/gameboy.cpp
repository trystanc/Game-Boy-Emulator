
#include "gameboy.h"
#include "../graphics/ppu.h"

GameBoy::GameBoy(std::ifstream &file)
    : cartridge(file),
      addressBus(cartridge),
      cpu(addressBus),
      timerHandler(addressBus),
      mediator(cpu, cartridge, addressBus, timerHandler)
{
}
