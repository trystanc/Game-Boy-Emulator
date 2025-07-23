#include <catch2/catch_test_macros.hpp>
#include "../src/gb/mediator.h"
#include "../src/types.h"
#include "../src/constants.h"
#include "../src/addressBus/addressBus.h"
#include "../src/timers/timerHandler.h"
#include "testinghelpers.h"


void setBitPos(u8& val, u8 bitPos ){
   val |= (1<<bitPos); 
}

void resetBitPos(u8& val, u8 bitPos){
  val &= u8(~(1<<bitPos));
}

TEST_CASE("Timers Increment correctly"){
    
    Cartridge cartridge {makeTestCartridge()};
    AddressBusDebugMode bus{cartridge};
    CPULogger cpu{bus};
    TimerHandler timer{bus};
    [[maybe_unused]] Mediator mediator{cpu, cartridge, bus, timer};
    
    u8& DIV = bus.ioRegisters[constants::DIVAddress - constants::ioRegistersStart];   
    u8& TIMA = bus.ioRegisters[constants::TIMAAddress - constants::ioRegistersStart];
    u8& TMA = bus.ioRegisters[constants::TMAAddress - constants::ioRegistersStart];
    u8& TAC = bus.ioRegisters[constants::TACAddress - constants::ioRegistersStart];
    u8& IF  = bus.ioRegisters[constants::interruptFlagAddress - constants::ioRegistersStart];    
    
    SECTION("DIV increments correctly (increments by 1 every 64 cycles)"){
    timer.updateTimers(63u);
    REQUIRE(DIV == 0);
    timer.updateTimers(1u);
    REQUIRE(DIV == 1);
    timer.updateTimers(128u);
    REQUIRE(DIV == 3);
    timer.updateTimers(64u);
    REQUIRE(DIV == 4);
    timer.updateTimers(128u);
    REQUIRE(DIV == 6);
    }

    SECTION("TIMA increments correctly"){

      SECTION("TIMA does not increment when disabled"){
        TIMA = 0;
        resetBitPos(TAC, 2);
        timer.updateTimers(256);
        REQUIRE(TIMA == 0);
      }
      SECTION("TIMA increments at the correct rates"){
        setBitPos(TAC,2);//ENABLES TIMA
        
        SECTION("Mode 0 - increment every256 cycles"){
        timer.updateTimers(256);
        REQUIRE(TIMA == 1);
        timer.updateTimers(256);
        REQUIRE(TIMA == 2);
        }
        SECTION("Mode 1 - increment every 4 cycles"){
        setBitPos(TAC, 0);
        timer.updateTimers(4);
        REQUIRE(TIMA == 1);
        timer.updateTimers(4);
        REQUIRE(TIMA == 2);
        }
        SECTION("Mode 2 - increment every 16 cycles"){
        TIMA = 0;
        setBitPos(TAC, 1);
        resetBitPos(TAC, 0);
        timer.updateTimers(16);
        REQUIRE(TIMA == 1);
        timer.updateTimers(16);
        REQUIRE(TIMA == 2);
        }
        SECTION("Mode 3 - increment every 64 cycles"){
        setBitPos(TAC, 1);
        setBitPos(TAC, 1);
        timer.updateTimers(16);
        REQUIRE(TIMA == 1);
        timer.updateTimers(16);
        REQUIRE(TIMA == 2);
        }
      }

      SECTION("TIMA overflow behaves correctly"){
        TIMA = 255;
        TMA = 13;
        resetBitPos(IF, 2);
        REQUIRE((IF >>2 & 0b1) == 0);        
        setBitPos(TAC, 2);
        timer.updateTimers(256);
        REQUIRE(TIMA == 13);
        REQUIRE((IF >>2 & 0b1) == 1);
      }


    }

}
