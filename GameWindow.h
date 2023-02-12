#ifndef MEGATICTACTOE_GAMEWINDOW_H
#define MEGATICTACTOE_GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include "GameLogic.h"
#include "AI.h"

class GameWindow : public sf::RenderWindow {

public:

    sf::Font gameFont;

    GameLogic game = GameLogic();
    AI ai = AI(game.curPlayer, game.largeState, game.smallState);

    GameWindow() = default;
    GameWindow(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());

    void update();
    void render();
    int frames = 0;
};


#endif //MEGATICTACTOE_GAMEWINDOW_H
