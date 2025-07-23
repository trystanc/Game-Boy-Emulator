#pragma once
#include "../types.h"
class CPU;
class AddressBus;
class Cartridge;
class TimerHandler;
class Mediator{
public:
    Mediator(CPU& _cpu, Cartridge& _cartridge, AddressBus& _addressBus, TimerHandler& _timerHandler);
    void advanceState(int cycles); //to be called by CPU
    virtual void requestInterrupt(Interrupt interrupt);
    void updatePPUMode(u8 mode);
    void updateLYequalsLYCbit(bool isLyLyc);
private:
    CPU& cpu;
    Cartridge& cartridge;
    AddressBus& addressBus;
    TimerHandler& timerHandler;
    void connect();
};
