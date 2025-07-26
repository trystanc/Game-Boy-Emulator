#include <catch2/catch_test_macros.hpp>
#include "../src/addressBus/addressBus.h"
#include "../src/cartridge/cartridge.h"
#include "../src/types.h"

TEST_CASE("Address Bus Tests"){
    
    std::ifstream file("./tests/gb-test-roms/cpu_instrs/individual/01-special.gb", std::ios::binary);
    CHECK(file);
    Cartridge cartridge(file);
    file.close();
    AddressBus addressBus(cartridge);
    
    SECTION("Test Addressing"){
    u16 lhs, rhs;

    // testing rom is accessed correctly 
    lhs = 0x100u; rhs = 0x100u;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == 0x00);
    lhs = 0x101u; rhs = 0x100u;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == 0xc3);
    lhs = 0x4000u; rhs = 0x4000u;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == cartridge.rom(rhs));
    lhs = 0x7fffu; rhs = 0x7fffu;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == cartridge.rom(rhs));
    // testing vram is accessed correctly
    lhs = 0x8000u; rhs = 0u;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == addressBus.vram[rhs]);
    lhs = 0x9fffu; rhs = 0x1fffu;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == addressBus.vram[rhs]);

    // testing external ram is accessed correctly
    lhs = 0xA000u; rhs = 0x0u;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == cartridge.ram(rhs));
    lhs = 0xBFFFu; rhs = 0x1FFFu;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == cartridge.ram(rhs));

    // testing wram is accessed correctly
    lhs = 0xC000u; rhs = 0u;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == addressBus.wram[rhs]);
    lhs = 0xDFFFu; rhs = 0x1FFFu;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == addressBus.wram[rhs]);

    // testing oam is accessed correctly
    lhs = 0xFE00u; rhs = 0u;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == addressBus.oam[rhs]);
    lhs = 0xFE9Fu; rhs = 0x9Fu;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == addressBus.oam[rhs]);

    // testing io registers are accessed correctly
    lhs = 0xFF00u; rhs = 0u;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == addressBus.ioRegisters[rhs]);
    lhs = 0xFFFFu; rhs = 0xFFu;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == addressBus.ioRegisters[rhs]);

    // testing hram and interrupt is accessed correctly
    lhs = 0xFF80u; rhs = 0u;
    REQUIRE(static_cast<u8>(addressBus[lhs]) ==  addressBus.hRam[rhs]);
    lhs = 0xFFFFu; rhs = 0x7fu;
    REQUIRE(static_cast<u8>(addressBus[lhs]) == addressBus.hRam[rhs]);


    // echo ram use is prohibited
    lhs = 0xE000u;
    REQUIRE_THROWS(static_cast<u8>(addressBus[lhs]));
    lhs = 0xFDFFu;
    REQUIRE_THROWS(static_cast<u8>(addressBus[lhs]));
    //use of addresses in the range 0xFEA0 to 0xFEFF is prohibited
    lhs = 0xFEA0u;
    REQUIRE_THROWS(static_cast<u8>(addressBus[lhs]));
    lhs = 0xFEFFu;
    REQUIRE_THROWS(static_cast<u8>(addressBus[lhs]));
}
    SECTION("Test Interrupt Handling"){
        u8& IF = addressBus.ioRegisters[constants::interruptFlagAddress - constants::ioRegistersStart];
        IF = 0;
        for(int i {0}; i < u8(Interrupt::numOfInterrupts); ++i){
            SECTION("Individual Interrupt"){
                addressBus.requestInterrupt(static_cast<Interrupt>(i));
                REQUIRE( (IF - (1<<i)) == 0);
            }
        }
    }
    
    SECTION("Writing to DIV sets it to 0"){
        u8& DIV = addressBus.ioRegisters[constants::DIVAddress - constants::ioRegistersStart];
        DIV = 0x50;
        addressBus[constants::DIVAddress] = 0x13;
        REQUIRE(DIV == 0);
    }

}
