#include "ppu.h"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <optional>

//This will do for now but will probably want to add more asserts later when 
//testing rendering.

u8 PPU::getTilePixel(const u16 address, u8 x_pixel, u8 y_pixel){
    u8 row1 = mem[address + 2 * y_pixel];
    u8 row2 = mem[address + 2 * y_pixel + 1];
    return ((row1 >> (7 - x_pixel)) & 0b1) | (((row2 >> (7 - x_pixel)) & 0b1) << 1);
}

enum attributeBits{
    paletteNumberPos = 4,
    xFlipBitPos = 5,
    yFlipBitPos = 6,
    priorityBitPos = 7
};

u16 PPU::getTileAddress(u8 offset){
    return (mapAddressMode())? (constants::vramStart + offset) : (constants::signedAddressStart + static_cast<s8>(offset));
}

void PPU::drawPixel(){
//Basically, if you draw a window pixel, you don't draw a background or sprite pixel...
    if (!drawWindowPixel()){
        if (!drawSpritePixel()){ //If you draw a sprite pixel(or if you draw a non-transparent), you don't draw a background pixel...
            drawBackgroundPixel(); //If you don't draw a sprite pixel or a window pixel, you draw a background pixel...
        }
    }
}

bool PPU::drawWindowPixel(){
    if(isWindowEnabled() && (currentX + 7 >= WX) && (currentY >= WY)){

        u8 x_start = currentX + 7 - WX;
        u8 y_start = currentY - WY;
        u8 x_pixel = x_start % 8;
        u8 y_pixel = y_start % 8;
        u16 i = (y_start / 8) * 32 + (x_start / 8);
        u8 offset = windowTileMap() ? tileMap2[i] : tileMap1[i];
        u16 address = getTileAddress(offset);
        u8 pixel = getTilePixel(address, x_pixel, y_pixel);
        writeToBufferBackground(pixel);
        return true;
    }
    return false;
}

void PPU::drawBackgroundPixel(){
    if (isBackgroundEnabled()){
        u8 x_start = currentX + SCX;
        u8 y_start = currentY + SCY;
        u8 x_pixel = x_start % 8;
        u8 y_pixel = y_start % 8;
        u16 i = (y_start / 8) * 32 + (x_start / 8);
        u8 offset = backgroundTileMap() ? tileMap2[i] : tileMap1[i];
        u16 address = getTileAddress(offset);
        u8 pixel = getTilePixel(address, x_pixel, y_pixel);
        writeToBufferBackground(pixel);
    }
}

u8 PPU::getYmin(u8 yMapped){
    if (spriteSizeMode()){
        return yMapped - 16;
    } else{
        return yMapped - 8;
    }

}

void PPU::getSprites(){
    u8 xMapped = currentX + 8;
    u8 yMapped = currentY + 16;
    u8 yMin = getYmin(yMapped);
    int spriteNumber {0};
    for (auto index : spriteDataIndices){
        Sprite sprite = std::span<u8, 4>(oam.subspan(index, 4));
        u8 yPos = sprite[0];
        if (yPos > yMin){
            currentSpriteData.push_back(sprite);
            ++spriteNumber;
        }

    }
    std::stable_sort(currentSpriteData.begin(), currentSpriteData.end(),
        [](const Sprite& a, const Sprite& b) {
            return a[1] > b[1];
        }
    );

}

std::optional<Sprite> PPU::getCurrentSprite(){
    Sprite currentSprite = currentSpriteData[currentSpriteIndex];
    while (currentX < currentSprite[1]){
        currentSpriteIndex++;
        if (currentSpriteIndex >= currentSpriteData.size()){
            currentSpriteIndex = -1;
            return std::nullopt;
        }
        currentSprite = currentSpriteData[currentSpriteIndex];
    }
    return (currentX + 8 >= currentSprite[1]) ? std::optional<Sprite>{currentSprite} : std::nullopt;
        
}

bool PPU::writeSpritePixel(Sprite sprite){
    u8 tileIndex = sprite[2];
    u8 attributes = sprite[3];
    u8 xMapped = currentX + 8;
    u8 yMapped = currentY + 8;

    u8 yRelative = currentY - sprite[0];
    u8 xRelative = xMapped - sprite[1];
    if (attributes >> yFlipBitPos & 0b1){
        yRelative = 7 + 8 * spriteSizeMode() - yMapped;
    }

    if(attributes >> xFlipBitPos & 0b1){
        xRelative = 7 - xRelative;
    }
    assert(xRelative >= 0 && xRelative < 8 && "x-coordinate out of range");
    assert(yRelative >= 0 && yRelative < (spriteSizeMode() ? 16 : 8) && "y-coordinate out of range");
    u16 address = mem[constants::vramStart + tileIndex + (yRelative >= 8)];
    u8 pixel = getTilePixel(address, xRelative, yRelative % 8);
    return writeToBufferSprite(pixel, attributes >> paletteNumberPos & 0b1);

}

bool PPU::drawSpritePixel(){
    if (!isObjectEnabled()) return false;
    if (currentSpriteIndex == -1) return false;
    std::optional<Sprite> sprite = getCurrentSprite();
    if (sprite){
        if (!(*sprite)[3] >> priorityBitPos) return false;
        else return writeSpritePixel(*sprite);
    }
    return false;
}

void PPU::writeToBuffer(sf::Color colour){
    frameBuffer[(currentY * constants::screenx + currentX) * 4] = colour.r;
    frameBuffer[(currentY * constants::screenx + currentX) * 4 + 1] = colour.g;
    frameBuffer[(currentY * constants::screenx + currentX) * 4 + 2] = colour.b;
    frameBuffer[(currentY * constants::screenx + currentX) * 4 + 3] = colour.a;

}

void PPU::writeToBufferBackground(u8 pixel){
    
    auto colour = colours[getBackgroundColour(pixel)];
    writeToBuffer(colour);
}

bool PPU::writeToBufferSprite(u8 pixel, bool mode){
    u8 colourIndex = (mem[constants::OBP0Address + mode] >> 2*pixel) & 0b11;
    if (!colourIndex) return false;
    writeToBuffer(colours[colourIndex]);
    return true;
}

