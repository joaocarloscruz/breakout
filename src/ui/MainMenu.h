#ifndef UI_MAIN_MENU
#define UI_MAIN_MENU

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

class MainMenu {
public:
    MainMenu(sf::RenderWindow &w);
    void start();

private:
    void setup_title();
    void setup_exit_button();
    void handle_mouse_click(sf::Vector2f mousePos);
    
    sf::RenderWindow &_window;
    sf::Font _font;
    std::optional<sf::Text> _title;
    std::optional<sf::Text> _exit_button;
    bool _is_exit_requested = false;
};

#endif // UI_MAIN_MENU