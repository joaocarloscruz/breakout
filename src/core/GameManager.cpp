#include "include/GameManager.h"
#include "ui/include/Layout.h"

GameManager::GameManager(const sf::Font& font)
    : _font(font), _status(font), _pauseButton(font, "PAUSE [P]", 22),
      _help(font, "MOVE\nA / D or arrows\n\nLAUNCH / NEXT\nSpace or Enter\n\nPAUSE\nP\n\nMENU\nEsc\n\nOutlined bricks\ntake two hits", 18) {
    _pauseButton.setPosition({1050.f, 24.f});
    _help.setPosition({1050.f, 120.f});
    _help.setFillColor(sf::Color(180, 180, 190));
}

void GameManager::start(sf::RenderWindow& window) {
    sf::Clock clock;
    while (window.isOpen()) {
        float elapsed = clock.restart().asSeconds();
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (event->is<sf::Event::Resized>()) {
                updateView(window);
            } else if (event->is<sf::Event::FocusLost>()) {
                _session.pause();
                elapsed = 0.f;
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) return;
                if (key->code == sf::Keyboard::Key::P) _session.togglePause();
                if (key->code == sf::Keyboard::Key::Space || key->code == sf::Keyboard::Key::Enter)
                    _session.advance();
                elapsed = 0.f;
            } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left &&
                    _pauseButton.getGlobalBounds().contains(window.mapPixelToCoords(mouse->position))) {
                    _session.togglePause();
                    elapsed = 0.f;
                }
            }
        }
        if (!window.isOpen()) break;
        float direction = 0.f;
        if (window.hasFocus()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) direction -= 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) direction += 1.f;
        }
        _session.update(elapsed, direction);
        window.clear();
        draw(window);
        window.display();
    }
}

void GameManager::drawOverlay(sf::RenderTarget& target, const std::string& title,
                              const std::string& detail, sf::Color color) {
    sf::RectangleShape panel({arena::right - arena::left - 40.f, 190.f});
    panel.setPosition({arena::left + 20.f, 370.f});
    panel.setFillColor(sf::Color(8, 8, 14, 235));
    panel.setOutlineColor(sf::Color(70, 70, 80));
    panel.setOutlineThickness(1.f);
    target.draw(panel);
    sf::Text heading(_font, title, 42);
    heading.setFillColor(color);
    centerText(heading, SCREEN_WIDTH / 2.f, 425.f);
    target.draw(heading);
    sf::Text body(_font, detail, 22);
    centerText(body, SCREEN_WIDTH / 2.f, 505.f);
    target.draw(body);
}

void GameManager::draw(sf::RenderTarget& target) {
    sf::RectangleShape topBar({0.6f * SCREEN_WIDTH, 15.f});
    topBar.setPosition({0.2f * SCREEN_WIDTH, 0.f});
    sf::RectangleShape leftBar({15.f, SCREEN_HEIGHT});
    leftBar.setPosition({0.2f * SCREEN_WIDTH, 0.f});
    sf::RectangleShape rightBar({15.f, SCREEN_HEIGHT});
    rightBar.setPosition({arena::right, 0.f});
    target.draw(topBar);
    target.draw(leftBar);
    target.draw(rightBar);
    _status.draw(target, _session);
    _pauseButton.setString(_session.state() == PlayState::Paused ? "RESUME [P]" : "PAUSE [P]");
    target.draw(_pauseButton);
    target.draw(_help);
    _session.bricks().draw(target);
    _session.paddle().draw(target);
    _session.ball().draw(target);

    switch (_session.state()) {
    case PlayState::Ready:
        drawOverlay(target, _session.level().name, "Move to aim  /  Space or Enter to launch", sf::Color::Cyan);
        break;
    case PlayState::Paused:
        drawOverlay(target, "PAUSED", "P to resume  /  Esc for menu");
        break;
    case PlayState::LevelComplete:
        drawOverlay(target, "LEVEL COMPLETE", "+500 points  /  +1 life (max 5)\nSpace or Enter for the next level", sf::Color::Green);
        break;
    case PlayState::Won:
        drawOverlay(target, "YOU WIN!", "All six levels cleared!  Score: " + std::to_string(_session.score()) +
            "\nSpace to play again  /  Esc for menu", sf::Color::Yellow);
        break;
    case PlayState::GameOver:
        drawOverlay(target, "GAME OVER", "Final score: " + std::to_string(_session.score()) +
            "\nSpace to try again  /  Esc for menu", sf::Color::Red);
        break;
    case PlayState::Playing:
        break;
    }
}
