#include <iostream>
#include "GameWindow.h"
#include "AI.h"

int main() {

    std::array<std::array<Players, 3>, 3> largeState{}; // default all 0
    std::array<std::array<std::array<std::array<Players, 3>, 3>, 3>, 3> smallState{}; // default all 0
    AI ai = AI(Players::blue, Players::blue, Players::neutral, -1, -1, largeState, smallState);

    //ai.gameState.generateTree(0, 6);
    std::cout << ai.gameState.eval() << std::endl;

    GameWindow window = GameWindow(sf::VideoMode(800, 800), "MegaTicTacToe");
    window.setFramerateLimit(30);

    while (window.isOpen()) {
        window.update();
        window.render();
    }

    return 0;
}
