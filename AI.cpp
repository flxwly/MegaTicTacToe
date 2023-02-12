#include <cmath>
#include "AI.h"

int AI::staticEval() {
    const int largeFieldWeight = 10;
    const int smallFieldWeight = 1;

    int score = 0;
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            switch (largeState[x][y]) {
                case Players::blue:
                    score += largeFieldWeight;
                    break;
                case Players::red:
                    score -= largeFieldWeight;
                default:
                    for (int _x = 0; _x < 3; ++_x) {
                        for (int _y = 0; _y < 3; ++_y) {
                            switch (smallState[x][y][_x][_y]) {
                                case Players::blue:
                                    score += smallFieldWeight;
                                    break;
                                case Players::red:
                                    score -= smallFieldWeight;
                                default:
                                    break;
                            }
                        }
                    }
                    break;
            }
        }
    }

    return score;
}

int AI::search(int depth, int alpha, int beta) {

    if (depth == 0)
        return staticEval();

    if (winner != Players::neutral) {
        return (winner == Players::blue) ? -INFINITY : INFINITY;
    }


    Players _curPlayer = curPlayer;
    int _nextLargeMoveX = nextLargeMoveX;
    int _nextLargeMoveY = nextLargeMoveY;
    SimpleGrid _largeState = largeState;
    LargeGrid _smallState = smallState;

    for (Move move: getPossibleMoves()) {

        makeMove(move);
        int eval = -search(depth - 1, alpha, beta);

        // unmake Move
        curPlayer = _curPlayer;
        nextLargeMoveX = _nextLargeMoveX;
        nextLargeMoveY = _nextLargeMoveY;
        largeState = _largeState;
        smallState = _smallState;

        if (eval >= beta) {
            return beta;
        }
        alpha = std::max(eval, alpha);
    }

    return alpha;

}

std::vector <Move> AI::getPossibleMoves() {

    std::vector <Move> moves = {};

    if (nextLargeMoveX != -1 && nextLargeMoveY != -1) {
        for (int x = 0; x < 3; ++x) {
            for (int y = 0; y < 3; ++y) {
                if (smallState[nextLargeMoveX][nextLargeMoveY][x][y] == Players::neutral) {
                    moves.emplace_back(nextLargeMoveX, nextLargeMoveY, x, y);
                }
            }
        }
        if (!moves.empty())
            return moves;
    }

    for (int outerX = 0; outerX < 3; ++outerX) {
        for (int outerY = 0; outerY < 3; ++outerY) {
            for (int innerX = 0; innerX < 3; ++innerX) {
                for (int innerY = 0; innerY < 3; ++innerY) {
                    if (smallState[nextLargeMoveX][nextLargeMoveY][innerX][innerY] == Players::neutral) {
                        moves.emplace_back(outerX, outerY, innerX, innerY);
                    }
                }
            }
        }
    }

    return moves;
}

void AI::makeMove(Move m) {

    smallState[m.outerX][m.outerY][m.innerX][m.innerY] = curPlayer;
    largeState[m.outerX][m.outerY] = GameLogic::checkBox(smallState[m.outerX][m.outerY], m.innerX, m.innerY);

    winner = GameLogic::checkWin(largeState, m.outerX, m.outerY);

    nextLargeMoveX = m.innerX;
    nextLargeMoveY = m.innerY;

    curPlayer = (curPlayer == Players::blue) ? Players::red : Players::blue;
}

Move AI::getBestMove() {
    Players _curPlayer = curPlayer;
    int _nextLargeMoveX = nextLargeMoveX;
    int _nextLargeMoveY = nextLargeMoveY;
    SimpleGrid _largeState = largeState;
    LargeGrid _smallState = smallState;

    int bestEval = -INFINITY;
    Move bestMove = Move(-1, -1, -1, -1);
    for (Move move: getPossibleMoves()) {

        makeMove(move);
        int eval = -search(3, -INFINITY, INFINITY);

        // unmake Move
        update(_curPlayer, _nextLargeMoveX, _nextLargeMoveY, _largeState, _smallState);

        if (eval >= INFINITY) {
            return move;
        } else if (eval > bestEval) {
            bestMove = move;
            bestEval = eval;
        }
    }

    return bestMove;
}

void AI::update(Players _curPlayer, int nextMoveX, int nextMoveY, SimpleGrid large, LargeGrid small) {
    this->curPlayer = _curPlayer;
    this->nextLargeMoveX = nextMoveX;
    this->nextLargeMoveY = nextMoveY;
    this->largeState = large;
    this->smallState = small;
}

