#include "../types.h"
#include "RegisterPair.h"
#include "cpu.h"

u8 CPU::add8bit(const uint first, const uint second) {
  u8 result = static_cast<u8>(first) + static_cast<u8>(second);
  bool zero = (result == 0);
  bool halfCarry = ((first & 0xf) + (second & 0xf)) > 0xf;
  bool carry = ((first & 0xff) + (second & 0xff)) > 0xff;
  setFlags(zero, false, halfCarry, carry);
  return result;
}

u16 CPU::add16bit(const uint first, const uint second) {
  u16 result = static_cast<u16>(first) + static_cast<u16>(second);
  bool halfCarry = ((first & 0xfff) + (second & 0xfff)) > 0xfff;
  bool carry = ((first & 0xffff) + (second & 0xffff)) > 0xffff;
  setFlags(Zflag(), false, halfCarry, carry);
  return result;
}

u8 CPU::adc8bit(const uint first, const uint second) {
  u8 result = u8(static_cast<u8>(first) + static_cast<u8>(second)) +
              static_cast<u8>(Cflag());
  bool zero = (result == 0);
  bool halfCarry = (((first & 0xf) + (second & 0xf)) + Cflag()) > 0xf;
  bool carry = ((first & 0xff) + (second & 0xff)) + Cflag() > 0xff;
  setFlags(zero, false, halfCarry, carry);
  return result;
}

u16 CPU::SPe8() {
  u8 n8 = getn8();
  s8 e8 = static_cast<s8>(n8);
  int result = static_cast<int>(sp) + e8;
  bool carry = ((sp & 0xff) + (n8 & 0xff)) > 0xff;
  bool halfCarry = ((sp & 0xf) + (n8 & 0xf)) > 0xf;
  setFlags(false, false, halfCarry, carry);
  return static_cast<u16>(result);
}

void CPU::adc_A_r8(Register &r8) { A = adc8bit(A, r8); }

void CPU::adc_A_mHL() { A = adc8bit(A, mem[HL]); }

void CPU::adc_A_n8() { A = adc8bit(A, getn8()); }

void CPU::add_A_r8(Register &r8) { A = add8bit(A, r8); }

void CPU::add_A_mHL() { A = add8bit(A, mem[HL]); }

void CPU::add_A_n8() { A = add8bit(A, getn8()); }

void CPU::add_HL_r16(RegisterPair &r16) { HL = add16bit(HL, r16); }

void CPU::add_HL_SP() { HL = add16bit(HL, sp); }

void CPU::add_SP_e8() { sp = SPe8(); }

void CPU::and_A(u8 val) {
  A &= val;
  bool zero = (A == 0);
  setFlags(zero, false, true, false);
}
void CPU::cp_A(u8 val) {
  s16 result = static_cast<s16>(A) - static_cast<s16>(val);
  bool zero = (result == 0);
  bool carry = (result < 0);
  bool halfCarry = ((A & 0xf) - (val & 0xf)) < 0;
  setFlags(zero, true, halfCarry, carry);
}

void CPU::ccf() { setFlags(Zflag(), false, false, !Cflag()); }

void CPU::cpl() {
  A = ~A;
  setFlags(Zflag(), true, true, Cflag());
}

void CPU::daa() {
  u8 correction = 0;
  bool carry = false;

  if (!Nflag()) {
    if (Cflag() || A > 0x99) {
      correction |= 0x60;
      carry = true;
    }
    if (Hflag() || (A & 0x0F) > 0x09) {
      correction |= 0x06;
    }
    A += correction;
  } else {
    if (Cflag()) {
      correction |= 0x60;
    }
    if (Hflag()) {
      correction |= 0x06;
    }
    A -= correction;
    carry = Cflag();
  }

  setFlags(A == 0, Nflag(), false, carry);
}

void CPU::dec8bit(u8 &value) {
  bool zero = (value == 0x1);
  bool halfCarry = ((value & 0xf) == 0);
  --value;
  setFlags(zero, true, halfCarry, Cflag());
}

u8 CPU::dec8bit_mHL(u8 value) {
  bool zero = (value == 0x1);
  bool halfCarry = ((value & 0xf) == 0);
  --value;
  setFlags(zero, true, halfCarry, Cflag());
  return value;
}

void CPU::dec16bit(RegisterPair &r16) { --r16; }

void CPU::dec16bit(u16 &value) { --value; }

void CPU::inc8bit(u8 &value) {
  bool zero = (value == 0xff);
  bool halfCarry = ((value & 0xf) == 0x0f);
  ++value;
  setFlags(zero, false, halfCarry, Cflag());
}

u8 CPU::inc8bit_mHL(u8 value) {
  bool zero = (value == 0xff);
  bool halfCarry = ((value & 0xf) == 0x0f);
  ++value;
  setFlags(zero, false, halfCarry, Cflag());
  return value;
}

void CPU::inc16bit(RegisterPair &r16) { ++r16; }

void CPU::inc16bit(u16 &value) { ++value; }

void CPU::or_A(u8 val) {
  A |= val;
  bool zero = (A == 0);
  setFlags(zero, false, false, false);
}

void CPU::sbc_A(u8 value) {
  u16 subtrahend = value + Cflag();
  [[maybe_unused]] bool x{Cflag()};
  u8 result = A - static_cast<u8>(subtrahend);
  bool zero = (result == 0);
  bool carry = (A < subtrahend);
  bool halfCarry = (A & 0xf) < ((value & 0xf) + Cflag());
  setFlags(zero, true, halfCarry, carry);
  A = result;
}

void CPU::scf() { setFlags(Zflag(), false, false, true); }

void CPU::sub_A(u8 value) {
  u8 result = A - value;
  bool zero = (result == 0);
  bool carry = (A < value);
  bool halfCarry = (A & 0xf) < (value & 0xf);
  setFlags(zero, true, halfCarry, carry);
  A = static_cast<u8>(result);
}

void CPU::xor_A(u8 val) {
  A ^= val;
  bool zero = (A == 0);
  setFlags(zero, false, false, false);
}
