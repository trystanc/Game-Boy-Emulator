#include "addressBus.h"
#include <cassert>
#include <iostream>
#include <stdexcept>


u8 AddressBus::read(u16 address) {

    u16 highNibble = address >> 12;
    switch (highNibble) {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
            return cartridge.rom(address);
        case 0x8:
        case 0x9:
            return vram[address - constants::vramStart];
        case 0xA:
        case 0xB:
            return cartridge.ram(address - constants::externalRamStart);
        case 0xC:
        case 0xD:
            return wram[address - constants::wramStart];
        case 0xE:
              throw std::out_of_range("Access to forbidden address attempted.");
              return hRam[0]; // This line will never be reached, but is needed to avoid compiler warnings.            
        case 0xF:


            if (address >= constants::hRamStart) {
                return hRam[address - constants::hRamStart];
            }
            else if (address >= constants::ioRegistersStart) {

                return ioRegisters[address - constants::ioRegistersStart];
            }
            else if (address >= constants::oamStart && address < constants::forbiddenStart) {
                return oam[address - constants::oamStart];
            } 
            else {
              throw std::out_of_range("Access to forbidden address attempted.");
              return hRam[0]; // This line will never be reached, but is needed to avoid compiler warnings.
            }
        default:
            throw std::out_of_range("Address out of range.");
            return hRam[0]; // This line will never be reached, but is needed to avoid compiler warnings.

    }

}

void AddressBus::write(u16 address, u8 value) {

    u16 highNibble = address >> 12;
    switch (highNibble) {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
            vram[address - constants::vramStart] = value; return;
        case 0xA:
        case 0xB:
            cartridge.ram(address - constants::externalRamStart) = value; return;
        case 0xC:
        case 0xD:
            wram[address - constants::wramStart] = value; return;
        case 0xE:
              throw std::out_of_range("Access to forbidden address attempted.");
              return;            
        case 0xF:
            if (address >= constants::hRamStart) {
                hRam[address - constants::hRamStart] = value; return;
            }
            else if (address >= constants::ioRegistersStart) {
                ioRegisters[address - constants::ioRegistersStart] = value; return;
            } 
            else if ( address >= constants::oamStart && address < constants::forbiddenStart) {
                oam[address - constants::oamStart] = value; return;
            } 
            else {
              throw std::out_of_range("Access to forbidden address attempted.");
              return; 
            }
        default:
            throw std::out_of_range("Address out of range.");
            return; 

    }

}

void AddressBus::requestInterrupt(Interrupt interrupt){
    ioRegisters[constants::interruptFlagAddress - constants::ioRegistersStart] |= (0b1 << (static_cast<int>(interrupt))); 
}

void AddressBus::setMediator(Mediator* _mediator){
    mediator = _mediator;
}


