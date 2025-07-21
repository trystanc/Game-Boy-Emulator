#pragma once
#include "types.h"
namespace constants{

    constexpr int screenx {160};
    constexpr int screeny {144};
    constexpr int masterHz {4194304};
    constexpr int systemHz {masterHz/4};
    
    constexpr u16 vramSize {8192};
    constexpr u16 wramSize {8192}; 
    constexpr u16 oamSize {160};
    constexpr u16 ioRegistersSize {128};
//note: also includes the interrupt enable register
    constexpr u16 hRamSize {128};
    
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



}