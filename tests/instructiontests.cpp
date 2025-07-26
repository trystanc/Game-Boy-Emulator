#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "testinghelpers.h"


//It's unclear how the logs created by gameboy doctor are generated when an intterupt occcurs, so have to find other ways to test interrupts have been handled correctly
// that don't rely on using these logs - will probably just run the blaarg test rom directly when graphics are working.
TEST_CASE("Generate log files for Gameboy Doctor"){
    std::array<std::string, 11> fileNames = {
        "01-special.gb", "02-interrupts.gb", "03-op sp,hl.gb", "04-op r,imm.gb", 
        "05-op rp.gb", "06-ld r,r.gb", "07-jr,jp,call,ret,rst.gb", 
        "08-misc instrs.gb", "09-op r,r.gb", "10-bit ops.gb", "11-op a,(hl).gb"
    };
    constexpr std::array<double ,11> romLengths{1.3e6, 2e5, 1.1e6, 1.3e6, 1.8e6, 3e5, 6e5, 3e5, 5e6, 7e6, 7.5e6};
    
    for(size_t i{0}; i<fileNames.size(); ++i){
        if (i == 1) continue;
        std::ifstream file("./tests/gb-test-roms/cpu_instrs/individual/" + fileNames[i], std::ios::binary);
        CHECK(file);
        Cartridge cartridge(file);
        file.close();
        AddressBusDebugMode addressBus(cartridge);
        CPULogger cpu(addressBus);
        TimerHandler timerHandler(addressBus);
        PPUTest ppu(addressBus);
        [[maybe_unused]] Mediator mediator(cpu, cartridge, addressBus, timerHandler, ppu); 
        cpu.createLogFile("./tests/outputs/" + std::to_string(i+1) + ".txt");
        for(int j{0}; j< static_cast<int> (romLengths[i]); ++j){
            cpu.tick();
        }
        cpu.closeLogFile();
        std::cout<<"Test Rom "<<i+1<<" compiled and ran ok - but will need to check the logs with gameboy doctor to make sure instructions are handled correctly."<<std::endl;
    }
}




