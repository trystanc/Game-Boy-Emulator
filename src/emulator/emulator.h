#include "../gb/gameboy.h"
#include<SFML/Graphics.hpp>

class Emulator{
    
public:
    Emulator(std::ifstream& file);
    bool frameNotFinished{true};
    void run();
    void draw(const u8* frameBuffer);


private:
    GameBoy GB;
    sf::RenderWindow window;

};