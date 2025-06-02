#pragma once

#include "../types.h"
#include <fstream>
#include <vector>


//This will need to be reewritten to support roms with external ram in the future
class Cartridge{
    std::vector<u8> cartridgeData;

    std::vector<u8> cartridgeRam{std::vector<u8>(8192)}; //unused for now, but reserved for ROMs with external RAM
//switchable bank number n-1 so when 0 switchable bank 01 is selected.
    u8 bankNumber {0};
public:
    u16 romBankOffSet() {
        return ((bankNumber) * 0x4000);
    }
    void setBankNumber(u8 bank) {
        bankNumber = bank;

    }
    u8& rom(u16 address) {
        return (address < 0x4000) ? cartridgeData.at(address) : cartridgeData.at(romBankOffSet() + address);
    }

    Cartridge(std::ifstream &file);
    
    u8& ram(u16 address) {
        return cartridgeRam.at(address);
    }

};