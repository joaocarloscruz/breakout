#include "include/MainMenu.h"
#include "core/include/GameManager.h"

MainMenu::MainMenu(sf::RenderWindow& window, sf::Font& font) : _window(window), _font(font) {
    setup_title();
    setup_start_button();
    setup_exit_button();
}

void MainMenu::start() {
    _window.setFramerateLimit(60);

    while (_window.isOpen() && !_is_exit_requested && !_is_start_requested) {
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
        _window.draw(*_start_button);
        _window.draw(*_exit_button);
        _window.display();
    }
    if(_window.isOpen() && _is_start_requested) {
        GameManager game(_window, _font);
        game.start();
    }
}

void MainMenu::setup_title() {
    _title.emplace(_font);
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

void MainMenu::setup_start_button() {
    _start_button.emplace(_font);
    _start_button->setString("START");
    _start_button->setCharacterSize(40);
    _start_button->setFillColor(sf::Color::White);

    sf::FloatRect startBounds = _start_button->getLocalBounds();
    sf::Vector2u windowSize = _window.getSize();

    _start_button->setPosition(sf::Vector2f(
        (windowSize.x - startBounds.size.x) / 2.0f,
        windowSize.y - startBounds.size.y - 400.0f
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
        windowSize.y - exitBounds.size.y - 300.0f
    ));
}

void MainMenu::handle_mouse_click(sf::Vector2f mousePos) {
    sf::FloatRect buttonBounds = _exit_button->getGlobalBounds();
    if (buttonBounds.contains(mousePos)) {
        _window.close();
        return;
    }

    buttonBounds = _start_button->getGlobalBounds();
    if (buttonBounds.contains(mousePos)) {
        _is_start_requested = true;
        return;
    }

}