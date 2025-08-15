#include "ui/include/MainMenu.h"
#include <SFML/Graphics.hpp>
#include "main.h"

int main(){
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(SCREEN_WIDTH, SCREEN_HEIGHT)), "Breakout");
    sf::Font mainfont;
    // Try to load the custom font first
    if (!mainfont.openFromFile("/assets/font/sansation.ttf")) {
        std::cerr << "Warning: Could not load custom font, using system font" << std::endl;
        
        // Fallback to system font
        #ifdef _WIN32
            if (!mainfont.openFromFile("C:/Windows/Fonts/arial.ttf")) {
                std::cerr << "Error: Could not load any font!" << std::endl;
                return 0;
            }
        #else
            if (!mainfont.openFromFile("/System/Library/Fonts/Arial.ttf") && !mainfont.openFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
                std::cerr << "Error: Could not load any font!" << std::endl;
                return 0;
            }
        #endif
    }
    MainMenu mainMenu(window, mainfont);
    mainMenu.start();
    return 0;
}