#include "../cpu/cpu.h"
using enum Interrupt;

constexpr std::array<u16, int(numOfInterrupts)> interruptAddresses
    {0x40, 0x48, 0x50, 0x58, 0x60};

//returns true if an intterupt occurs
bool CPU::handleInterrupts(){
    if (imePending){
        ime = true;
        imePending = false;
        return false;
    }
    if(ime){
        for(size_t i {0}; i < size_t(numOfInterrupts); ++i){
            bool serviceInterrupt {((mem[constants::interruptFlagAddress] >> i) & 1) && 
                                   ((mem[constants::interruptEnableAddress] >> i) & 1) };
            if (serviceInterrupt){
                storen16(sp-2, pc);
                sp -= 2;
                pc = interruptAddresses[i];
                ime = false;
                mem[constants::interruptFlagAddress] = mem[constants::interruptFlagAddress] & static_cast<u8>(~(1 << i));
                return true;
            }
        }
    }
    return false;
}