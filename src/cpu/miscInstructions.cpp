#include "cpu.h"
#include "../types.h"

void CPU::di(){
    ime = false;
}

void CPU::ei(){
    imePending = true;
}

void CPU::halt(){
    //placeholder for halt instruction
}

void CPU::stop(){
    //placeholder for stop instruction
    mem[constants::DIVaddress] = 0;
}

void CPU::nop(){
    return;
}







