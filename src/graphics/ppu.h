#pragma once
#include "../types.h"
#include "../addressBus/addressBus.h"
#include "../constants.h"
#include "ppuRenderer.h"
#include <SFML/Graphics/Color.hpp>
#include <optional>
#include <span>
#include <array>
#include<utility>


//may need to change how LCDC.0 is handled.
class PPU{
public:
    PPU(AddressBus& _addressBus);
    const u8* getFrameBuffer(){
        return renderer.getFrameBuffer();
    }
    void runCycles(int cycles);
    int getMode();

private:
    // const u8& SCX, SCY;
    // const u8& WX;
    // const u8& WY; 
    // const u8& LCDC;
    PPURenderer renderer;
    u8& LY;
    int PPUmode {VBlank};
    uint cycleNumber {constants::cyclesPerFrame};


    enum PPUmodes{
        HBlank = 0,
        VBlank = 1,
        OAMScan = 2,
        DrawMode = 3

    };
//array index is the colour index as defined by the gameboy, so 0 is white, 1 is light grey, 2 is dark grey, 3 is black    

    void reset();
    void tick();
    void checkForNewMode();
};