#include <catch2/catch_test_macros.hpp>
#include "../src/cpu/cpu.h"
#include <fstream>
#include <string>
#include <iomanip>
#include "../src/addressBus/addressBus.h"
#include "../src/cartridge/cartridge.h"
using namespace std::string_view_literals;
//Generates log files to passed to gameboy doctor.

class CPULogger: public CPU{
private:
    std::ofstream logFile;
public:
    void createLogFile(const std::string logFileName){
        logFile.open(logFileName);
        logState();
    }
    CPULogger(AddressBus& bus) : CPU(bus) {}
    ~CPULogger(){
        if (logFile.is_open()) logFile.close();
    }
    void logState(){
        logFile << std::uppercase
                <<"A:"<<std::hex << std::setw(2) << std::setfill('0') << +A << " "
                << "F:"<<std::hex << std::setw(2) << std::setfill('0') << +F << " "
                << "B:"<<std::hex << std::setw(2) << std::setfill('0') << +B << " "
                << "C:"<<std::hex << std::setw(2) << std::setfill('0') << +C << " "
                << "D:"<<std::hex << std::setw(2) << std::setfill('0') << +D << " "    
                << "E:"<<std::hex << std::setw(2) << std::setfill('0') << +E << " "            
                << "H:"<<std::hex << std::setw(2) << std::setfill('0') << +H << " "  
                << "L:"<<std::hex << std::setw(2) << std::setfill('0') << +L << " "  
                << "SP:"<<std::hex << std::setw(4) << std::setfill('0') << sp << " "      
                << "PC:"<<std::hex << std::setw(4) << std::setfill('0') << pc << " " 
                << "PCMEM:"<<std::hex << std::setw(2) << std::setfill('0') << +mem[pc] << "," 
                           <<std::hex << std::setw(2) << std::setfill('0') << +mem[pc+1] << "," 
                           <<std::hex << std::setw(2) << std::setfill('0') << +mem[pc+2] << "," 
                           <<std::hex << std::setw(2) << std::setfill('0') << +mem[pc+3]
                           <<std::endl;

    }
};

//hardcodes address bus to return 0x90 at address 0xff44
class AddressBusDebugMode : public AddressBus{
private: 
    u8 fixedValue {0x90};
public:
    AddressBusDebugMode(Cartridge& cart) : AddressBus(cart) {}
    u8& operator[](u16 address) override{
        if (address == 0xff44){
            return fixedValue;
        }
        else{
            return AddressBus::operator[](address);
        }
    }

};

TEST_CASE("Generate log files for Gameboy Doctor - rom 1"){
    std::ifstream file("./tests/gb-test-roms/cpu_instrs/individual/01-special.gb", std::ios::binary);
    
    CHECK(file);
    Cartridge cartridge(file);
    file.close();
    AddressBusDebugMode addressBus(cartridge);
    CPULogger cpu(addressBus);
    cpu.createLogFile("./tests/outputs/01.txt");
    for(int i{0}; i<2e6; ++i){
        cpu.executeInstruction();
        cpu.logState();
    }



}


