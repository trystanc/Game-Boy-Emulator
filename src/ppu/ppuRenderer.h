#pragma once
#include "../types.h"
#include "../addressBus/addressBus.h"
#include "../constants.h"
#include <SFML/Graphics/Color.hpp>
#include <optional>
#include<utility>

constexpr std::array<u8, 40> makeSpriteDataIndices();
class PPURenderer{
 public:
    PPURenderer(AddressBus& _addressBus);
    const u8* getFrameBuffer(){
        return frameBuffer.data();
    }
    void getSprites();
    void drawPixel();
    void resetX();
    void resetY();
    void incrementX();
    void incrementY();
    const u8& getY();
private:
    AddressBus& mem;
    TileMap tileMap1;
    TileMap tileMap2;
    const u8& SCX, SCY;
    const u8& WX;
    const u8& WY; 
    const u8& LY;
    const u8& LCDC;
    OAM oam;
    std::array<u8, constants::screenx * constants::screeny *4> frameBuffer;
    u8 currentX{0};
    u8 currentY{0};
//array index is the colour index as defined by the gameboy, so 0 is white, 1 is light grey, 2 is dark grey, 3 is black    
    const std::array<sf::Color, 4> colours { 
        sf::Color{255, 255, 255},
        sf::Color{170, 170, 170},
        sf::Color{85, 85, 85},
        sf::Color{0, 0, 0},
    };

    std::vector<Sprite> currentSpriteData;
    int currentSpriteIndex{0};
    static constexpr std::array<u8, constants::maxSpritesPerFrame> spriteDataIndices = [] () {
    std::array<u8, 40> arr = {};
    for (u8 i = 0; i < 40; ++i) {
        arr[i] = i * 4;
    }
    return arr;
    }();

    bool isWindowEnabled();
    bool isBackgroundEnabled();
    bool mapAddressMode();
    bool windowTileMap();
    bool backgroundTileMap();
    bool spriteSizeMode();
    bool isObjectEnabled();
    u8 getBackgroundColour(u8 pixel);
    u16 getTileAddress(u8 offset);
    u8 getTilePixel(const u16 address, u8 x_pixel, u8 y_pixel);
    bool drawWindowPixel();
    void drawBackgroundPixel();
    void writeToBuffer(sf::Color colour);
    void writeToBufferBackground(u8 pixel);
    bool writeToBufferSprite(u8 pixel, bool mode);
    u8 getYmin(u8 yMapped);
    std::optional<Sprite> getCurrentSprite();
    bool drawSpritePixel();
    bool writeSpritePixel(Sprite sprite);
};