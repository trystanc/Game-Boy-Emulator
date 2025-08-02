#pragma once
#include <array>
#include "../types.h"
#include "../constants.h"
#include "../cartridge/cartridge.h"
#include "../gb/mediator.h"

class AddressBus{
    struct Proxy{
        AddressBus& mem;
        u16 address;
        Proxy(AddressBus& _mem, u16 _address) : mem{_mem}, address{_address} {}
        operator u8(){return mem.read(address);}
        void operator=(u8 value) { mem.write(address, value); }
    };

public:
    Mediator* mediator;
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
    void setMediator(Mediator* _mediator);
    void requestInterrupt(Interrupt interrupt);
    void DMATransfer(u8 address);

        
};