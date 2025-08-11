#include "ui/include/MainMenu.h"
#include <SFML/Graphics.hpp>
#include "main.h"

int main(){
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(SCREEN_WIDTH, SCREEN_HEIGHT)), "Breakout");
    MainMenu mainMenu(window);
    mainMenu.start();
    return 0;
}