
#include "gameboy.h"
#include "../ppu/ppu.h"
#include "SFML/Window/Event.hpp"
#include "mediator.h"
#include <SFML/Window/Keyboard.hpp>
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

template<typename T>
void GameBoy::handleKeyEvent(const T* const ptr, bool released){
    sf::Keyboard::Scan keyCode = ptr->scancode;
    addressBus.updateButton(keyCode, released);

}
template void GameBoy::handleKeyEvent(const sf::Event::KeyPressed* const ptr, bool released);
template void GameBoy::handleKeyEvent(const sf::Event::KeyReleased* const ptr, bool released);