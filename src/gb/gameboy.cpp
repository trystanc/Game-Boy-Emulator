
#include "gameboy.h"
#include "../graphics/ppu.h"

GameBoy::GameBoy(std::ifstream &file)
    : cartridge(file),
      addressBus(cartridge),
      cpu(addressBus),
      ppu(addressBus)

{
}

void GameBoy::runCycles(){
  int cycles = cpu.runCycles();
  ppu.runCycles(cycles);

}
