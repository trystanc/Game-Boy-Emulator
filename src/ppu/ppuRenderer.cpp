#include "ppuRenderer.h"
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>
#include <iostream>
#include <optional>
#include <cassert>

PPURenderer::PPURenderer(AddressBus& _addressBus)
    : mem(_addressBus),
        tileMap1(_addressBus.vram.data() + constants::tileMap1Start - constants::vramStart, constants::tileMapSize),
        tileMap2(_addressBus.vram.data() + constants::tileMap2Start - constants::vramStart, constants::tileMapSize),
        SCX(_addressBus.ioRegisters[constants::scxAddress - constants::ioRegistersStart]),
        SCY(_addressBus.ioRegisters[constants::scyAddress - constants::ioRegistersStart]),
        WX(_addressBus.ioRegisters[constants::wxAddress - constants::ioRegistersStart]),
        WY(_addressBus.ioRegisters[constants::wyAddress - constants::ioRegistersStart]),
        LY(_addressBus.ioRegisters[constants::LYAddress - constants::ioRegistersStart]),
        LCDC(_addressBus.ioRegisters[constants::LCDCAddress - constants::ioRegistersStart]),
        oam(_addressBus.oam.data(), constants::oamSize)

{
    frameBuffer.fill(constants::maxRGBvalue);
    currentSpriteData.reserve(constants::maxSpritesPerLine);
}

void PPURenderer::resetX(){
    currentX = 0;
    currentMinSprite = -1;
    if (windowPixelDrawnOnLineAlready){
        ++internalLineCounter;
    }
    windowPixelDrawnOnLineAlready = false;
    getSprites();
}

