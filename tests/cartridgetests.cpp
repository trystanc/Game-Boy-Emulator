#include "../src/cartridge/cartridge.h"
#include <fstream>
#include <iostream>
#include <catch2/catch_test_macros.hpp>



TEST_CASE("Cartridge Tests") {
    SECTION("Test Basic Cartridge (no bank switching)"){

    std::ifstream file("./tests/gb-test-roms/cpu_instrs/individual/01-special.gb", std::ios::binary);
    
    CHECK(file);
    Cartridge cartridge(file);
    file.close();
    //testing file is loaded in correctly
    REQUIRE(cartridge.rom(0x100) == 0x00);
    REQUIRE(cartridge.rom(0x101) == 0xc3);
    
    REQUIRE(cartridge.rom(0x4000));
    REQUIRE_NOTHROW(cartridge.rom(0x7fff));
    REQUIRE_THROWS(cartridge.rom(0x8000));

    cartridge.setBankNumber(1);
    REQUIRE_THROWS(cartridge.rom(0x4000));//should throw because bank 2 does not exist for this cartridge
    }   
}


