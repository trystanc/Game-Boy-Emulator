#include "emulator.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <SFML/System.hpp>
#include <optional>

Emulator::Emulator(std::ifstream& file)
    : GB(file, *this)
{
    window.create(sf::VideoMode({constants::screenx, constants::screeny}), "GameBoy");
    window.setKeyRepeatEnabled(false);

}

void Emulator::run(){
    sf::Clock clock;
    while (window.isOpen()){
        while(const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }
    
    while (frameNotFinished){
        GB.advance();
    }
    
    draw(GB.getFrameBuffer());
    // auto timeElapsed = clock.getElapsedTime().asMicroseconds();
    // sf::sleep(sf::microseconds(constants::frameTimeMicroS - timeElapsed));
    frameNotFinished = true;
    }


}  


void Emulator::draw(const u8* frameBuffer){
    sf::Image image({constants::screenx, constants::screeny}, frameBuffer);
    sf::Texture texture(image);
    sf::Sprite sprite(texture);
    window.clear();
    window.draw(sprite);
    window.display();
}