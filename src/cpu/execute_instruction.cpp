#include "cpu.h"
#include "instructionLengths.h"
CPU::executeInstruction(){
    opcode = m_ram[pc];
    switch(opcode){
    case 0x00:
        NOP();
        break;
    case 0x01:
        ldR16Imm16(BC);
        break;
    case 0x02:
        ldMr16R8(BC. A);
    case 0x06:
        ldR8Imm8(BC.setHi())
        break;
    default:
        std::cout<<"Opcode not implemented!"<std::endl;
        break;
    }
    pc += instructionLengths[opcode];
}
