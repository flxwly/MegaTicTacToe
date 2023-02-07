//
// Created by flxwl on 07.02.2023.
//

#include <iostream>
#include "GameWindow.h"


int GameWindow::checkBox(std::array<std::array<int, 3>, 3> box, int x, int y) {
    int val = box[x][y];

    // check row
    bool result = true;
    for (int x = 0; x < 3; ++x) {
        if (box[x][y] != val) {
            result = false;
            break;
        }
    }
    if (result)
        return val;

    // check column
    result = true;
    for (int y = 0; y < 3; ++y) {
        if (box[x][y] != val) {
            result = false;
            break;
        }
    }
    if (result)
        return val;

    // check diagonal
    switch (y) {
        case 0:
            switch (x) {
                case 0:
                    return (box[1][1] != val || box[2][2] != val) ? 0 : val;
                case 2:
                    return (box[1][1] != val || box[2][0] != val) ? 0 : val;
                default:
                    return 0;
            }
        case 1:
            switch (x) {
                case 0:
                    return 0;
                case 1:
                    return ((box[0][0] == val && box[2][2] == val) ||
                            (box[2][0] == val && box[0][2] == val)) ? val : 0;
                default:
                    return 0;
            }
        case 2:
            switch (x) {
                case 0:
                    return (box[1][1] != val || box[2][0] != val) ? 0 : val;
                case 2:
                    return (box[1][1] != val || box[0][0] != val) ? 0 : val;
                default:
                    return 0;
            }
        default:
            return 0;
    }
}
int GameWindow::checkWin(int outerX, int outerY) {
    return checkBox(largeState, outerX, outerY);
}
int GameWindow::checkLargeState(int outerX, int outerY, int innerX, int innerY) {
    // checkLargeState
    if (largeState[outerX][outerY] != 0)
        return largeState[outerX][outerY];

    return checkBox(smallState[outerX][outerY], innerX, innerY);
}

void GameWindow::update() {
    const int size = static_cast<int> (std::min(getSize().x, getSize().y));
    const sf::Vector2f largeCellSize = sf::Vector2f(size / 3, size / 3);
    const sf::Vector2f smallCellSize = sf::Vector2f(largeCellSize.x / 3, largeCellSize.y / 3);

    sf::Vector2f topLeft = sf::Vector2f((getSize().x - size) / 2, (getSize().y - size) / 2);

    if (sf::Event event; pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {

                    sf::Vector2i largeField =
                            sf::Vector2i((event.mouseButton.x - topLeft.x) / largeCellSize.x,
                                         (event.mouseButton.y - topLeft.y) / largeCellSize.y);

                    sf::Vector2i smallField =
                            sf::Vector2i(static_cast<int> ((event.mouseButton.x - topLeft.x) / smallCellSize.x) % 3,
                                         static_cast<int>((event.mouseButton.y - topLeft.y) / smallCellSize.y) % 3);

                    if (smallField.x < 0 || smallField.y < 0 || smallField.x > 3 || smallField.y > 3)
                        break;

                    if (largeField.x < 0 || largeField.y < 0 || largeField.x > 3 || largeField.y > 3)
                        break;

                    if (smallState[largeField.x][largeField.y][smallField.x][smallField.y] != 0)
                        break;

                    if (nextLargeMove.x != -1 && nextLargeMove.y != -1 && nextLargeMove != largeField)
                        break;

                    smallState[largeField.x][largeField.y][smallField.x][smallField.y] = (curPlayer) ? 1 : 2;
                    largeState[largeField.x][largeField.y] = checkLargeState(largeField.x, largeField.y, smallField.x, smallField.y);

                    if (int winner = checkWin(largeField.x, largeField.y); winner != 0) {
                        std::cout << "Player " << winner << " hat gewonnen!";
                        close();
                        return;
                    }

                    nextLargeMove = smallField;

                    curPlayer = !curPlayer;
                }
        }
    }


}

