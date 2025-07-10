#pragma once
#include <array>
#include "../types.h"
#include "../constants.h"
#include "../cartridge/cartridge.h"

class AddressBus{

public:
    u8 garbage{0}; // Sentinel object to prevent access or write to memory when forbidden - e.g. LY register.
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