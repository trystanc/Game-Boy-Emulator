#include "ppuRenderer.h"
#include <algorithm>
#include <optional>
#include <cassert>

PPURenderer::PPURenderer(AddressBus& _addressBus)
    : mem(_addressBus),
        tileMap1(_addressBus.vram.data() + constants::tileMap1Start - constants::vramStart, 1024),
        tileMap2(_addressBus.vram.data() + constants::tileMap2Start - constants::vramStart, 1024),
        SCX(_addressBus.ioRegisters[constants::scxAddress - constants::ioRegistersStart]),
        SCY(_addressBus.ioRegisters[constants::scyAddress - constants::ioRegistersStart]),
        WX(_addressBus.ioRegisters[constants::wxAddress - constants::ioRegistersStart]),
        WY(_addressBus.ioRegisters[constants::wyAddress - constants::ioRegistersStart]),
        LY(_addressBus.ioRegisters[constants::LYAddress - constants::ioRegistersStart]),
        LCDC(_addressBus.ioRegisters[constants::LCDCAddress - constants::ioRegistersStart]),
        oam(_addressBus.oam.data(), 160)

{
    frameBuffer.fill(constants::maxRGBvalue);
    currentSpriteData.reserve(constants::maxSpritesPerLine);
}

void PPURenderer::resetX(){
    currentX = 0;
}

void PPURenderer::resetY(){
    currentY = 0;
}

void PPURenderer::incrementX(){
    ++currentX;
}

void PPURenderer::incrementY(){
    ++currentY;
}

const u8& PPURenderer::getY(){
    return currentY;
}

void PPURenderer::drawPixel(){
//Basically, if you draw a sprite pixel(depending on object priority, whether its transparent etc.), you don't draw a background or sprite pixel...
    if (!drawSpritePixel()){
        if (!drawWindowPixel()){ //If you draw a sprite pixel(or if you draw a non-transparent), you don't draw a background pixel...
            drawBackgroundPixel(); //If you don't draw a sprite pixel or a window pixel, you draw a background pixel...
        }
    }
}

enum attributeBits{
    paletteNumberPos = 4,
    xFlipBitPos = 5,
    yFlipBitPos = 6,
    priorityBitPos = 7
};

//LCD control
enum LCDcontrolBits{
    bgWindowEnablePos = 0,
    objEnablePos = 1,
    objSizePos = 2,
    bgTileMapPos = 3,
    bgWindowTilesPos = 4,
    windowEnablePos = 5,
    windowTileMapAreaPos = 6,
    LCDPPUenablePos = 7

};

u8 PPURenderer::getTilePixel(const u16 address, u8 x_pixel, u8 y_pixel){
    u8 row1 = mem[(u16)(address + 2 * y_pixel)];
    u8 row2 = mem[(u16)(address + 2 * y_pixel + 1)];
    return ((row1 >> (7 - x_pixel)) & 0b1) | (((row2 >> (7 - x_pixel)) & 0b1) << 1);
}



u16 PPURenderer::getTileAddress(u8 offset){
    return (mapAddressMode()) ? static_cast<u16>((constants::vramStart + offset*static_cast<u16>(16))) : static_cast<u16>(constants::signedAddressStart + 16*static_cast<s8>(offset));
}



bool PPURenderer::drawWindowPixel(){
    if(isWindowEnabled() && (currentX + 7 >= WX) && (currentY >= WY)){

        u8 x_start = u8(currentX + 7 - WX);
        u8 y_start = u8(currentY - WY);
        u8 x_pixel = x_start % 8;
        u8 y_pixel = y_start % 8;
        u16 i = u16((y_start / 8) * 32 + (x_start / 8));
        u8 offset = windowTileMap() ? tileMap2[i] : tileMap1[i];
        u16 address = getTileAddress(offset);
        u8 pixel = getTilePixel(address, x_pixel, y_pixel);
        writeToBufferBackground(pixel);
        return true;
    }
    return false;
}

constexpr std::array<u8, 10> tileNumbers {48, 49, 45, 155, 112, 101, 99, 105, 97, 108};

void PPURenderer::drawBackgroundPixel(){
    if (isBackgroundEnabled()){
        u8 x_start = u8(currentX + SCX);
        u8 y_start = u8(currentY + SCY);
        u8 x_pixel = x_start % 8;
        u8 y_pixel = y_start % 8;
        u16 i = u16((y_start / 8) * 32) + u16((x_start / 8));
        u8 offset = backgroundTileMap() ? tileMap2[i] : tileMap1[i];
        u16 address = getTileAddress(offset);
        u8 pixel = getTilePixel(address, x_pixel, y_pixel);
        writeToBufferBackground(pixel);
    }
}

