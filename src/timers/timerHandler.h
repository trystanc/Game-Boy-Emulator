#pragma once
#include "../types.h"
#include "../addressBus/addressBus.h"
#include "../gb/mediator.h"
class TimerHandler{
public:
    TimerHandler(AddressBus& addressBus);
    void updateTimers(uint cycles);
    void setMediator(Mediator* _mediator);
    void runCycles(uint cycles);
private:
    u8 cycleCount {0};
    u8& DIV;
    u8& TIMA;
    u8& TMA;
    u8& TAC;
    Mediator* mediator;
    void advance();
    bool TIMAEnabled();
    void handleTIMA();

};