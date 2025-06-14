#include <catch2/catch_test_macros.hpp>
#include "../src/cpu/cpu.h"
#include "../src/cpu/RegisterPair.h"




TEST_CASE("Test Register Pair Class"){
    std::ifstream file("./tests/gb-test-roms/cpu_instrs/individual/01-special.gb", std::ios::binary);
    CHECK(file);
    Cartridge cartridge(file);
    file.close();
    AddressBus addressBus(cartridge);
    CPU cpu(addressBus);

    Register A, B, X, Y {};
    RegisterPair AB(A, B);
    RegisterPair XY(X, Y);
    A = 0x12u; 
    B = 0x34u;
    X = 0x56u;
    Y = 0x78u;
    SECTION("Test RegisterPair Update and conversion"){
        REQUIRE(AB == 0x1234u);
        REQUIRE(XY == 0x5678u);
        REQUIRE(AB.hi == 0x12u);
        REQUIRE(A == 0x12u);
        REQUIRE(AB.lo == 0x34u);
        REQUIRE(B == 0x34u);
        AB = XY;
        REQUIRE(A == 0x56u);
        REQUIRE(B == 0x78u);
        REQUIRE(AB.hi == X);
        REQUIRE(AB.lo == Y);
        REQUIRE(AB == XY);
    }

    SECTION("Test RegisterPair Arithmetic Operations"){
        REQUIRE(AB + XY == 0x68ACu);
        REQUIRE(XY + AB == 0x68ACu);
        REQUIRE(XY - AB == 0X4444u);
        REQUIRE(AB + static_cast<u16>(0x1000u) == 0x2234u);
        REQUIRE(static_cast<u16>(0x1000u) + AB == 0x2234u);
        
        REQUIRE(0x1235u == ++AB);
        REQUIRE(AB == 0x1235u);
        REQUIRE(0x1234u == --AB);
        REQUIRE(AB == 0x1234u);
    }

}

