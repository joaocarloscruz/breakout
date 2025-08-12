#include "include/GameManager.h"
#include "main.h"
#include <cmath>


const float wallThickness = 15.f;
const float topLimit = 15.f;
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

        _ball.update(deltaTime);

        sf::Vector2f ballPosition = _ball.getPosition();
        sf::FloatRect ballBounds = _ball.getBounds();

        // Collision with left or right wall
        if (ballPosition.x <= leftLimit || ballBounds.position.x + ballBounds.size.x >= rightLimit) {
            _ball.reverseX();
        }

        // Collision with top wall
        if (ballPosition.y <= topLimit ) {
            _ball.reverseY();
        }

        if (ballPosition.y >= _window.getSize().y) {
            // TO-DO: remove a life
        }        

        // collision with paddle
        if (ballBounds.findIntersection(_paddle.getBounds())) {
            _ball.reverseY();
            
            float halfPaddleWidth = PADDLE_WIDTH / 2.0f;
            float paddleCenter = _paddle.getPosition().x + halfPaddleWidth;
            float ballCenter = ballPosition.x + ballBounds.size.x / 2;
            float offset = ballCenter - paddleCenter;
            
            // Normalize offset into a -1, 1 range
            float normalizedOffset = offset / halfPaddleWidth;
            
            sf::Vector2f currentVelocity = _ball.getVelocity();
            float speed = std::sqrt(currentVelocity.x * currentVelocity.x + currentVelocity.y * currentVelocity.y); 
            
            float bounceAngle = 0.0f;
            if (normalizedOffset <= -0.75f) {        // Far-left -60 degrees
                bounceAngle = -60.0f * M_PI / 180.0f;
            }
            else if (normalizedOffset <= -0.5f) {    // Left -45 degrees  
                bounceAngle = -45.0f * M_PI / 180.0f;
            }
            else if (normalizedOffset <= -0.25f) {   // Center-left -30 degrees
                bounceAngle = -30.0f * M_PI / 180.0f;
            }
            else if (normalizedOffset <= 0.25f) {    // Center straight up
                bounceAngle = 0.0f;
            }
            else if (normalizedOffset <= 0.5f) {     // Center-right 30 degrees
                bounceAngle = 30.0f * M_PI / 180.0f;
            }
            else if (normalizedOffset <= 0.75f) {    // Right 45 degrees
                bounceAngle = 45.0f * M_PI / 180.0f;
            }
            else {                                   // Far-right 60 degrees
                bounceAngle = 60.0f * M_PI / 180.0f;
            }
            
            // Calculate new velocity components
            float newVx = speed * std::sin(bounceAngle);
            float newVy = -speed * std::cos(bounceAngle); // Negative because we want upward movement
            _ball.setVelocity(newVx, newVy);
            
            // Move ball slightly above paddle to prevent multiple collisions
            sf::FloatRect paddleBounds = _paddle.getBounds();
            float ballRadius = BALL_RADIUS;
            _ball.setPosition(ballCenter - ballRadius, paddleBounds.position.y - 2 * ballRadius);
        }

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