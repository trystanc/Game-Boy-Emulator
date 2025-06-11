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
}

void CPU::nop(){
    return;
}







