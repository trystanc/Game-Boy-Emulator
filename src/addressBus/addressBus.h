#pragma once
#include <array>
#include "../types.h"
#include "../constants.h"
#include "../cartridge/cartridge.h"

class AddressBus{

public:
    Cartridge& cartridge;
    Array<constants::vramSize> vram;
    Array<constants::wramSize> wram;
    Array<constants::oamSize> oam;
    Array<constants::ioRegistersSize> ioRegisters;
    Array<constants::hRamSize> hRam;
    AddressBus(Cartridge& cart)
        : cartridge(cart), vram{}, wram{}, oam{}, ioRegisters{}, hRam{} {}
    virtual u8& operator[](u16 address);


        
};