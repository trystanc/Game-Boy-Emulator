#include "emulator.h"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
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
            else if (const auto* keyPressed  = event->getIf<sf::Event::KeyPressed>()){
                GB.handleKeyEvent(keyPressed, false);
            }
            else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()){
                GB.handleKeyEvent(keyReleased, true);
            }
        }
    
    while (frameNotFinished){
        GB.advance();
    }
    
    draw(GB.getFrameBuffer());
    auto timeElapsed = clock.getElapsedTime().asMicroseconds();
    if (timeElapsed < constants::frameTimeMicroS){
        sf::sleep(sf::microseconds(constants::frameTimeMicroS - timeElapsed));
    }
    clock.restart();
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

