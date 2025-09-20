#pragma once
#include "../types.h"
#include "../addressBus/addressBus.h"
#include "../constants.h"
#include "ppuRenderer.h"
#include <optional>
#include <span>
#include <array>
#include<utility>


//may need to change how LCDC.0 is handled.
class PPU{
public:
    PPU(AddressBus& _addressBus);
    const u8* getFrameBuffer();
    void runCycles(int cycles);
    int getMode();
    void setMediator(Mediator* mediator);

private:
    enum PPUmodes : u8{
        HBlank = 0,
        VBlank = 1,
        OAMScan = 2,
        DrawMode = 3

    };
    PPURenderer renderer;
    u8& LY;
    PPUmodes PPUmode {VBlank};
    uint cycleNumber {constants::cyclesPerFrame};
    const u8& STAT;
    bool statLine{false};



//array index is the colour index as defined by the gameboy, so 0 is white, 1 is light grey, 2 is dark grey, 3 is black    
    void reset();
    void tick();
    void checkForNewMode();
    void updateMode(PPUmodes mode);
    void requestSTATInterrupts();
    void requestVBlankInterrupt();
protected:    
    u8& LYC;
    Mediator* mediator;
    virtual void notifyLYequalsLYC();

};