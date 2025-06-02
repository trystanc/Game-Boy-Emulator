#pragma once
#include <array>
#include "../constants.h"
#include "RegisterPair.h"
#include "../types.h"
#include "../addressBus/addressBus.h"

using Register = u8;

class CPU{

    enum flagPosition{
        ZPos = 7,
        NPos = 6,
        HPos = 5,
        CPos = 4,
    };
    AddressBus& mem;

//Value of Registers after boot sequence
    Register A{0x01};
    Register F {0xB0};
    Register B{0x00};
    Register C{0x13};
    Register D{0x00};
    Register E{0xD8};
    Register H{0x01};
    Register L{0x4D};
    u16 pc {0x0100};
    u16 sp {0xFFFE}; 

    RegisterPair AF;
    RegisterPair BC;
    RegisterPair DE;
    RegisterPair HL;

    friend class RegisterPair;
    friend class Debugger;

    bool getFlag(flagPosition pos) const {
        return (F >> pos) & 0x1;
    }
    void setFlag(flagPosition pos, bool val){
        auto value = static_cast<u8>(val);
        F = static_cast<u8>((F & ~(1u << pos)) | (value << pos));
    }
    bool Zflag() const {return getFlag(ZPos);}
    bool Nflag() const {return getFlag(NPos);}
    bool Hflag() const {return getFlag(HPos);}
    bool Cflag() const {return getFlag(CPos);}

    void setZ(bool val) { setFlag(ZPos, val); }
    void setN(bool val) { setFlag(NPos, val); }
    void setH(bool val) { setFlag(HPos, val); }
    void setC(bool val) { setFlag(CPos, val); }
//indicates if the last instruction was a branch instruction and was taken.
    bool branched {false}; 
public:
   CPU(AddressBus& addressBus)
        : mem(addressBus),
          AF(A, F),
          BC(B, C),
          DE(D, E),
          HL(H, L) {}


    void setFlags(bool zero, bool arithmetic, bool halfCarry, bool carry){
        setZ(zero);
        setN(arithmetic);
        setH(halfCarry);
        setC(carry);
    }

    int executeInstruction();


//Function declarations for executing specific instructions.
 private:   
 //helper functions
    u8 add8bit(const uint first, const uint second);
    u16 add16bit(const uint first, const uint second);
    void setn8(u8 value);
    u16 getn16();
    u8 getn8();
    void storen16(u16 addr, u16 val);
    void storen8(u16 addr, u8 val);
    u16 SPe8();
    
//Arithmetic instructions
    void adc_A_r8(Register r8);
    void adc_A_mHL();
    void adc_A_n8();
    void add_A_r8(Register r8);
    void add_A_mHL();
    void add_A_n8();
    void add_HL_r16(RegisterPair& r16);
    void add_SP_n8();
    void add_HL_SP();
    void add_SP_e8();
    void and_A(u8& val);
    void ccf();
    void cp_A(u8& val);
    void cpl();
    void daa();
    void dec8bit(u8& value);
    void dec16bit(RegisterPair& r16);
    void inc8bit(u8& value);
    void inc16bit(RegisterPair& r16);
    void or_A(u8& val);
    void sbc_A(u8& value);
    void scf();
    void sub_A(u8& value);
    void xor_A(u8& val);

//Load instructions
    void ld_r8_r8(Register& register1, Register& register2);
    void ld_r8_n8(Register& r8);
    void ld_r16_n16(RegisterPair& r16);
    void ld_mr16_r8(RegisterPair& r16, Register& r8);
    void ld_mr16_n8(RegisterPair& r16);
    void ld_r8_mr16(Register& r8, RegisterPair& r16);
    void ld_mr16_A(RegisterPair& r16);
    void ld_mn16_A();
    void ldh_mn16_A();
    void ldh_mC_A();
    void ld_A_mr16(RegisterPair& r16);
    void ldh_A_mn16();
    void ldh_A_mC();
    void ld_mHLi_A();
    void ld_mHLd_A();
    void ld_A_mHLd();
    void ld_A_mHLi();
    void ld_SP_n16();
    void ld_mn16_SP();
    void ld_HL_SPe8();
    void ld_SP_HL();
    void pop(RegisterPair& r16);
    void push_AF();
    void push(RegisterPair& r16);


    

    void NOP();
    
};

//helper function for the CPU instruction classes
 inline u16 convertLittleEndian(u8 firstByte, u8 secondByte){
    return (firstByte | static_cast<u16>(secondByte) << 8);

}