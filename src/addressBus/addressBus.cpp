#include "addressBus.h"
#include "SFML/Window/Keyboard.hpp"
#include <cassert>
#include <stdexcept>
#include <iostream>
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
              return hRam[0]; // Access to forbidden address attempted, return arbitrary value.           
        case 0xF:


            if (address >= constants::hRamStart) {
                return hRam[address - constants::hRamStart];
            }
            else if (address >= constants::ioRegistersStart) {
                if (address == constants::ioRegistersStart) return 0xff;
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
            return;
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
              return;            
        case 0xF:
            if (address >= constants::hRamStart) {
                hRam[address - constants::hRamStart] = value; return;
            }
            
            else if (address >= constants::ioRegistersStart) {
                if (address == constants::ioRegistersStart) writeJPRegister(value);
                if (address == constants::DMARegisterAddress){
                    DMATransfer(value);
                    ioRegisters[address - constants::ioRegistersStart] = value;
                    return;  
                }

                if (address == constants::DIVAddress){ 
                    ioRegisters[address - constants::ioRegistersStart] = 0; 
                    return;}
                
                ioRegisters[address - constants::ioRegistersStart] = value; return;
            } 
            
            else if ( address >= constants::oamStart && address < constants::forbiddenStart) {
                oam[address - constants::oamStart] = value; return;
            } 
            
            else {
              return; // Forbidden address,  do nothing.
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

void AddressBus::DMATransfer(u8 value){
    u16 startAddress = value << 8; //value specifies the upper byte for the address.

    for(u16 i {0}; i < constants::oamSize; ++i){
        write(constants::oamStart + i, read(startAddress + i)); 
    }
}

namespace input{
        enum buttonIndicies{
        a,
        b,
        select,
        start,
        right,
        left,
        up,
        down,
        numberOfButtons
    };
}

u8 unsetBit(u8 val, u8 bitPos){
    return val & static_cast<u8>((~(1<<bitPos)));
}

u8 setBit(u8 val, u8 bitPos){
    return val | static_cast<u8>((1<<bitPos));
};


//Some kind of hashmap is probably cleaner here, but switch statement is more performant.
using SCode = sf::Keyboard::Scan;
void AddressBus::updateButton(SCode code, bool released){
    switch(static_cast<int>(code)){
        case(static_cast<int>(SCode::X)):
            updateButtonState(input::a, released);
            break;
        case(static_cast<int>(SCode::Z)):
            updateButtonState(input::b, released);
            break;
        case(static_cast<int>(SCode::Enter)):
            updateButtonState(input::start, released);
            break;
        case(static_cast<int>(SCode::RShift)):
            updateButtonState(input::select, released);
            break;
        case(static_cast<int>(SCode::Up)):
            updateButtonState(input::up, released);
            break;
        case(static_cast<int>(SCode::Down)):
            updateButtonState(input::down, released);
            break;
        case(static_cast<int>(SCode::Left)):
            updateButtonState(input::left, released);
            break;
        case(static_cast<int>(SCode::Right)):
            updateButtonState(input::right, released);
            break;
    }       
}


void AddressBus::updateButtonState(u8 button, bool released){
        const u8 beforeState {buttonStates};
        buttonStates = released ? setBit(beforeState, button): 
                                unsetBit(beforeState, button);
        if(!released){
            handlePotentialJPInterrupt(beforeState, buttonStates);
        }

}
void AddressBus::handlePotentialJPInterrupt(const u8 beforeState, const u8 afterState){
    const u8 JPRegister {ioRegisters[constants::ioRegistersStart]};
    if ((JPRegister >> 5) & 0b1) {
        if ((0xf & beforeState) > (0xf & afterState)) requestInterrupt(Interrupt::Joypad);
    }
    if ((JPRegister >> 4) & 0b1) {
        if ((0xf0 & beforeState) > (0xf0 & afterState)) requestInterrupt(Interrupt::Joypad);
    } 
}

void AddressBus::writeJPRegister(u8 value){
    const u8 beforeState(ioRegisters[constants::ioRegistersStart]);
    ioRegisters[constants::ioRegistersStart] = value & ~0xf;
    ioRegisters[constants::ioRegistersStart] += (beforeState & 0x0f);   
}

u8 AddressBus::readJPRegister(){
    const u8 JPRegister {ioRegisters[constants::ioRegistersStart]};
        if ((JPRegister >> 5) & 0b1) return (JPRegister | (0xf & buttonStates));
        if ((JPRegister >> 4) & 0b1) return (JPRegister | (0xf0 & buttonStates)); 
        return 0xff;       
    
}