void GameWindow::render() {
    clear(sf::Color::White);

    const int size = static_cast<int> (std::min(getSize().x, getSize().y));
    const sf::Vector2f largeCellSize = sf::Vector2f(size / 3, size / 3);
    const sf::Vector2f smallCellSize = sf::Vector2f(largeCellSize.x / 3, largeCellSize.y / 3);
    sf::Vector2f topLeft = sf::Vector2f((getSize().x - size) / 2, (getSize().y - size) / 2);

    // draw field
    sf::CircleShape circ;
    sf::RectangleShape rect;
    rect.setOutlineColor(sf::Color::Black);
    rect.setFillColor(sf::Color::Transparent);
    for (int largeX = 0; largeX < 3; ++largeX) {
        for (int largeY = 0; largeY < 3; ++largeY) {

            // draw outer rect
            rect.setSize(largeCellSize);
            rect.setPosition(topLeft.x + largeX * largeCellSize.x,
                             topLeft.y + largeY * largeCellSize.y);
            rect.setOutlineThickness(2);
            draw(rect);

            // draw player mark
            if (largeState[largeX][largeY] != 0) {
                circ.setRadius(largeCellSize.x / 2);
                circ.setOutlineThickness(0.1f);
                circ.setPosition(rect.getPosition());
                switch (largeState[largeX][largeY]) {
                    case 1:
                        circ.setFillColor(sf::Color(0, 0, 255, 50));
                        circ.setOutlineColor(sf::Color(0, 0, 255, 200));
                        break;
                    case 2:
                        circ.setFillColor(sf::Color(255, 0, 0, 50));
                        circ.setOutlineColor(sf::Color(255, 0, 0, 200));
                        break;
                    default:
                        break;
                }
                draw(circ);
            }


            // draw inner rectangles + player marks
            rect.setOutlineThickness(1);
            rect.setSize(smallCellSize);
            for (int smallX = 0; smallX < 3; ++smallX) {
                for (int smallY = 0; smallY < 3; ++smallY) {

                    // draw inner rect
                    rect.setPosition(topLeft.x + largeX * largeCellSize.x + smallX * smallCellSize.x,
                                     topLeft.y + largeY * largeCellSize.y + smallY * smallCellSize.y);
                    draw(rect);

                    if (smallState[largeX][largeY][smallX][smallY] != 0) {
                        circ.setRadius(smallCellSize.x / 2);
                        circ.setOutlineThickness(0.1f);
                        circ.setPosition(rect.getPosition());
                        switch (smallState[largeX][largeY][smallX][smallY]) {
                            case 1:
                                circ.setFillColor(sf::Color(0, 0, 255, 50));
                                circ.setOutlineColor(sf::Color(0, 0, 255, 200));
                                break;
                            case 2:
                                circ.setFillColor(sf::Color(255, 0, 0, 50));
                                circ.setOutlineColor(sf::Color(255, 0, 0, 200));
                                break;
                            default:
                                break;
                        }
                        draw(circ);
                    }
                }
            }

        }
    }


    // draw next Move
    if ( nextLargeMove.x != -1 && nextLargeMove.y != -1) {
        rect.setSize(largeCellSize);
        rect.setPosition(largeCellSize.x * nextLargeMove.x, largeCellSize.y * nextLargeMove.y);
        rect.setOutlineColor(sf::Color::Yellow);
        if ((frames / 10) % 2) {
            draw(rect);
        }
    }

    display();
    frames++;
}

GameWindow::GameWindow(sf::VideoMode mode, const sf::String &title, sf::Uint32 style,
                       const sf::ContextSettings &settings) : RenderWindow(mode, title, style, settings) {
    nextLargeMove = {-1, -1};
    for (int largeX = 0; largeX < 3; ++largeX) {
        for (int largeY = 0; largeY < 3; ++largeY) {
            largeState[largeX][largeY] = 0;
            for (int smallX = 0; smallX < 3; ++smallX) {
                for (int smallY = 0; smallY < 3; ++smallY) {
                    smallState[largeX][largeY][smallX][smallY] = 0;
                }
            }

        }
    }
}



