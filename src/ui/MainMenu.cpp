#include "MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow& window) : _window(window) {
    // Try to load the custom font first
    if (!_font.openFromFile("/assets/font/sansation.ttf")) {
        std::cerr << "Warning: Could not load custom font, using system font" << std::endl;
        
        // Fallback to system font
        #ifdef _WIN32
            if (!_font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
                std::cerr << "Error: Could not load any font!" << std::endl;
                return;
            }
        #else
            if (!_font.openFromFile("/System/Library/Fonts/Arial.ttf") && 
                !_font.openFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) {
                std::cerr << "Error: Could not load any font!" << std::endl;
                return;
            }
        #endif
    }
    
    _title.emplace(_font);
    setup_title();
    setup_exit_button();
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
                        _window.close();
                    }
                }
            }
            else if (event->is<sf::Event::MouseButtonPressed>()) {
                if (auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        handle_mouse_click(sf::Vector2f(mouseEvent->position.x, mouseEvent->position.y));
                    }
                }
            }
        }
        
        _window.clear();
        _window.draw(*_title);
        _window.draw(*_exit_button);
        _window.display();
    }
}

void MainMenu::setup_title() {
    _title->setString("BREAKOUT");
    _title->setCharacterSize(50);
    _title->setFillColor(sf::Color::White);
    
    sf::FloatRect titleBounds = _title->getLocalBounds();
    sf::Vector2u windowSize = _window.getSize();
    
    _title->setPosition(sf::Vector2f(
        (windowSize.x - titleBounds.size.x) / 2.0f, 
        50.0f
    ));
}

void MainMenu::setup_exit_button() {
    _exit_button.emplace(_font);
    _exit_button->setString("EXIT");
    _exit_button->setCharacterSize(40);
    _exit_button->setFillColor(sf::Color::White);

    sf::FloatRect exitBounds = _exit_button->getLocalBounds();
    sf::Vector2u windowSize = _window.getSize();

    _exit_button->setPosition(sf::Vector2f(
        (windowSize.x - exitBounds.size.x) / 2.0f,
        windowSize.y - exitBounds.size.y - 150.0f
    ));
}

void MainMenu::handle_mouse_click(sf::Vector2f mousePos) {
    sf::FloatRect buttonBounds = _exit_button->getGlobalBounds();
    if (buttonBounds.contains(mousePos)) {
        _window.close();
    }
}
