#include <iostream>
#include "GameWindow.h"
#include "AI.h"

int main() {

    GameWindow window = GameWindow(sf::VideoMode(800, 800), "MegaTicTacToe");
    window.setFramerateLimit(30);

    while (window.isOpen()) {
        window.update();
        window.render();
    }

    return 0;
}
