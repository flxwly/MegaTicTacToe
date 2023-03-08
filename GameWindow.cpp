#include <iostream>
#include <future>
#include "GameWindow.h"

void GameWindow::update() {
    const int size = static_cast<int> (std::min(getSize().x, getSize().y));
    const sf::Vector2f largeCellSize = sf::Vector2f(size / 3, size / 3);
    const sf::Vector2f smallCellSize = sf::Vector2f(largeCellSize.x / 3, largeCellSize.y / 3);

    sf::Vector2f topLeft = sf::Vector2f((getSize().x - size) / 2, (getSize().y - size) / 2);
    std::future<Move> bestMove;
    bool findingBestMove = false;

    sf::Event event;
    while (pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                close();
                break;

            case sf::Event::Resized: {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                setView(sf::View(visibleArea));
            }
                break;

            case sf::Event::MouseButtonPressed:

                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (game.curPlayer == Players::blue && false) {
                        game.update((event.mouseButton.x - topLeft.x) / largeCellSize.x,
                                    (event.mouseButton.y - topLeft.y) / largeCellSize.y,
                                    static_cast<int> ((event.mouseButton.x - topLeft.x) / smallCellSize.x) % 3,
                                    static_cast<int>((event.mouseButton.y - topLeft.y) / smallCellSize.y) % 3);
                    } else {
//                        if (findingBestMove && bestMove.valid()) {
//                            Move move = bestMove.get();
//                            game.update(move.outerX, move.outerY, move.innerX, move.innerY);
//                        } else if(!findingBestMove) {
//                            bestMove = std::async(std::launch::async, AI::findBestMove, &ai);
//                            findingBestMove = true;
//                        }
                        Move move = ai.findBestMove();
                        game.update(move.outerX, move.outerY, move.innerX, move.innerY);
                    }
                    ai.update(game.curPlayer, game.nextLargeMoveX, game.nextLargeMoveY, game.largeState, game.smallState);
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
            rect.setOrigin(rect.getGlobalBounds().width / 2, rect.getGlobalBounds().height / 2);
            rect.setPosition(topLeft.x + largeX * largeCellSize.x + largeCellSize.x / 2,
                             topLeft.y + largeY * largeCellSize.y + largeCellSize.x / 2);
            rect.setOutlineThickness(2);
            draw(rect);

            // draw player mark
            if (game.largeState[largeX][largeY] != Players::neutral) {
                circ.setRadius(largeCellSize.x / 2 - 4);
                circ.setOrigin(circ.getGlobalBounds().width / 2, circ.getGlobalBounds().height / 2);
                circ.setOutlineThickness(1);
                circ.setPosition(rect.getPosition());
                switch (game.largeState[largeX][largeY]) {
                    case Players::blue:
                        circ.setFillColor(sf::Color(0, 0, 255, 50));
                        circ.setOutlineColor(sf::Color(0, 0, 255, 200));
                        break;
                    case Players::red:
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
            rect.setOrigin(rect.getGlobalBounds().width / 2, rect.getGlobalBounds().height / 2);
            for (int smallX = 0; smallX < 3; ++smallX) {
                for (int smallY = 0; smallY < 3; ++smallY) {

                    // draw inner rect
                    rect.setPosition(
                            topLeft.x + largeX * largeCellSize.x + smallX * smallCellSize.x + smallCellSize.x / 2,
                            topLeft.y + largeY * largeCellSize.y + smallY * smallCellSize.y + smallCellSize.x / 2);
                    draw(rect);

                    if (game.smallState[largeX][largeY][smallX][smallY] != Players::neutral) {
                        circ.setRadius(smallCellSize.x / 2 - 2);
                        circ.setOrigin(circ.getGlobalBounds().width / 2, circ.getGlobalBounds().height / 2);
                        circ.setOutlineThickness(2);
                        circ.setPosition(rect.getPosition());
                        switch (game.smallState[largeX][largeY][smallX][smallY]) {
                            case Players::blue:
                                circ.setFillColor(sf::Color(0, 0, 255, 50));
                                circ.setOutlineColor(sf::Color(0, 0, 255, 200));
                                break;
                            case Players::red:
                                circ.setFillColor(sf::Color(255, 0, 0, 50));
                                circ.setOutlineColor(sf::Color(255, 0, 0, 200));
                                break;
                            default:
                                circ.setFillColor(sf::Color(0, 255, 0, 50));
                                circ.setOutlineColor(sf::Color(0, 255, 0, 200));
                                break;
                        }
                        draw(circ);
                    }
                }
            }

        }
    }


    // draw next Move
    if (game.nextLargeMoveX != -1 && game.nextLargeMoveY != -1) {
        rect.setSize(largeCellSize);
        rect.setOrigin(rect.getGlobalBounds().width / 2, rect.getGlobalBounds().height / 2);
        rect.setPosition(topLeft.x + largeCellSize.x * game.nextLargeMoveX + largeCellSize.x / 2,
                         topLeft.y + largeCellSize.y * game.nextLargeMoveY + largeCellSize.y / 2);
        rect.setOutlineColor(sf::Color::Yellow);
        if ((frames / 10) % 2) {
            draw(rect);
        }
    }

    // Draw winner text
    if (game.winner != Players::neutral) {
        sf::Text text;
        text.setFont(gameFont);
        text.setCharacterSize(50);
        text.setString("Spieler " + std::to_string(game.winner) + " hat gewonnen!");
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2);
        if ((frames / 5) % 2) {
            text.setFillColor(sf::Color::Black);
        } else {
            if (game.winner == Players::blue)
                text.setFillColor(sf::Color::Blue);
            else
                text.setFillColor(sf::Color::Red);
        }
        text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
        text.setPosition(topLeft.x + size / 2, topLeft.y + size / 2);
        draw(text);
    }

    frames++;
    display();
}

GameWindow::GameWindow(sf::VideoMode
                       mode,
                       const sf::String &title, sf::Uint32
                       style,
                       const sf::ContextSettings &settings) : RenderWindow(mode, title, style, settings) {
    if (!gameFont.loadFromFile("fonts/arial.ttf")) {
        close();
    }
}



