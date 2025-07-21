#pragma once
#include <cstdint>
#include <array>

using u8 = uint8_t;
using u16 = uint16_t;
using s8 = int8_t;
using s16 = int16_t;
using uint = unsigned int;

template<typename std::size_t N>
using Array = std::array<u8, N>;

//values correspond to the bit in interrupt flag register.
enum class Interrupt{
    VBlank = 0,
    STAT = 1,
    Timer = 2,
    Serial = 3,
    Joypad = 4,
    numOfInterrupts = 5,
};