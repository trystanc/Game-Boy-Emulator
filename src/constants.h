#pragma once
#include "types.h"
namespace constants{
//CPU related constants
    constexpr int masterHz {4194304};
    constexpr int systemHz {masterHz/4};
    constexpr int mCycleTimeNs = static_cast<int>(1e9/masterHz);
    constexpr int sCycleTimeNs = static_cast<int>(1e9/systemHz);
    constexpr int cyclesPerFrame{70224};
    constexpr long frameTimeMicroS{16667};


//memory sizes
    constexpr u16 vramSize {8192};
    constexpr u16 wramSize {8192}; 
    constexpr u16 oamSize {160};
    constexpr u16 ioRegistersSize {128};
    constexpr u16 hRamSize {128}; //also includes the interrupt enable register

//memory map constants
    constexpr u16 forbiddenStart {0xFEA0}; 
    constexpr u16 vramStart {0x8000};
    constexpr u16 externalRamStart {0xA000};
    constexpr u16 wramStart {0xC000};
    constexpr u16 oamStart {0xFE00};
    constexpr u16 ioRegistersStart {0xFF00};
    constexpr u16 hRamStart {0xFF80};
    constexpr u16 endOfMemory {0xFFFF};
//Interrupt related constants
    constexpr u16 interruptFlagAddress{0xFF0F};
    constexpr u16 interruptEnableAddress{0xFFFF};
    constexpr u16 VBlankInterruptAddress{0x0040};
    constexpr u16 STATInterruptAddress{0x0048};
    constexpr u16 TimerInterruptAddress{0x0050};
    constexpr u16 SerialInterruptAddress{0x0058};
    constexpr u16 JoypadInterruptAddress{0x0060};
    constexpr int interruptCycleLength{5};

//Timer related registers
    constexpr u16 DIVAddress{0xFF04};
    constexpr u16 TIMAAddress{0xFF05};
    constexpr u16 TMAAddress{0xFF06};
    constexpr u16 TACAddress{0xFF07};

//PPU related constants
    constexpr int screenx {160};
    constexpr int screeny {144};
    constexpr u16 LCDCAddress {0xFF40};
    constexpr u16 scyAddress {0xFF42};
    constexpr u16 scxAddress{0xFF43};
    constexpr u16 wxAddress{0xFF4B};
    constexpr u16 wyAddress{0xFF4A};
    constexpr u16 tileMap1Start{0x9800};
    constexpr u16 tileMap2Start{0x9C00};
    constexpr u16 BGPAddress {0xFF47};
    constexpr u16 OBP0Address {0xFF48};
    constexpr u16 maxSpritesPerLine {10}; //maximum number of sprites that can be drawn on a single line, this is a hardware limitation
    constexpr u16 maxSpritesPerFrame {40}; //maximum number of sprites that can be drawn on a single frame, this is a hardware limitation
    constexpr u8 maxRGBvalue {255}; //maximum value for RGB components, used for the frame buffer
    constexpr u16 signedAddressStart {0x8800}; //start of the signed address space for tile data, used for 8x16 sprites
    constexpr uint cyclesPerLine {456};
    constexpr uint cyclesUntilVBlank{65664};
    constexpr u16 LYAddress{0xFF44};
    constexpr u16 LYCAddress{0xFF45};
    constexpr u16 LCDStatusAddress{0xFF41};
}