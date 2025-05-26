#pragma once
#include <array>
#include "../constants.h"
#include "RegisterPair.h"
#include "../types.h"
//value stores the 16 bit integers of the
//8 bit pairs.
using Register = u8;

class CPU{

    enum flagPosition{
        ZPos = 7,
        NPos = 6,
        HPos = 5,
        CPos = 4,
    };
    std::array<u8, constants::ramSize>& m_ram;
    Register A,F,B,C,D,E,H,L;
    RegisterPair AF;
    RegisterPair BC;
    RegisterPair DE;
    RegisterPair HL;
    u16 pc;
    u16 sp;

    bool getFlag(flagPosition pos) const {
        return (F >> pos) & 0x1;
    }
    void setFlag(flagPosition pos, bool val){
        auto value = static_cast<u8>(val);
        F = (F & ~(1 << pos)) | (value << pos);
    }
    bool Zflag() const {return getFlag(ZPos);}
    bool Nflag() const {return getFlag(NPos);}
    bool Hflag() const {return getFlag(HPos);}
    bool Cflag() const {return getFlag(CPos);}

    void setZ(bool val) { setFlag(ZPos, val); }
    void setN(bool val) { setFlag(NPos, val); }
    void setH(bool val) { setFlag(HPos, val); }
    void setC(bool val) { setFlag(CPos, val); }
    friend class RegisterPair;
    friend class Debugger;

public:
   CPU(std::array<u8, constants::ramSize>& ram)
        : m_ram(ram),
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

    void executeInstruction();



    
    
    
    u16 getN16();
    void storeN16(u16 addr, u16 val);
    u8 getn8();
    void NOP();
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
};

