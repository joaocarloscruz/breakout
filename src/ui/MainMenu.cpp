#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow& window) : _window(window) {
    if (!_font.openFromFile("assets/font/sansation.ttf")) {
        std::cerr << "Error loading font: assets/font/sansation.ttf" << std::endl;
    }
}

void MainMenu::start() {
    _window.setFramerateLimit(60);
    
    while (_window.isOpen() && !_is_exit_requested) {
        while (std::optional<sf::Event> event = _window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                _window.close();
            }
            else if (event->is<sf::Event::KeyPressed>()) {
                if (auto keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyEvent->code == sf::Keyboard::Key::Escape) {
                        _window.close(); // press escape to leave the game from main menu
                    }
                }
            }
        }
        
        _window.clear();
        _window.display();
    }
}