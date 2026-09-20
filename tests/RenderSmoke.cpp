#include "TestAccess.h"
#include "core/include/GameManager.h"
#include "ui/include/MainMenu.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>

struct GameManagerTestAccess {
    static GameSession& session(GameManager& manager) { return manager._session; }
};

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: breakout_render_test <font> <screenshot-directory>\n";
        return 1;
    }
    try {
        sf::Font font;
        if (!font.openFromFile(argv[1])) throw std::runtime_error("Font failed to load");
        const std::filesystem::path output(argv[2]);
        std::filesystem::create_directories(output);
        sf::Vector2u renderSize{SCREEN_WIDTH, SCREEN_HEIGHT};
        const auto maximumSize = sf::Texture::getMaximumSize();
        if (maximumSize == 0) throw std::runtime_error("No usable OpenGL texture support");
        while (renderSize.x > maximumSize || renderSize.y > maximumSize) {
            renderSize.x /= 2;
            renderSize.y /= 2;
        }
        sf::RenderTexture texture(renderSize);
        texture.setView(sf::View(sf::FloatRect({0.f, 0.f}, {SCREEN_WIDTH, SCREEN_HEIGHT})));
        auto save = [&](const std::string& name) {
            texture.display();
            const auto image = texture.getTexture().copyToImage();
            const sf::Vector2u wallPixel{260 * renderSize.x / SCREEN_WIDTH, 450 * renderSize.y / SCREEN_HEIGHT};
            if (name != "menu" && image.getPixel(wallPixel) != sf::Color::White)
                throw std::runtime_error("Missing arena wall: " + name);
            if (!image.saveToFile(output / (name + ".png")))
                throw std::runtime_error("Screenshot failed: " + name);
            std::cout << "Rendered " << name << '\n';
        };
        MainMenu menu(font);
        texture.clear();
        menu.draw(texture);
        save("menu");

        GameManager game(font);
        auto& session = GameManagerTestAccess::session(game);
        for (std::size_t i = 0; i < campaignLevels().size(); ++i) {
            GameSessionTestAccess::level(session, i);
            texture.clear();
            game.draw(texture);
            save("level-" + std::to_string(i + 1));
        }
        session.advance();
        session.update(0.1f, 1.f);
        texture.clear();
        game.draw(texture);
        save("playing");
        for (const auto& [state, name] : {
            std::pair{PlayState::Paused, "paused"},
            std::pair{PlayState::LevelComplete, "level-complete"},
            std::pair{PlayState::GameOver, "game-over"},
            std::pair{PlayState::Won, "victory"}}) {
            GameSessionTestAccess::state(session, state);
            if (state == PlayState::LevelComplete || state == PlayState::Won) {
                for (auto& brick : GameSessionTestAccess::bricks(session))
                    while (!brick.isDestroyed()) brick.hit();
                GameSessionTestAccess::score(session, 5000);
            }
            if (state == PlayState::GameOver) GameSessionTestAccess::lives(session, 0);
            texture.clear();
            game.draw(texture);
            save(name);
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
