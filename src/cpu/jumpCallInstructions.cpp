#include "cpu.h"
#include "../types.h"

void CPU::call_n16(){
//store the current program counter on the stack
    storen16(sp-2, pc+3);
    sp -= 2;
//set the program counter to the new address
    pc = getn16();
    jumped = true;

}

void CPU::call_cc_n16(bool condition) {
    if (condition) {
        call_n16();
    }
}

void CPU::jp_n16(){
    pc = getn16();
    jumped = true;

}

void CPU::jp_cc_n16(bool condition) {
    if (condition) {
        jp_n16();

    }
}

void CPU::jp_HL(){
    pc = HL;
    jumped = true;
}

void CPU::jr_n16(){
    s8 offset = static_cast<s8>(getn8());
    u16 newPC = static_cast<u16>(pc + offset + 2);
    pc = newPC;
    jumped = true;
}

void CPU::jr_cc_n16(bool condition) {

    if (condition) {
        jr_n16();

    }

}


void CPU::ret(){
    pc = ((mem[sp+1] <<  8) | mem[sp]);
    sp += 2;
    jumped = true;
}

void CPU::ret_cc(bool condition) {
    if (condition) {
        ret();
    }
}

void CPU::reti(){
    ret();
    ime = true;
}

void CPU::rst(u8 vector) {
    storen16(sp - 2, pc+1);
    sp -= 2;
    pc = vector;
    jumped = true;
}
