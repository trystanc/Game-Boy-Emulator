#pragma once
#include "../types.h"
#include "../addressBus/addressBus.h"
#include "../constants.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <span>
#include <array>

constexpr std::array<u8, 40> makeSpriteDataIndices() {
    std::array<u8, 40> arr = {};
    for (u8 i = 0; i < 40; ++i) {
        arr[i] = i * 4;
    }
    return arr;
}
//may need to change how LCDC.0 is handled.
class PPU{
public:
    PPU(AddressBus& _addressBus, sf::RenderWindow& _window)
        : mem(_addressBus),
          window(_window),
          tileMap1(_addressBus.vram.data() + constants::tileMap1Start - constants::vramStart, 1024),
          tileMap2(_addressBus.vram.data() + constants::tileMap2Start - constants::vramStart, 1024),
          SCY{_addressBus[constants::scyAddress]},
          SCX{_addressBus[constants::scxAddress]},
          WX{_addressBus[constants::wxAddress]},
          WY{_addressBus[constants::wyAddress]},
          LCDC{_addressBus[constants::LCDCAddress]},
          oam{_addressBus.oam.data(), 160}

    {
        frameBuffer.fill(constants::maxRGBvalue);
        currentSpriteData.reserve(constants::maxSpritesPerLine);
    }

    Tile getTile(u16 address);
    const u8* getFrameBuffer(){
        return frameBuffer.data();
    }
    void tick();

private:
    AddressBus& mem;
    TileMap tileMap1;
    TileMap tileMap2;
    OAM oam;
    const u8& SCX, SCY;
    std::array<u8, constants::screenx * constants::screeny *4> frameBuffer;
    const u8& LCDC;
    u8& WX;
    u8& WY; 
    sf::RenderWindow& window;

//array index is the colour index as defined by the gameboy, so 0 is white, 1 is light grey, 2 is dark grey, 3 is black    
    const std::array<sf::Color, 4> colours { 
        sf::Color{255, 255, 255},
        sf::Color{170, 170, 170},
        sf::Color{85, 85, 85},
        sf::Color{0, 0, 0},
    };

    u8 currentX;
    u8 currentY;
    std::vector<Sprite> currentSpriteData;
    int currentSpriteIndex{0};
    static constexpr std::array<u8, constants::maxSpritesPerFrame> spriteDataIndices = makeSpriteDataIndices();

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

    bool isWindowEnabled() {
        return ((LCDC >> windowEnablePos) & 0b1) & (LCDC >> bgWindowEnablePos & 0b1);
    }

    bool isBackgroundEnabled() {
        return (LCDC >> bgWindowEnablePos & 0b1);
    }

    bool mapAddressMode(){
        return (LCDC >> bgWindowTilesPos & 0b1);

    }
    bool windowTileMap(){
        return (LCDC >> windowTileMapAreaPos & 0b1);
    }
    bool backgroundTileMap(){
        return (LCDC >> bgTileMapPos & 0b1);
    }

    bool spriteSizeMode(){
        return (LCDC >> objSizePos & 0b1);
    }
    bool isObjectEnabled(){
        return (LCDC >> objEnablePos & 0b1);
    }

    u8 getBackgroundColour(u8 pixel){
        return (mem[constants::BGPAddress] >> 2*pixel) & 0b11;
    }
    

    u16 getTileAddress(u8 offset);
    void drawPixel();
    u8 getTilePixel(const u16 address, u8 x_pixel, u8 y_pixel);
    bool drawWindowPixel();
    void drawBackgroundPixel();
    void writeToBuffer(sf::Color colour);
    void writeToBufferBackground(u8 pixel);
    bool writeToBufferSprite(u8 pixel, bool mode);
    void getSprites();
    u8 getYmin(u8 yMapped);
    std::optional<Sprite> getCurrentSprite();
    bool drawSpritePixel();
    bool writeSpritePixel(Sprite sprite);

};