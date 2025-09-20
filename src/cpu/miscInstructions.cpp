#include "cpu.h"

void CPU::di() { ime = false; }

void CPU::ei() { imePending = true; }

using enum Interrupt;

void CPU::halt() {
  bool interruptRequested {false};
  for (size_t i{0}; i < size_t(numOfInterrupts); ++i){
    bool serviceInterrupt{
        ((mem[constants::interruptFlagAddress] >> i) & 1) &&
        ((mem[constants::interruptEnableAddress] >> i) & 1)};
    if (serviceInterrupt) interruptRequested= true;
  }
  if (interruptRequested){
    return;
  }
  pc -= 1;
}

void CPU::stop() {
  return; // no licencsed games apparently make use of this instruction.
}

void CPU::nop() {
  return; // literally no operation
}
