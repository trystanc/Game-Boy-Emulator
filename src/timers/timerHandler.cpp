#include "timerHandler.h"
#include "../types.h"

TimerHandler::TimerHandler(AddressBus& addressBus):
    DIV (addressBus.ioRegisters[constants::DIVAddress - constants::ioRegistersStart]),
    TIMA (addressBus.ioRegisters[constants::TIMAAddress - constants::ioRegistersStart]),
    TMA (addressBus.ioRegisters[constants::TMAAddress - constants::ioRegistersStart]),
    TAC (addressBus.ioRegisters[constants::TACAddress - constants::ioRegistersStart])
    {}

void TimerHandler::updateTimers(uint cycles){
    for(uint _ {0}; _ < cycles; ++_){
        advance();
    }
}

void TimerHandler::advance(){
    ++cycleCount;
    if (cycleCount % 64 == 0){
        ++DIV;
    }
    if (TIMAEnabled()){
    handleTIMA();
    }
}

bool TimerHandler::TIMAEnabled(){
    return (TAC >> 2) & 0b1; //bit 2 controls TAC.
}

static constexpr std::array<int, 4> timerModes{256, 4, 16, 64};

void TimerHandler::handleTIMA(){
    u8 timerMode = TAC & 0b11;
    if ((cycleCount % timerModes[timerMode]) == 0){
        ++TIMA;
    }
    if (TIMA == 0){
        TIMA =  TMA;
        mediator->requestInterrupt(Interrupt::Timer);
    }


}

void TimerHandler::setMediator(Mediator* _mediator){
    mediator = _mediator;
}