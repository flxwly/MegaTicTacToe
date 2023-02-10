#include <algorithm>
#include "AI.h"

void GameNode::generateTree(int depth, int maxDepth) {
    if (depth == maxDepth || winner != Players::neutral)
        return;

    if (nextLargeMoveX != -1 && nextLargeMoveY != -1) {
        for (int x = 0; x < 3; ++x) {
            for (int y = 0; y < 3; ++y) {
                if (smallState[nextLargeMoveX][nextLargeMoveY][x][y] == Players::neutral) {

                    auto newSmall = smallState;
                    newSmall[nextLargeMoveX][nextLargeMoveY][x][y] = curPlayer;
                    auto newLarge = largeState;
                    newLarge[nextLargeMoveX][nextLargeMoveY] = GameLogic::checkBox(
                            smallState[nextLargeMoveX][nextLargeMoveX], x, y);

                    possibleContinuations.emplace_back(player,
                                                       (curPlayer == Players::red) ? Players::blue : Players::red,
                                                       GameLogic::checkBox(newLarge, nextLargeMoveX, nextLargeMoveY),
                                                       x, y, newLarge, newSmall);
                    possibleContinuations.back().generateTree(depth + 1, maxDepth);
                }
            }
        }
    } else {
        for (int largeX = 0; largeX < 3; ++largeX) {
            for (int largeY = 0; largeY < 3; ++largeY) {
                for (int x = 0; x < 3; ++x) {
                    for (int y = 0; y < 3; ++y) {
                        if (smallState[largeX][largeY][x][y] == Players::neutral) {

                            auto newSmall = smallState;
                            newSmall[largeX][largeY][x][y] = curPlayer;
                            auto newLarge = largeState;
                            newLarge[largeX][largeY] = GameLogic::checkBox(smallState[largeX][largeY], x, y);

                            possibleContinuations.emplace_back(player, (curPlayer == Players::red) ? Players::blue
                                                                                                   : Players::red,
                                                               GameLogic::checkBox(newLarge, largeX, largeY),
                                                               x, y, newLarge, newSmall);
                            possibleContinuations.back().generateTree(depth + 1, maxDepth);
                        }
                    }
                }
            }
        }
    }

    GameNode best = possibleContinuations.at(0);
    if (curPlayer == Players::red) {
        for (int i = 1; i < possibleContinuations.size(); ++i) {
           if (possibleContinuations[i].eval() < best.eval()) {
                best = possibleContinuations[i];
            }
        }
    } else {
           if (possibleContinuations[i].eval() > best.eval()) {
                best = possibleContinuations[i];
            }
        }
    }
    
    possibleContinuations.clear();
    possibleContinuations.push_back(best);
}

int GameNode::eval() {
    if (evaluation != -1) {
        return evaluation;
    }


    if (possibleContinuations.empty()) {
        // Static eval: Max Number of moves - moves done if won
        // Max number of moves: 99

        if (winner == Players::blue) {
            evaluation = 100;
            return 100;
        } else if (winner == Players::red) {
            evaluation = -100;
            return -100;
        }

        int counter = 0;
        for (int largeX = 0; largeX < 3; ++largeX) {
            for (int largeY = 0; largeY < 3; ++largeY) {
                for (int x = 0; x < 3; ++x) {
                    for (int y = 0; y < 3; ++y) {
                        if (smallState[largeX][largeY][x][y] != Players::neutral) {
                            counter++;
                        }
                    }
                }
            }
        }
        if (player == Players::blue) {
            evaluation = 99 - counter;
        } else {
            evalution = -99 + counter;
        }
        return counter;
    } else {
        if (player == Player::blue) {
            evaluation = std::max_element(possibleContinuations.begin(), possibleContinuations.end(), [](GameNode &a, GameNode &b) {return a.eval() > b.eval(); })->eval();
        } else {
            evaluation = std::min_element(possibleContinuations.begin(), possibleContinuations.end(), [](GameNode &a, GameNode &b) {return a.eval() > b.eval(); })->eval();
        }
        return evaluation;
    }
}
