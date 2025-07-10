#pragma once
#include "../constants.h"
#include "../types.h"
#include "../addressBus/addressBus.h"
#include <SFML/System.hpp>
#include <thread>
//logic for incrementing timers.
class Clock{
    AddressBus& addressBus;
    u8& DIV;
    u8& TIMA;
    u8& TMA;
    u8& TAC;
    int mCycleCount {0};

    int clockSelect(){
        return TAC & 0b11;
    }
    void requestTimerInterrupt(){
        addressBus[0xFFFF] |= (1 << Timer);
    }

public:
    Clock(AddressBus& _addressBus) : addressBus{_addressBus}, 
                                    DIV{_addressBus[constants::DIVaddress]},
                                    TIMA{_addressBus[constants::TIMAaddress]},
                                    TMA{_addressBus[constants::TMAaddress]},
                                    TAC{_addressBus[constants::TACaddress]}
                                     {}
 
    void updateTimers(int cycles){
        int newCount = cycles + mCycleCount;
    //integer division to check if a certain number of m-cycles has passed.
        if (newCount / 256 != mCycleCount /256) DIV += 1;
        
        switch(clockSelect()){
            case 0: if(newCount / 256 != mCycleCount / 256) TIMA += 1; break;
            case 1: if(newCount / 4 !=  mCycleCount / 4) TIMA += 1; break;
            case 2: if(newCount / 16 !=  mCycleCount / 16) TIMA += 1; break;
            case 3: if(newCount / 16 !=  mCycleCount / 16) TIMA += 1; break;
        }
        if (TIMA == 0) {
            TIMA = TMA;
            requestTimerInterrupt();
        }


    }
    void updateClock(int cycles){
        updateTimers(cycles);
        mCycleCount += cycles;
        
    }
    int getCycleCount(){
        return mCycleCount;
    }

//called after each frame
    void reset(){
        mCycleCount %= 256;
    }
};