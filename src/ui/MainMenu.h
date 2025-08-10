#ifndef UI_MAIN_MENU
#define UI_MAIN_MENU

#include <SFML/Graphics.hpp>
#include <iostream>
class MainMenu {
public:
  MainMenu(sf::RenderWindow &w);
  void start();

private:  
  sf::RenderWindow &_window;
  sf::Font _font;
  bool _is_exit_requested = false;
};

#endif // UI_MAIN_MENU

