#include "emulator/emulator.h"
#include <array>
#include <string>

std::array<std::string, 11> fileNames = {
    "01-special.gb", "02-interrupts.gb", "03-op sp,hl.gb", "04-op r,imm.gb", 
    "05-op rp.gb", "06-ld r,r.gb", "07-jr,jp,call,ret,rst.gb", 
    "08-misc instrs.gb", "09-op r,r.gb", "10-bit ops.gb", "11-op a,(hl).gb"
};
int main(){
    std::ifstream file("./tests/gb-test-roms/" + std::string{"tetris.gb"}, std::ios::binary);
    if (!file){
        std::cerr<< "File not found."<<std::endl;
        return -1;
    }
    Emulator emulator(file);
    file.close();
    emulator.run();

 
    return 0;
}
