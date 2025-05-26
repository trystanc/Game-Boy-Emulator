#include "cpu.h"
#include "../types.h"
namespace{

u16 convertLittleEndian(u8 firstByte, u8 secondByte){
    return (firstByte | static_cast<u16>(secondByte) << 8);
}

}

u16 CPU::getN16(){
    return convertLittleEndian(m_ram[pc+1], m_ram[pc+2]);

}
//stores 16 bit value at address little endian.
void CPU::storeN16(u16 addr, u16 val){
    m_ram[addr] = static_cast<u8>(val);
    m_ram[addr+1] = static_cast<u8>(val >> 8);
}



u8 CPU::getn8(){ return m_ram[pc+1];}


void CPU::NOP(){
    return;
}

//load nstructions
void CPU::ld_r8_r8(Register& register1, Register& register2){
    register1 = register2;

}

void CPU::ld_r8_n8(Register& r8){
    r8 = getn8();
}

void CPU::ld_r16_n16(RegisterPair& r16){
    r16 = getN16();
}

void CPU::ld_mr16_r8(RegisterPair& r16, Register& r8){
    m_ram[r16] = r8;

}

void CPU::ld_mr16_n8(RegisterPair& r16){
    m_ram[r16] = getn8();
}

void CPU::ld_r8_mr16(Register& r8, RegisterPair& r16){
    r8 = m_ram[r16];
}

void CPU::ld_mr16_A(RegisterPair& r16){
    m_ram[r16] = A;
}

void CPU::ld_mn16_A(){
    m_ram[getN16()] = A;
}

void CPU::ldh_mn16_A(){
    u16 n16 = 0xff00 + getn8();
    m_ram[n16] = A;
}

void CPU::ldh_mC_A(){
    m_ram[0xff00+C] = A;
}

void CPU::ld_A_mr16(RegisterPair& r16){
    A = m_ram[r16];
}

void CPU::ldh_A_mn16(){
    A = m_ram[0xff00 + getn8()];
}

void CPU::ldh_A_mC(){
    A = m_ram[0xff00 + C];
}

void CPU::ld_mHLi_A(){
    m_ram[HL] = A;
    ++HL;
}

void CPU::ld_mHLd_A(){
    m_ram[HL] = A;
    --HL;
}

void CPU::ld_A_mHLd(){
    A = m_ram[HL];
    --HL;
}

void CPU::ld_A_mHLi(){
    A = m_ram[HL];
    ++HL;
}

void CPU::ld_SP_n16(){
    sp = getN16();
}

void CPU::ld_mn16_SP(){
    u16 n16 = getN16();
    storeN16(n16, sp);
}

void CPU::ld_HL_SPe8(){
    u8 n8 = getn8();

    s8 e8 = static_cast<s8>(n8);
    int result = static_cast<int>(sp) + e8;
    bool carry = ((sp &0xff) + (n8 & 0xff)) > 0xff;
    bool halfCarry = ((sp & 0xf) + (n8 & 0xf)) > 0xf;
    setFlags(false, false, halfCarry, carry);

    HL = static_cast<u16>(result);

}

void CPU::ld_SP_HL(){
    sp = HL;

}

