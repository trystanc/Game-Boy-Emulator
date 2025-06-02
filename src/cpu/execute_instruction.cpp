#include "cpu.h"
#include "opcodeData.h"
#include "../types.h"
/* Executes non-CB functions. Resets the branched flag before executing the instruction. 
This may be modified by the instruction.  If the instruction is a branch instruction and 
it was taken, the flag will be set to true. Affects the number of cycles the instruction 
takes to execute and hence the return value of this function.*/

int CPU::executeInstruction(){

    branched = false;
    u8 opcode = mem[pc];
    switch(opcode){
    case 0x00:
        NOP();
        break;
    case 0x01:
        ld_r16_n16(BC);
        break;
    case 0x02:
        ld_mr16_r8(BC,A);
        break;
    case 0x03:
    case 0x06:
        
    default:
        std::cout<<"Opcode not implemented!"<<std::endl;
        break;
    }
    pc += instructionLengths[opcode];
    return branched ? opcode_cycles_branched[opcode] : opcode_cycles[opcode];
}
