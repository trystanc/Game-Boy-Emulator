#include <catch2/catch_test_macros.hpp>
#include "../src/addressBus/addressBus.h"
#include "../src/cartridge/cartridge.h"

TEST_CASE("Address Bus Tests"){
    SECTION("Test Address Bus functionality."){

    std::ifstream file("./tests/gb-test-roms/cpu_instrs/individual/01-special.gb", std::ios::binary);
    CHECK(file);
    Cartridge cartridge(file);
    file.close();
    AddressBus addressBus(cartridge);

    u16 lhs, rhs;

    // testing rom is accessed correctly 
    lhs = 0x100u; rhs = 0x100u;
    REQUIRE(&(addressBus[lhs]) == &cartridge.rom(rhs));
    lhs = 0x4000u; rhs = 0x4000u;
    REQUIRE(&(addressBus[lhs]) == &cartridge.rom(rhs));
    lhs = 0x7fffu; rhs = 0x7fffu;
    REQUIRE(&(addressBus[lhs]) == &cartridge.rom(rhs));
    // testing vram is accessed correctly
    lhs = 0x8000u; rhs = 0u;
    REQUIRE(&addressBus[lhs] == &addressBus.vram[rhs]);
    lhs = 0x9fffu; rhs = 0x1fffu;
    REQUIRE(&addressBus[lhs] == &addressBus.vram[rhs]);

    // testing external ram is accessed correctly
    lhs = 0xA000u; rhs = 0x0u;
    REQUIRE(&addressBus[lhs] == &cartridge.ram(rhs));
    lhs = 0xBFFFu; rhs = 0x1FFFu;
    REQUIRE(&addressBus[lhs] == &cartridge.ram(rhs));

    // testing wram is accessed correctly
    lhs = 0xC000u; rhs = 0u;
    REQUIRE(&addressBus[lhs] == &addressBus.wram[rhs]);
    lhs = 0xDFFFu; rhs = 0x1FFFu;
    REQUIRE(&addressBus[lhs] == &addressBus.wram[rhs]);

    // testing oam is accessed correctly
    lhs = 0xFE00u; rhs = 0u;
    REQUIRE(&addressBus[lhs] == &addressBus.oam[rhs]);
    lhs = 0xFE9Fu; rhs = 0x9Fu;
    REQUIRE(&addressBus[lhs] == &addressBus.oam[rhs]);

    // testing io registers are accessed correctly
    lhs = 0xFF00u; rhs = 0u;
    REQUIRE(&addressBus[lhs] == &addressBus.ioRegisters[rhs]);
    lhs = 0xFFFFu; rhs = 0xFFu;
    REQUIRE(&addressBus[lhs] == &addressBus.ioRegisters[rhs]);

    // testing hram and interrupt is accessed correctly
    lhs = 0xFF80u; rhs = 0u;
    REQUIRE(&addressBus[lhs] == &addressBus.hRam[rhs]);
    lhs = 0xFFFFu; rhs = 0x7fu;
    REQUIRE(&addressBus[lhs] == &addressBus.hRam[rhs]);


    // echo ram use is prohibited
    lhs = 0xE000u;
    REQUIRE_THROWS(addressBus[lhs]);
    lhs = 0xFDFFu;
    REQUIRE_THROWS(addressBus[lhs]); 
    //use of addresses in the range 0xFEA0 to 0xFEFF is prohibited
    lhs = 0xFEA0u;
    REQUIRE_THROWS(addressBus[lhs]);
    lhs = 0xFEFFu;
    REQUIRE_THROWS(addressBus[lhs]);
}
}
