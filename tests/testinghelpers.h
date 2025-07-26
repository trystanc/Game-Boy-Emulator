#include "../src/addressBus/addressBus.h"
#include "../src/cpu/cpu.h"
#include <fstream>
#include <string>
#include <iomanip>
#include "../src/addressBus/addressBus.h"
#include "../src/cartridge/cartridge.h"
#include <stdexcept>
#include "../src/gb/mediator.h"
#include "../src/ppu/ppu.h"
#include "../src/timers/timerHandler.h"

inline bool enable_debug{false}; //constant that maybe useful for setting breakponts.


class AddressBusDebugMode : public AddressBus{
private: 
    u8 fixedValue {0x90};
public:
    AddressBusDebugMode(Cartridge& cart) : AddressBus(cart) {}
    u8 read(u16 address) override {
        if (address == 0xff44) {
            return fixedValue;
        } else {
            return AddressBus::read(address);
        }
    }
    void write(u16 address, u8 value) override {

            AddressBus::write(address, value);

    }

};

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
    void tick() override{
    bool interruptOccured = handleInterrupts();
    if(interruptOccured){
        mediator->advanceState(constants::interruptCycleLength);
        return;
    }
    uint cycles = executeInstruction();
    logState();
    mediator->advanceState(cycles);
    return;
    }
    void closeLogFile(){
        if (logFile.is_open()) logFile.close();
    }
};

class PPUTest: public PPU{

    void notifyLYequalsLYC() override{
    mediator->updateLYequalsLYCbit(LYC == 0x90);
    }
public:
    PPUTest(AddressBusDebugMode& bus) : PPU(bus){}

};







inline Cartridge makeTestCartridge(){
    std::ifstream file("./tests/gb-test-roms/cpu_instrs/individual/01-special.gb", std::ios::binary);
    if(!file){
        throw std::runtime_error("File not found");
    }
    Cartridge cartridge(file);
    file.close();
    return cartridge;
}

inline AddressBusDebugMode makeTestBus(Cartridge& cartridge){
    AddressBusDebugMode addressBus(cartridge);
    return addressBus;
}


