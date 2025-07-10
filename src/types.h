#pragma once
#include <cstdint>
#include <array>
#include <utility>
#include <span>

using u8 = uint8_t;
using u16 = uint16_t;
using s8 = int8_t;
using s16 = int16_t;
using uint = unsigned int;

template<typename std::size_t N>
using Array = std::array<u8, N>;
template<std::size_t X, std::size_t Y>
using Array2d = std::array<std::array<u8, X>, Y>;
using Tile = std::array<std::pair<u8, u8>, 8>;
using TileMap = std::span<u8, 1024>;
using OAM = std::span<u8, 160>;
using Sprite = std::span<u8, 4>; // x, y, tile index, flags

enum Interrupt{
    VBlank = 0,
    LCD = 1,
    Timer = 2,
    Serial = 3,
    Joypad = 4
};
