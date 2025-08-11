#include "include/GameManager.h"
#include "main.h"

const float leftLimit = 0.20f * SCREEN_WIDTH + 15.f; // Account for bar width
const float rightLimit = 0.80f * SCREEN_WIDTH - 15.f; // Account for bar width

GameManager::GameManager(sf::RenderWindow& window) : _window(window) {
    // Initialize game states or other resources here
}

void GameManager::start() {
    auto windowSize = _window.getSize();
    sf::Clock clock; // For delta time calculation

    // Create game boundaries
    sf::RectangleShape topBar(sf::Vector2f(static_cast<float>(windowSize.x * 0.60f), 15.f));
    topBar.setFillColor(sf::Color::White);
    topBar.setPosition(sf::Vector2f((0.20f * SCREEN_WIDTH), 0.f));

    sf::RectangleShape leftBar(sf::Vector2f(15.f, static_cast<float>(windowSize.y)));
    leftBar.setFillColor(sf::Color::White);
    leftBar.setPosition(sf::Vector2f(0.20f * SCREEN_WIDTH, 0.f));

    sf::RectangleShape rightBar(sf::Vector2f(15.f, static_cast<float>(windowSize.y)));
    rightBar.setFillColor(sf::Color::White);
    rightBar.setPosition(sf::Vector2f((static_cast<float>(windowSize.x) - 15.f) - (0.20f * SCREEN_WIDTH), 0.f));

    // Game loop
    while (_window.isOpen()) {
        float deltaTime = clock.restart().asSeconds(); // time between two frames (current - previous)
        
        // handle events
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
        }

        // continuous input
        float paddleDirection = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            paddleDirection = -1.f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            paddleDirection = 1.f;
        }

        _paddle.setMovement(paddleDirection);
        _paddle.update(deltaTime, leftLimit, rightLimit);

        // render 
        _window.clear();
        _window.draw(topBar);
        _window.draw(leftBar);
        _window.draw(rightBar);
        _paddle.draw(_window);
        _ball.draw(_window);
        _window.display();
    }
}