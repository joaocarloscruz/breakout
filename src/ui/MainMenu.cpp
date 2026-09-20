#include "include/MainMenu.h"
#include "include/Layout.h"
#include "core/include/GameManager.h"

MainMenu::MainMenu(const sf::Font& font)
    : _font(font), _title(font, "BREAKOUT", 50),
      _subtitle(font, "SIX LEVELS. ONE PADDLE.", 22),
      _start(font, "START", 40), _exit(font, "EXIT", 40),
      _help(font, "Enter to start  /  Esc to exit\n\nMove: A / D or arrows    Launch: Space    Pause: P", 20) {
    centerText(_title, SCREEN_WIDTH / 2.f, 100.f);
    _subtitle.setFillColor(sf::Color::Cyan);
    centerText(_subtitle, SCREEN_WIDTH / 2.f, 160.f);
    centerText(_start, SCREEN_WIDTH / 2.f, 440.f);
    centerText(_exit, SCREEN_WIDTH / 2.f, 530.f);
    _help.setFillColor(sf::Color(180, 180, 190));
    centerText(_help, SCREEN_WIDTH / 2.f, 710.f);
}
void MainMenu::draw(sf::RenderTarget& target) const {
    target.draw(_title);
    target.draw(_subtitle);
    target.draw(_start);
    target.draw(_exit);
    target.draw(_help);
}
void MainMenu::start(sf::RenderWindow& window) {
    while (window.isOpen()) {
        bool startRequested = false;
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (event->is<sf::Event::Resized>()) {
                updateView(window);
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) window.close();
                if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space)
                    startRequested = true;
            } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    const auto position = window.mapPixelToCoords(mouse->position);
                    if (_exit.getGlobalBounds().contains(position)) window.close();
                    if (_start.getGlobalBounds().contains(position)) startRequested = true;
                }
            }
        }
        if (!window.isOpen()) break;
        if (startRequested) {
            GameManager game(_font);
            game.start(window);
        } else {
            const auto mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            _start.setFillColor(_start.getGlobalBounds().contains(mouse) ? sf::Color::Cyan : sf::Color::White);
            _exit.setFillColor(_exit.getGlobalBounds().contains(mouse) ? sf::Color::Cyan : sf::Color::White);
            window.clear();
            draw(window);
            window.display();
        }
    }
}
