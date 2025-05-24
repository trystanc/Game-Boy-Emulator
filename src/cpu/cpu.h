#pragma once
#include <array>
#include "constants.h"
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
    Register A,F,B,C,D,E,H,L {};
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
        F = (F & ~(1 << pos)) | (val << pos);
    }
    friend class RegisterPair;

public:
   CPU(std::array<u8, constants::ramSize>& ram)
        : m_ram(ram),
          AF(A, F),
          BC(B, C),
          DE(D, E),
          HL(H, L) {}

    bool Z() const {return getFlag(ZPos);}
    bool N() const {return getFlag(NPos);}
    bool H() const {return getFlag(HPos);}
    bool C() const {return getFlag(CPos);}

    void setZ(bool val) { setFlag(ZPos, val); }
    void setN(bool val) { setFlag(NPos, val); }
    void setH(bool val) { setFlag(HPos, val); }
    void setC(bool val) { setFlag(CPos, val); }

    void executeInstruction();
};



