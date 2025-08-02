#include "../types.h"
#include "cpu.h"

void CPU::bit_u3_r8(u8 bitPos, u8 value) {
  bool zero = (value >> bitPos) & 0x1;
  setFlags(!zero, 0, 1, Cflag());
}

void CPU::bit_u3_mHL(u8 bitPos) { bit_u3_r8(bitPos, mem[HL]); }

void CPU::res_u3_r8(u8 bitPos, Register &value) {
  value &= static_cast<u8>(~(1 << bitPos));
}

void CPU::res_u3_mHL(u8 bitPos) {
  mem[HL] = static_cast<u8>(~(1 << bitPos) & mem[HL]);
}

void CPU::rl_r8(u8 &value) {
  bool carry = (value >> 7) & 0x1;
  bool lowerBit = Cflag();
  value = value << 1 | lowerBit;
  setFlags(value == 0, false, false, carry);
}

void CPU::rl_mHL() {
  u8 value = mem[HL];
  bool carry = (value >> 7) & 0x1;
  bool lowerBit = Cflag();
  u8 result = value << 1 | lowerBit;
  setFlags(result == 0, false, false, carry);
  mem[HL] = result;
}

void CPU::rl_A() { rl_r8(A); }

void CPU::rla() {
  rl_r8(A);
  setFlags(false, false, false, Cflag());
}

void CPU::rlc_r8(u8 &value) {
  bool carry = (value >> 7) & 0x1;
  value *= 2;
  value |= carry;
  setFlags(value == 0, false, false, carry);
}

void CPU::rlc_mHL() {
  u8 value = mem[HL];
  bool carry = (value >> 7) & 0x1;
  value *= 2;
  value |= carry;
  setFlags(value == 0, false, false, carry);
  mem[HL] = value;
}

void CPU::rlca() {
  rlc_r8(A);
  setFlags(false, false, false, Cflag());
}

void CPU::rlc_A() { rlc_r8(A); }

void CPU::rr_r8(u8 &value) {
  bool carry = value & 0x1;
  value = (value >> 1) | (Cflag() << 7);
  setFlags(value == 0, false, false, carry);
}

void CPU::rr_mHL() {
  u8 value = mem[HL];
  bool carry = value & 0x1;
  value = (value >> 1) | (Cflag() << 7);
  setFlags(value == 0, false, false, carry);
  mem[HL] = value;
}

void CPU::rr_A() { rr_r8(A); }

void CPU::rra() {
  rr_r8(A);
  setFlags(false, false, false, Cflag());
}

void CPU::rrc_r8(u8 &value) {
  bool carry = value & 0x1;
  value = (value >> 1) | (carry << 7);
  setFlags(value == 0, false, false, carry);
}

void CPU::rrc_mHL() {
  u8 value = mem[HL];
  bool carry = value & 0x1;
  value = (value >> 1) | (carry << 7);
  setFlags(value == 0, false, false, carry);
  mem[HL] = value;
}

void CPU::rrc_A() { rrc_r8(A); }

void CPU::rrca() {
  rrc_r8(A);
  setFlags(false, false, false, Cflag());
}

void CPU::set_u3_r8(u8 bitPos, u8 &value) { value |= (1 << bitPos); }

void CPU::set_u3_mHL(u8 bitPos) {
  u8 value = mem[HL];
  mem[HL] = value | (1 << bitPos);
}

void CPU::sla_r8(u8 &value) {
  bool carry = (value >> 7) & 0x1;
  value <<= 1;
  setFlags(value == 0, false, false, carry);
}

void CPU::sla_mHL() {
  u8 value = mem[HL];
  bool carry = (value >> 7) & 0x1;
  value <<= 1;
  setFlags(value == 0, false, false, carry);
  mem[HL] = value;
}

void CPU::sra_r8(u8 &value) {
  bool carry = value & 0x1;
  value = (value >> 1) | (value & 0x80);
  setFlags(value == 0, false, false, carry);
}

void CPU::sra_mHL() {
  u8 value = mem[HL];
  bool carry = value & 0x1;
  value = (value >> 1) | (value & 0x80);
  setFlags(value == 0, false, false, carry);
  mem[HL] = value;
}

void CPU::srl_r8(u8 &value) {
  bool carry = value & 0x1;
  value >>= 1;
  setFlags(value == 0, false, false, carry);
}

void CPU::srl_mHL() {
  u8 value = mem[HL];
  bool carry = value & 0x1;
  value >>= 1;
  setFlags(value == 0, false, false, carry);
  mem[HL] = value;
}

void CPU::swap_r8(u8 &value) {
  value = (value >> 4) | (value << 4);
  setFlags(value == 0, false, false, false);
}

void CPU::swap_mHL() {
  u8 value = mem[HL];
  value = (value >> 4) | (value << 4);
  setFlags(value == 0, false, false, false);
  mem[HL] = value;
}
