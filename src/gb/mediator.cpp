#include "mediator.h"
#include "../cpu/cpu.h"
#include "../cartridge/cartridge.h"
#include "../addressBus/addressBus.h"
#include "../timers/timerHandler.h"

void Mediator::connect(){
    cpu.setMediator(this);
    cartridge.setMediator(this);
    addressBus.setMediator(this);
    timerHandler.setMediator(this);
}
Mediator::Mediator(CPU& _cpu, Cartridge& _cartridge, AddressBus& _addressBus, TimerHandler& _timerHandler) : 
                cpu(_cpu), cartridge(_cartridge), addressBus(_addressBus), timerHandler(_timerHandler){
                    this->connect();
                }

void Mediator::advanceState([[maybe_unused]] int cycles){
    return;
}

void Mediator::requestInterrupt(Interrupt interrupt){
    addressBus.requestInterrupt(interrupt);
}