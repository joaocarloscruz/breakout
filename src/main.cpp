#include "ui/MainMenu.h"
#include <SFML/Graphics.hpp>

int main(){
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1280, 720)), "Breakout");
    MainMenu mainMenu(window);
    mainMenu.start();
    return 0;
}