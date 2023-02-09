#ifndef MEGATICTACTOE_AI_H
#define MEGATICTACTOE_AI_H

#include <vector>
#include "GameLogic.h"

class GameNode {
public:
    GameNode() = default;

    GameNode(Players player, Players curPlayer, Players winner,
             int nextMoveX, int nextMoveY,
             std::array<std::array<Players, 3>, 3> large,
             std::array<std::array<std::array<std::array<Players, 3>, 3>, 3>, 3> small) :
            player(player), curPlayer(curPlayer), winner(winner),
            nextLargeMoveX(nextMoveX), nextLargeMoveY(nextMoveY),
            largeState(large), smallState(small) {};

    Players player = Players::neutral;
    Players curPlayer = Players::neutral;
    Players winner = Players::neutral;
    int nextLargeMoveX = -1, nextLargeMoveY = -1;

    std::array<std::array<Players, 3>, 3> largeState; // default all 0
    std::array<std::array<std::array<std::array<Players, 3>, 3>, 3>, 3> smallState; // default all 0

    std::vector<GameNode> possibleContinuations = {};
    int evaluation = -1;

    void generateTree(int depth, int maxDepth);
    int eval();
};

class AI {
public:
    AI() = default;

    AI(Players player, Players curPlayer, Players winner,
       int nextMoveX, int nextMoveY,
       std::array<std::array<Players, 3>, 3> large,
       std::array<std::array<std::array<std::array<Players, 3>, 3>, 3>, 3> small) :
            gameState(player, curPlayer, winner, nextMoveX, nextMoveY, large, small) {};

    GameNode gameState;
};


#endif //MEGATICTACTOE_AI_H
