
#include "gameboy.h"
#include "../ppu/ppu.h"
#include "mediator.h"

GameBoy::GameBoy(std::ifstream &file, Emulator& emulator)
    : cartridge{file},
      addressBus{cartridge},
      cpu{addressBus},
      timerHandler{addressBus},
      ppu{addressBus},
      mediator(cpu, cartridge, addressBus, timerHandler, ppu, emulator)
{
}

void GameBoy::advance(){
  cpu.tick();
}

const u8* GameBoy::getFrameBuffer(){
  return ppu.getFrameBuffer();
}
