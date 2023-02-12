#include "GameLogic.h"

#include <iostream>
#include "GameWindow.h"


Players GameLogic::checkBox(std::array<std::array<Players, 3>, 3> box, int x, int y) {
    Players player = box[x][y];

    // check row
    bool result = true;
    for (int x = 0; x < 3; ++x) {
        if (box[x][y] != player) {
            result = false;
            break;
        }
    }
    if (result)
        return player;

    // check column
    result = true;
    for (int y = 0; y < 3; ++y) {
        if (box[x][y] != player) {
            result = false;
            break;
        }
    }
    if (result)
        return player;

    // check diagonal
    switch (y) {
        case 0:
            switch (x) {
                case 0:
                    return (box[1][1] != player || box[2][2] != player) ? Players::neutral : player;
                case 2:
                    return (box[1][1] != player || box[0][2] != player) ? Players::neutral : player;
                default:
                    return Players::neutral;
            }
        case 1:
            switch (x) {
                case 0:
                    return Players::neutral;
                case 1:
                    return ((box[0][0] == player && box[2][2] == player) ||
                            (box[2][0] == player && box[0][2] == player)) ? player : Players::neutral;
                default:
                    return Players::neutral;
            }
        case 2:
            switch (x) {
                case 0:
                    return (box[1][1] != player || box[2][0] != player) ? Players::neutral : player;
                case 2:
                    return (box[1][1] != player || box[0][0] != player) ? Players::neutral : player;
                default:
                    return Players::neutral;
            }
        default:
            return Players::neutral;
    }
}


Players GameLogic::checkWin(const SimpleGrid &largeState, int outerX, int outerY) {
    return checkBox(largeState, outerX, outerY);
}

Players GameLogic::checkLargeState(const SimpleGrid &largeState, const LargeGrid &smallState, int outerX, int outerY,
                                   int innerX, int innerY) {
    // checkLargeState
    if (largeState[outerX][outerY] != Players::neutral)
        return largeState[outerX][outerY];

    return checkBox(smallState[outerX][outerY], innerX, innerY);
}

void GameLogic::update(int outerX, int outerY, int innerX, int innerY) {

    if (winner != Players::neutral) {
        init();
        return;
    }


    if (outerX < 0 || outerY < 0 || outerX > 3 || outerY > 3)
        return;

    if (innerX < 0 || innerY < 0 || innerX > 3 || innerY > 3)
        return;

    if (smallState[innerX][innerY][innerX][innerY] != Players::neutral)
        return;

    if ((nextLargeMoveX != -1 && nextLargeMoveY != -1) && (nextLargeMoveX != outerX || nextLargeMoveY != outerY))
        return;

    smallState[outerX][outerY][innerX][innerY] = curPlayer;
    largeState[outerX][outerY] = checkLargeState(largeState, smallState, outerX, outerY, innerX, innerY);

    nextLargeMoveX = innerX;
    nextLargeMoveY = innerY;

    curPlayer = (curPlayer == Players::red) ? Players::blue : Players::red;

    if (Players w = checkWin(largeState, outerX, outerY); w != Players::neutral) {
        winner = w;
    } else if (!canMakeAMove(smallState[nextLargeMoveX][nextLargeMoveY])) {
        nextLargeMoveX = -1;
        nextLargeMoveY = -1;
    }
}

void GameLogic::init() {
    curPlayer = Players::blue;
    winner = Players::neutral;
    nextLargeMoveX = -1;
    nextLargeMoveY = -1;
    for (int largeX = 0; largeX < 3; ++largeX) {
        for (int largeY = 0; largeY < 3; ++largeY) {
            largeState[largeX][largeY] = Players::neutral;
            for (int smallX = 0; smallX < 3; ++smallX) {
                for (int smallY = 0; smallY < 3; ++smallY) {
                    smallState[largeX][largeY][smallX][smallY] = Players::neutral;
                }
            }

        }
    }
}

bool GameLogic::canMakeAMove(SimpleGrid box) {
    for (auto r: box) {
        for (auto e: r) {
            if (e == Players::neutral)
                return true;
        }
    }
    return false;
}



