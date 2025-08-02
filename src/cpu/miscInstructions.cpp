#include "cpu.h"

void CPU::di() { ime = false; }

void CPU::ei() { imePending = true; }

void CPU::halt() {
  // placeholder for halt instruction
}

void CPU::stop() {
  return; // no licencsed games apparently make use of this instruction.
}

void CPU::nop() {
  return; // literally no operation
}
