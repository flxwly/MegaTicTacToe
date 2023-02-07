#ifndef MEGATICTACTOE_GAMEWINDOW_H
#define MEGATICTACTOE_GAMEWINDOW_H

#include <SFML/Graphics.hpp>

class GameWindow : public sf::RenderWindow {

public:

    bool curPlayer = true;
    std::array<std::array<int, 3>, 3> largeState; // default all 0
    std::array<std::array<std::array<std::array<int, 3>, 3>, 3>, 3> smallState; // default all 0

    sf::Vector2i nextLargeMove;

    int checkLargeState(int outerX, int outerY, int innerX, int innerY);
    int checkWin(int outerX, int outerY);
    int checkBox(std::array<std::array<int, 3>, 3> box, int x, int y);

    GameWindow() = default;
    GameWindow(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());

    void update();

    int frames = 0;
    void render();
};


#endif //MEGATICTACTOE_GAMEWINDOW_H
