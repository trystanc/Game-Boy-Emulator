#include "cpu.h"
#include "../types.h"
#include "../constants.h"

int CPU::mainLoop(){
    int cycleNumbers = serviceInterrupts();
    if(!(cycleNumbers)) {
        cycleNumbers = executeInstruction(); 
        applyPendingIME();
    }
    return cycleNumbers;
}

static constexpr std::array<u16, 5> interruptTable {40, 48, 50, 58, 60};

//returns number of cycles required to return
int CPU::serviceInterrupts(){
    
    if (ime == false) return 0;
    u8 interruptFlag = mem[constants::interruptFlagAddress];
    u8 interruptEnable = mem[constants::interruptEnableAddress];
    u8 validInterrupt = interruptEnable & interruptFlag;
    for( int i{0}; i < 5; ++i){
        if( (validInterrupt >> i) & 0b1){
            ime = false;
            interruptFlag &= ~(1<<i);
            storen16(sp-2, pc);
            sp -= 2;
            pc = interruptTable[i];
            return 5;
        }
    return 0;
    }



}

