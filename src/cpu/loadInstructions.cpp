#include "cpu.h"
#include "../types.h"

void CPU::ld_r8_r8(Register& register1, Register& register2){
    register1 = register2;

}

void CPU::ld_r8_n8(Register& r8){
    r8 = getn8();
}

void CPU::ld_r16_n16(RegisterPair& r16){
    r16 = getn16();
}

void CPU::ld_mr16_r8(RegisterPair& r16, Register& r8){
    mem[r16] = r8;

}

void CPU::ld_mr16_n8(RegisterPair& r16){
    mem[r16] = getn8();
}

void CPU::ld_r8_mr16(Register& r8, RegisterPair& r16){
    r8 = mem[r16];
}

void CPU::ld_mr16_A(RegisterPair& r16){
    mem[r16] = A;
}

void CPU::ld_mn16_A(){
    mem[getn16()] = A;
}

void CPU::ldh_mn16_A(){
    u16 n16 = 0xff00 + getn8();
    mem[n16] = A;
}

void CPU::ldh_mC_A(){
    mem[0xff00+C] = A;
}

void CPU::ld_A_mr16(RegisterPair& r16){
    A = mem[r16];
}

void CPU::ldh_A_mn16(){
    A = mem[0xff00 + getn8()];
}

void CPU::ldh_A_mC(){
    A = mem[0xff00 + C];
}

void CPU::ld_mHLi_A(){
    mem[HL] = A;
    ++HL;
}

void CPU::ld_mHLd_A(){
    mem[HL] = A;
    --HL;
}

void CPU::ld_A_mHLd(){
    A = mem[HL];
    --HL;
}

void CPU::ld_A_mHLi(){
    A = mem[HL];
    ++HL;
}

void CPU::ld_SP_n16(){
    sp = getn16();
}

void CPU::ld_mn16_SP(){
    u16 n16 = getn16();
    storen16(n16, sp);
}

void CPU::ld_HL_SPe8(){
    HL = SPe8();
}

void CPU::ld_SP_HL(){
    sp = HL;
}

void CPU::pop(RegisterPair& r16){

    r16.lo = mem[sp];
    ++sp;
    r16.hi = mem[sp];
    ++sp;
}

void CPU::push_AF(){

    --sp;
    mem[sp] = A;
    --sp;
    mem[sp] = F & 0xf0;;
}

void CPU::push_r16(RegisterPair& r16){

    --sp;
    mem[sp] = r16.hi;
    --sp;
    mem[sp] = r16.lo;
}