u8 PPURenderer::getYmin(u8 yMapped){
    if (spriteSizeMode()){
        return yMapped - 16;
    } else{
        return yMapped - 8;
    }

}

void PPURenderer::getSprites(){
    currentSpriteData.clear();
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

std::optional<Sprite> PPURenderer::getCurrentSprite(){
    Sprite currentSprite = currentSpriteData[currentSpriteIndex];
    while (currentX < currentSprite[1]){
        currentSpriteIndex++;
        if (uint(currentSpriteIndex) >= currentSpriteData.size()){
            currentSpriteIndex = -1;
            return std::nullopt;
        }
        currentSprite = currentSpriteData[currentSpriteIndex];
    }
    return (currentX + 8 >= currentSprite[1]) ? std::optional<Sprite>{currentSprite} : std::nullopt;
        
}

bool PPURenderer::writeSpritePixel(Sprite sprite){
    u8 tileIndex = sprite[2];
    u8 attributes = sprite[3];
    u8 xMapped = currentX + 8;
    u8 yMapped = currentY + 8;

    u8 yRelative = currentY - sprite[0];
    u8 xRelative = xMapped - sprite[1];
    if (attributes >> yFlipBitPos & 0b1){
        yRelative = u8(7 + 8 * spriteSizeMode() - yMapped);
    }

    if(attributes >> xFlipBitPos & 0b1){
        xRelative = 7 - xRelative;
    }
    assert(xRelative >= 0 && xRelative < 8 && "x-coordinate out of range");
    assert(yRelative >= 0 && yRelative < (spriteSizeMode() ? 16 : 8) && "y-coordinate out of range");
    u16 address = mem[u16(constants::vramStart + tileIndex + (yRelative >= 8))];
    u8 pixel = getTilePixel(address, xRelative, yRelative % 8);
    return writeToBufferSprite(pixel, (attributes >> paletteNumberPos) & 0b1);

}

bool PPURenderer::drawSpritePixel(){
    if (!isObjectEnabled()) return false;
    if (currentSpriteIndex == -1) return false;
    std::optional<Sprite> sprite = getCurrentSprite();
    if (sprite){
        if (!(*sprite)[3] >> priorityBitPos && isBackgroundEnabled())  return false;
        return writeSpritePixel(*sprite);
    }
    return false;
}

void PPURenderer::writeToBuffer(sf::Color colour){
    frameBuffer[(currentY * constants::screenx + currentX) * 4] = colour.r;
    frameBuffer[(currentY * constants::screenx + currentX) * 4 + 1] = colour.g;
    frameBuffer[(currentY * constants::screenx + currentX) * 4 + 2] = colour.b;
    frameBuffer[(currentY * constants::screenx + currentX) * 4 + 3] = colour.a;

}

void PPURenderer::writeToBufferBackground(u8 pixel){
    
    auto colour = colours[getBackgroundColour(pixel)];
    writeToBuffer(colour);
}

bool PPURenderer::writeToBufferSprite(u8 pixel, bool mode){
    u8 colourIndex = (mem[constants::OBP0Address + mode] >> 2*pixel) & 0b11;
    if (!colourIndex) return false;
    writeToBuffer(colours[colourIndex]);
    return true;
}

    bool PPURenderer::isWindowEnabled() {
        return ((LCDC >> windowEnablePos) & 0b1) & (LCDC >> bgWindowEnablePos & 0b1);
    }
    bool PPURenderer::isBackgroundEnabled() {
        return (LCDC >> bgWindowEnablePos & 0b1);
    }
    bool PPURenderer::mapAddressMode(){
        return (LCDC >> bgWindowTilesPos & 0b1);
    }
    bool PPURenderer::windowTileMap(){
        return (LCDC >> windowTileMapAreaPos & 0b1);
    }
    bool PPURenderer::backgroundTileMap(){
        return (LCDC >> bgTileMapPos & 0b1);
    }
    bool PPURenderer::spriteSizeMode(){
        return (LCDC >> objSizePos & 0b1);
    }
    bool PPURenderer::isObjectEnabled(){
        return (LCDC >> objEnablePos & 0b1);
    }
    u8 PPURenderer::getBackgroundColour(u8 pixel){
        return (mem[constants::BGPAddress] >> 2*pixel) & 0b11;
    }