#include "cpu.h"
#include "../types.h"
namespace{
 u16 convertLittleEndian(u8 firstByte, u8 secondByte){
    return (firstByte | static_cast<u16>(secondByte) << 8);
}

}

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


u8 CPU::add8bit(const uint first, const uint second){
    u8 result = static_cast<u8>(first) + static_cast<u8>(second);
    bool zero = (result == 0);
    bool halfCarry = ((first & 0xf) + (second & 0xf)) > 0xf;
    bool carry = ((first & 0xff) + (second & 0xff)) > 0xff;
    setFlags(zero, false, halfCarry, carry);
    return result;

}

u16 CPU::add16bit(const uint first, const uint second){
    u16 result = static_cast<u16>(first) + static_cast<u16>(second);
    bool halfCarry = ((first & 0xfff) + (second & 0xfff)) > 0xfff;
    bool carry = ((first & 0xffff) + (second & 0xffff)) > 0xffff;
    setFlags(Cflag(), false, halfCarry, carry);
    return result;
}

u16 CPU::SPe8(){
    u8 n8 = getn8();
    s8 e8 = static_cast<s8>(n8);
    int result = static_cast<int>(sp) + e8;
    bool carry = ((sp &0xff) + (n8 & 0xff)) > 0xff;
    bool halfCarry = ((sp & 0xf) + (n8 & 0xf)) > 0xf;
    setFlags(false, false, halfCarry, carry);
    return static_cast<u16>(result);
}

/*
To see the definition of these opcodes, see the website:
https://rgbds.gbdev.io/docs/v0.9.2/gbz80.7
*/


//stores 16 bit value at address little endian.

void CPU::adc_A_r8(Register r8){
    A = add8bit(A, r8 + Cflag());
}

void CPU::adc_A_mHL(){
    A = add8bit(A, mem[HL] + Cflag());        
}

void CPU::adc_A_n8(){
    A += static_cast<u8>(getn8() + static_cast<u8>(Cflag()));
}

void CPU::add_A_r8(Register r8){
    A = add8bit(A, r8);
}

void CPU::add_A_mHL(){
    A = add8bit(A, mem[HL]);
}

void CPU::add_A_n8(){
    A = add8bit(A, getn8());
}

void CPU::add_HL_r16(RegisterPair& r16){
    HL = add16bit(HL, r16);
}

void CPU::add_HL_SP(){
    HL = add16bit(HL, sp);
}

void CPU::add_SP_e8(){
    sp = SPe8();
}

void CPU::NOP(){
    return;
}

//load instructions
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




