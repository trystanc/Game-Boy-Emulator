#pragma once
#include <array>
#include "../types.h"
#include "../constants.h"
#include "../cartridge/cartridge.h"

class AddressBus{
    struct Proxy{
        AddressBus& mem;
        u16 address;
        Proxy(AddressBus& _mem, u16 _address) : mem{_mem}, address{_address} {}
        operator u8(){return mem.read(address);}
        void operator=(u8 value) { mem.write(address, value); }
    };

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

    Proxy operator[](u16 address){
        return Proxy(*this, address);
    }
    virtual u8 read(u16 address);
    virtual void write(u16 address, u8 value);

        
};