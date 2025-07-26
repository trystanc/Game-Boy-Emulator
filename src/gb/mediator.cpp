#include "mediator.h"
#include "../cpu/cpu.h"
#include "../cartridge/cartridge.h"
#include "../addressBus/addressBus.h"
#include "../timers/timerHandler.h"
#include "../ppu/ppu.h"

void Mediator::connect(){
    cpu.setMediator(this);
    cartridge.setMediator(this);
    addressBus.setMediator(this);
    timerHandler.setMediator(this);
    ppu.setMediator(this);
}
Mediator::Mediator(CPU& _cpu, Cartridge& _cartridge, AddressBus& _addressBus, TimerHandler& _timerHandler, PPU& _ppu) : 
                cpu(_cpu), cartridge(_cartridge), addressBus(_addressBus), timerHandler(_timerHandler), ppu(_ppu){
                    this->connect();
                }

void Mediator::advanceState([[maybe_unused]] uint cycles){
    timerHandler.updateTimers(cycles);
    ppu.runCycles(cycles);
    return;
}

void Mediator::requestInterrupt(Interrupt interrupt){
    addressBus.requestInterrupt(interrupt);
}

void Mediator::updatePPUMode(u8 mode){
    addressBus[constants::LCDStatusAddress] = (addressBus[constants::LCDStatusAddress] & ~0x3) | (mode & 0x3);
}

void Mediator::updateLYequalsLYCbit(bool isLyLyc){
    u8& status = addressBus.ioRegisters[constants::LCDStatusAddress];
    status = (status & ~(1 << 2)) | (isLyLyc << 2);
}
