#ifndef MEGATICTACTOE_AI_H
#define MEGATICTACTOE_AI_H

#include <vector>
#include <map>
#include "GameLogic.h"

struct Move {
    Move(short innerX, short innerY, short outerX, short outerY) : innerX(innerX), innerY(innerY), outerX(outerX),
                                                                   outerY(outerY) {};

    short innerX;
    short innerY;
    short outerX;
    short outerY;
};


class AI {
public:
    AI() = default;

    AI(Players curPlayer, int nextMoveX, int nextMoveY,
       SimpleGrid large, LargeGrid small) : curPlayer(curPlayer),
            nextLargeMoveX(nextMoveX), nextLargeMoveY(nextMoveY),
            largeState(large), smallState(small) {};

    Players curPlayer = Players::neutral;
    Players winner = Players::neutral;
    int nextLargeMoveX = -1, nextLargeMoveY = -1;

    SimpleGrid largeState{}; // default all 0
    LargeGrid smallState{}; // default all 0

    void update(Players curPlayer, int nextMoveX, int nextMoveY,
                SimpleGrid large, LargeGrid small);

    int staticEval();
    int search(int depth, int alpha, int beta);
    Move getBestMove();

    void makeMove(Move move);

    std::vector<Move> getPossibleMoves();
    void orderMoves();
};


#endif //MEGATICTACTOE_AI_H
