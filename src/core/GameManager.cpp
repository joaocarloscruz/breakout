#include "include/GameManager.h"
#include "main.h"


GameManager::GameManager(sf::RenderWindow& window) : _window(window) {
    // Initialize game states or other resources here
}

void GameManager::start() {
    auto windowSize = _window.getSize();

    sf::RectangleShape topBar(sf::Vector2f(static_cast<float>(windowSize.x * 0.60f), 5.f));
    topBar.setFillColor(sf::Color::White);
    topBar.setPosition(sf::Vector2f((0.20f * SCREEN_WIDTH), 0.f));

    sf::RectangleShape leftBar(sf::Vector2f(15.f, static_cast<float>(windowSize.y)));
    leftBar.setFillColor(sf::Color::White);
    leftBar.setPosition(sf::Vector2f(0.20 * SCREEN_WIDTH, 0.f));

    sf::RectangleShape rightBar(sf::Vector2f(15.f, static_cast<float>(windowSize.y)));
    rightBar.setFillColor(sf::Color::White);
    rightBar.setPosition(sf::Vector2f((static_cast<float>(windowSize.x) - 15.f) - (0.20f * SCREEN_WIDTH), 0.f));

    while (_window.isOpen()) {
        while (std::optional<sf::Event> event = _window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                _window.close();
            }
            else if (event->is<sf::Event::KeyPressed>()) {
                if (auto keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyEvent->code == sf::Keyboard::Key::A || keyEvent->code == sf::Keyboard::Key::Left) {
                        _paddle.move(-20.f);
                    }
                    else if (keyEvent->code == sf::Keyboard::Key::D || keyEvent->code == sf::Keyboard::Key::Right) {
                        _paddle.move(20.f);
                    }
                }
            }
        }
        _window.clear();
        _window.draw(topBar);
        _window.draw(leftBar);
        _paddle.draw(_window);
        _window.draw(rightBar);
        _window.display();
    }
}