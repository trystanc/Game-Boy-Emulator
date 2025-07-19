#include "ppu.h"
#include <cassert>
//This will do for now but will probably want to add more asserts later when 
//testing rendering.

PPU::PPU(AddressBus& _addressBus)
    : renderer(_addressBus),
 LY(_addressBus.ioRegisters[constants::LYAddress - constants::ioRegistersStart])

{
        LY = 0;
}



void PPU::runCycles(int cycles){
    for(int _; _ < cycles; ++_){
        tick();
    }
}

void PPU::checkForNewMode(){
    switch(PPUmode){
        case HBlank:
            if ((cycleNumber % constants::cyclesPerLine) == 0){ //if  going on to new line
                PPUmode = OAMScan;
                renderer.incrementY();
                renderer.resetX();
                LY += 1;
                if (LY < constants::screeny) renderer.getSprites();
                break;
            
            } 
//After drawing all scanlines, go into VBlank mode (overwrites previous PPUmode = 2 above)
            if (cycleNumber == constants::cyclesUntilVBlank) PPUmode = 1; 
            break;
        case VBlank:
            if (cycleNumber == constants::cyclesPerFrame) {
                PPUmode = OAMScan;
                reset();
            }
                break;
        case OAMScan:
            if ((cycleNumber % constants::cyclesPerLine) ==  80) PPUmode = DrawMode; 
                break;
        case DrawMode:
            if ((cycleNumber % constants::cyclesPerLine) ==  252) PPUmode = HBlank; 
            break;
        default: assert(false && "Invalid PPU mode");
    }
}

void PPU::tick(){
    checkForNewMode();
    if (PPUmode == DrawMode && (cycleNumber % 456 >= 92)){
        renderer.drawPixel();
        renderer.incrementX();
    }
    ++cycleNumber;
    assert(renderer.getY() == LY && "LY does not equal the Y position in renderer.");
}


void PPU::reset(){
    LY = 0;
    renderer.resetX();
    renderer.resetY();
    renderer.getSprites();
    cycleNumber = 0;

}




