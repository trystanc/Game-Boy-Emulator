#pragma once
#include "../types.h"
class CPU;
class AddressBus;
class Cartridge;
class TimerHandler;
class PPU;
class Emulator;
class Mediator{
public:
    Mediator(CPU& _cpu, Cartridge& _cartridge, AddressBus& _addressBus, TimerHandler& _timerHandler, PPU& _ppu, Emulator& _emulator);
    void advanceState(uint cycles); //to be called by CPU
    virtual void requestInterrupt(Interrupt interrupt);
    void updatePPUMode(u8 mode);
    void updateLYequalsLYCbit(bool isLyLyc);
    void notifyFrameDone();
private:
    CPU& cpu;
    Cartridge& cartridge;
    AddressBus& addressBus;
    TimerHandler& timerHandler;
    PPU& ppu;
    Emulator& emulator;
    void connect();
};
