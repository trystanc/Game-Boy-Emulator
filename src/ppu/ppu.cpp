#include "ppu.h"
#include <cassert>
#include "../gb/mediator.h"

//This will do for now but will probably want to add more asserts later when 
//testing rendering.

PPU::PPU(AddressBus& _addressBus)
    : renderer(_addressBus),
 LY(_addressBus.ioRegisters[constants::LYAddress - constants::ioRegistersStart]),
 STAT(_addressBus.ioRegisters[constants::LCDStatusAddress - constants::ioRegistersStart]),
 LYC( _addressBus.ioRegisters[constants::LYCAddress - constants::ioRegistersStart])

{
        LY = 0;
}

void PPU::setMediator(Mediator* _mediator){
    mediator = _mediator;
}



void PPU::runCycles(int cycles){
    for(int _; _ < cycles*4; ++_){
        tick();
    }
}

void PPU::updateMode(PPUmodes mode){
    PPUmode = mode;
    mediator->updatePPUMode(mode);
}

void PPU::checkForNewMode(){
    switch(PPUmode){
        case HBlank: //either goes to newline or vblank
            if ((cycleNumber % constants::cyclesPerLine) == 0){ //if  going on to new line
                updateMode(OAMScan);
                renderer.incrementY();
                renderer.resetX();
                LY += 1;
                if (LY < constants::screeny) renderer.getSprites();
            } 
//After drawing all scanlines, go into VBlank mode (overwrites previous PPUmode = 2 above)
            if (cycleNumber == constants::cyclesUntilVBlank) updateMode(VBlank);//if going to vblank 
            break;
        case VBlank:
            if (cycleNumber == constants::cyclesPerFrame) {
                updateMode(OAMScan);
                reset();
            }
                break;
        case OAMScan:
            if ((cycleNumber % constants::cyclesPerLine) ==  80) updateMode(DrawMode); 
                break;
        case DrawMode:
            if ((cycleNumber % constants::cyclesPerLine) ==  252) updateMode(HBlank); 
            break;
        default: assert(false && "Invalid PPU mode");
    }
}

void PPU::tick(){
    notifyLYequalsLYC();
    requestSTATInterrupts();
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

void PPU::requestSTATInterrupts(){
    bool lyConditionmet {((STAT >> 6 & 1) && (STAT>>1 & 1))}; //checks STAT register if LY = LYC and if the interrupt is enabled.
    bool modeConditionmet = ((STAT >> (PPUmode+3))); //checks the mode of the PPUmode status bit and 
    bool validConditionMet = lyConditionmet | modeConditionmet;
    if(!(statLine) && validConditionMet){
        statLine = true;
        mediator->requestInterrupt(Interrupt::STAT);
    }
    else if(!validConditionMet){
        statLine = false;
    }

}

void PPU::notifyLYequalsLYC(){
    mediator->updateLYequalsLYCbit(LY == LYC);
}




