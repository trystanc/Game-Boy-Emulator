#include "cpu.h"
#include "../types.h"

u16 CPU::getn16(){
    return convertLittleEndian(mem[pc+1], mem[pc+2]);

}
//helper function to store a 16 bit value at an address in little endian format
void CPU::storen16(u16 addr, u16 val){
    mem[addr] = static_cast<u8>(val);
    mem[addr+1] = static_cast<u8>(val >> 8);
}

u8 CPU::getn8(){
    return mem[pc+1];
}

void CPU::storen8(u16 addr, u8 val){
    mem[addr] = val;
}

void CPU::applyPendingIME(){
    if(imePending){
        ime = true;
        imePending = false;
    }
}