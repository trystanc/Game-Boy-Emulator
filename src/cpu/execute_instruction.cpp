#include "cpu.h"
#include "instructionLengths.h"
void CPU::executeInstruction(){
    u8 opcode = m_ram[pc];
    switch(opcode){
    case 0x00:
        NOP();
        break;
    case 0x01:
        break;
    case 0x02:

    case 0x06:
        
    default:
        std::cout<<"Opcode not implemented!"<<std::endl;
        break;
    }
    pc += instructionLengths[opcode];
}
