#include "include/Status.h"

Status::Status(const sf::Font& font)
    : _score(font, "", 24), _lives(font, "", 24), _level(font, "", 22), _remaining(font, "", 20) {
    _score.setPosition({24.f, 24.f});
    _lives.setPosition({24.f, 60.f});
    _level.setPosition({24.f, 130.f});
    _level.setFillColor(sf::Color::Cyan);
    _remaining.setPosition({24.f, 204.f});
}
void Status::draw(sf::RenderTarget& target, const GameSession& session) {
    _score.setString("Score: " + std::to_string(session.score()));
    _lives.setString("Lives: " + std::to_string(session.lives()));
    _level.setString("LEVEL " + std::to_string(session.levelIndex() + 1) + " / " +
        std::to_string(campaignLevels().size()) + "\n" + session.level().name);
    _remaining.setString("Bricks: " + std::to_string(session.bricks().getBricksRemaining()));
    target.draw(_score);
    target.draw(_lives);
    target.draw(_level);
    target.draw(_remaining);
}