void PPURenderer::resetY(){
    currentY = 0;
    internalLineCounter = 0;
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
    if (currentX < 160 && currentY < 144){
    bool wasSpritePixelDrawn {drawSpritePixel()};
    if (!wasSpritePixelDrawn){
        bool wasWindowPixelDrawn {drawWindowPixel()};
        if (!wasWindowPixelDrawn){ //If you draw a sprite pixel(or if you draw a non-transparent), you don't draw a background pixel...
            drawBackgroundPixel(); //If you don't draw a sprite pixel or a window pixel, you draw a background pixel...
        }
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
    return (mapAddressMode()) ? static_cast<u16>((constants::vramStart + offset*constants::tileSize)) 
                                :static_cast<u16>(constants::signedAddressStart + constants::tileSize*static_cast<s8>(offset));
}



bool PPURenderer::drawWindowPixel(){
    if(isWindowEnabled() && (currentX + 7 >= WX) && (currentY >= WY)){
        u8 x_start = u8(currentX + 7 - WX);
        u8 y_start = u8(internalLineCounter);
        u8 x_pixel = x_start % 8;
        u8 y_pixel = y_start % 8;
        u16 i = u16((y_start / 8) * 32 + (x_start / 8));
        u8 offset = windowTileMap() ? tileMap2[i] : tileMap1[i];
        u16 address = getTileAddress(offset);
        u8 pixel = getTilePixel(address, x_pixel, y_pixel);
        writeToBufferBackground(pixel);
        handleInternalLineCounter();
        return true;
    }
    return false;
}

void PPURenderer::drawBackgroundPixel(){
    if (isBackgroundEnabled()){
        u8 x_start = u8(currentX + SCX);
        u8 y_start = u8(currentY + SCY);
        u8 x_pixel = x_start % 8;
        u8 y_pixel = y_start % 8;
        u16 mapIndex = u16((y_start / 8) * 32) + u16((x_start / 8));
        u8 vramOffset = isTileMap2Active() ? tileMap2[mapIndex] : tileMap1[mapIndex];
        u16 tileAddress = getTileAddress(vramOffset);
        u8 pixel = getTilePixel(tileAddress, x_pixel, y_pixel);
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
    [[maybe_unused]] bool temp {currentY == 0};
    u8 yMapped = currentY + 16;
    u8 yMin = getYmin(yMapped);
    int spriteNumber {0};
    for (auto index : spriteDataIndices){
        if(spriteNumber >= constants::maxSpritesPerLine){
            break;
        }
        Sprite sprite = std::span<u8, 4>(oam.subspan(index, 4));
        u8 yPos = sprite[0];
        if (yPos > yMin && yPos <= (yMin + (spriteSizeMode()+1)*8)){
            currentSpriteData.push_back(sprite);
            ++spriteNumber;
        }

    }
    assert(spriteNumber <= constants::maxSpritesPerLine);
    if (spriteNumber > 0){
        std::stable_sort(currentSpriteData.begin(), currentSpriteData.end(),
            [](const Sprite& a, const Sprite& b) {
                return a[1] < b[1];
            }
        );
}

}

std::optional<Sprite> PPURenderer::getCurrentSprite(){

    return std::nullopt;        
}

u16 PPURenderer::getSpriteAddress(u8 tileIndex, u8 yRelative){
    u16 address{constants::vramStart};
    if (spriteSizeMode()){
        if (yRelative > 7) address += static_cast<u16>(constants::tileSize*tileIndex | (1));
        else address += static_cast<u16>(constants::tileSize*(tileIndex & 0xFE));
    }
    else{
        address += static_cast<u16>(constants::tileSize*tileIndex);
    }
    return address;
}

u8 getXRelative(u8 xMapped, u8 spriteX, u8 attributes){
     u8 xRelative = xMapped - spriteX;   
    if( (attributes>>xFlipBitPos)& 1){
        xRelative = 7 - xRelative;
    }
    return xRelative;
}

u8 PPURenderer::getYRelative(u8 yMapped, u8 spriteY, u8 attributes){
    u8 yRelative = yMapped - spriteY;
    if ( (attributes>>yFlipBitPos) & 1){
        yRelative = static_cast<u8>((8*spriteSizeMode() + 7)) - yRelative;
    }
    return yRelative;
}

bool PPURenderer::writeSpritePixel([[maybe_unused]] Sprite sprite){
    u8 tileIndex = sprite[2];
    u8 attributes = sprite[3];
    u8 xMapped = currentX + 8;
    u8 yMapped = currentY + 16;
    u8 yRelative = getYRelative(yMapped, sprite[0], attributes);
    u8 xRelative = getXRelative(xMapped, sprite[1], attributes);

    
    assert(xRelative < 8 && "x-coordinate out of range");
    assert(yRelative < 16 && "y-coordinate out of range");
    u16 address = getSpriteAddress(tileIndex, yRelative);
    u8 pixel = getTilePixel(address, xRelative, yRelative );
    return writeToBufferSprite(pixel, (attributes >> paletteNumberPos) & 0b1);

}

bool PPURenderer::spriteInRange(u8 xStart){
    return ( (xStart - u8(8))<= currentX && currentX < xStart);
}

std::vector<u8> PPURenderer::getValidSpriteIndices(){
    std::vector<u8> validIndicies;
    for(u8 i {0}; i < currentSpriteData.size(); ++i){
        u8 xStart = currentSpriteData[i][1];
        if (spriteInRange(xStart)){
            validIndicies.push_back(i);
        }
    }
    return validIndicies;
}

bool PPURenderer::drawSpritePixel(){
    if (!isObjectEnabled() || currentSpriteData.empty()) return false;
    
    bool spriteDrawn {false};   
    
    std::vector<u8> spriteIndicies {getValidSpriteIndices()};
    auto rit = spriteIndicies.rbegin();
    for(;rit != spriteIndicies.rend(); ++rit){
        bool thisSpriteDrawn = writeSpritePixel(currentSpriteData[*rit]);
        if (thisSpriteDrawn) spriteDrawn = true;
    }
    return spriteDrawn;
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

void PPURenderer::handleInternalLineCounter(){
    if (windowPixelDrawnOnLineAlready)
        return;
    else{
        windowPixelDrawnOnLineAlready = true;
    }

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
bool PPURenderer::isTileMap2Active(){
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