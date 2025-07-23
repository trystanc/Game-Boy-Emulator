#include <catch2/catch_test_macros.hpp>

#include "testinghelpers.h"

//hardcodes address bus to return 0x90 at address 0xff44


TEST_CASE("Generate log files for Gameboy Doctor - rom 1"){
    std::ifstream file("./tests/gb-test-roms/cpu_instrs/individual/01-special.gb", std::ios::binary);
    
    CHECK(file);
    Cartridge cartridge(file);
    file.close();
    AddressBusDebugMode addressBus(cartridge);
    CPULogger cpu(addressBus);
    cpu.createLogFile("./tests/outputs/01.txt");
    for(int i{0}; i<2e6; ++i){
        cpu.tick();
        cpu.logState();
    }
}


