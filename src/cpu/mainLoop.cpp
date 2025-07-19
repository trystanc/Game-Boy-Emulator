#include "cpu.h"
#include "../types.h"
#include "../constants.h"

int CPU::runCycles(){
    int cycleNumbers = executeInstruction(); 
    applyPendingIME();
    return cycleNumbers;
}



