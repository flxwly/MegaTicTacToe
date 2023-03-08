#include <algorithm>
#include "AI.h"

std::map<std::string, std::pair<float, int>>AI::transpositions = {};

float AI::staticEval() {
    const int largeFieldWeight = 100;
    const int smallFieldWeight = 1;

    float score = 0;
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            if (largeState[x][y] == curPlayer) {
                score = +largeFieldWeight;
            } else if (largeState[x][y] != Players::neutral) {
                score -= largeFieldWeight;
            } else {
                for (int _x = 0; _x < 3; ++_x) {
                    for (int _y = 0; _y < 3; ++_y) {
                        if (smallState[x][y][_x][_y] == curPlayer) {
                            score += smallFieldWeight;
                        } else if (smallState[x][y][_x][_y] != Players::neutral) {
                            score -= smallFieldWeight;
                        }
                    }
                }
            }
        }
    }

    return score;
}

EvalResult AI::search(int depth, float alpha, float beta) {

    //std::cout << "depth: " << depth << " alpha: " << alpha << " beta: " << beta << std::endl;

    if (depth <= 0) {
        return EvalResult(staticEval());
    }

    Move bestMove = Move();
    for (Move m: getPossibleMoves()) {
        debugInfo.movesLookedAt += 1;

        // Save current state
        Players _lS = largeState[m.outerX][m.outerY];
        Players _w = winner;
        Coord _nM = nextOuterGrid;
        Players cp = curPlayer;

        // make the move
        smallState[m.outerX][m.outerY][m.innerX][m.innerY] = curPlayer;
        largeState[m.outerX][m.outerY] = GameLogic::checkBox(smallState[m.outerX][m.outerY], m.innerX, m.innerY);
        winner = GameLogic::checkWin(largeState, m.outerX, m.outerY);
        nextOuterGrid.set(m.innerX, m.innerY);
        curPlayer = (curPlayer == Players::blue) ? Players::red : Players::blue;
        // evaluate

        float eval = 0;
        if (winner != Players::neutral) {
            eval = beta;
        } else {
            std::_Rb_tree_iterator<std::pair<const std::string, std::pair<float, int>>> tp;
            for (const auto &e: getTranspositions()) {
                if (tp = transpositions.find(e); tp != transpositions.end()) {
                    break;
                }
            }

            if (tp != transpositions.end()) {
                if (tp->second.second >= depth) {
                    eval = tp->second.first;
                    debugInfo.transpositionsUsed += 1;
                } else {
                    eval = -search(depth - 1, alpha, beta).eval;
                    tp->second = {eval, depth};
                    debugInfo.transpositionsOverridden += 1;
                }
            } else {
                eval = -search(depth - 1, -alpha, -beta).eval;
                transpositions.insert({str(), {eval, depth}});
                debugInfo.transpositionsAdded += 1;
                //std::cout << "Added transposition calculation..." << std::endl;
            }
        }

        // unmake Move
        smallState[m.outerX][m.outerY][m.innerX][m.innerY] = Players::neutral;
        largeState[m.outerX][m.outerY] = _lS;
        nextOuterGrid = _nM;
        winner = _w;
        curPlayer = cp;

        if (eval > alpha) {
            alpha = eval;
            bestMove = m;
            if (eval >= beta) {
                break;
            }
        }
    }
    return {bestMove, alpha};
}

std::vector<Move> AI::getPossibleMoves() {

    std::vector<Move> moves = {};

    if (nextOuterGrid.x != -1 && nextOuterGrid.y != -1) {
        auto box = smallState[nextOuterGrid.x][nextOuterGrid.y];

        // middle
        if (box[1][1] == Players::neutral)
            moves.emplace_back(nextOuterGrid.x, nextOuterGrid.y, 1, 1);

        // corner points
        if (box[0][0] == Players::neutral)
            moves.emplace_back(nextOuterGrid.x, nextOuterGrid.y, 0, 0);
        if (box[2][0] == Players::neutral)
            moves.emplace_back(nextOuterGrid.x, nextOuterGrid.y, 2, 0);
        if (box[0][2] == Players::neutral)
            moves.emplace_back(nextOuterGrid.x, nextOuterGrid.y, 0, 2);
        if (box[2][2] == Players::neutral)
            moves.emplace_back(nextOuterGrid.x, nextOuterGrid.y, 2, 2);

        // side points
        if (box[1][0] == Players::neutral)
            moves.emplace_back(nextOuterGrid.x, nextOuterGrid.y, 1, 0);
        if (box[0][1] == Players::neutral)
            moves.emplace_back(nextOuterGrid.x, nextOuterGrid.y, 0, 1);
        if (box[2][1] == Players::neutral)
            moves.emplace_back(nextOuterGrid.x, nextOuterGrid.y, 2, 1);
        if (box[1][2] == Players::neutral)
            moves.emplace_back(nextOuterGrid.x, nextOuterGrid.y, 1, 2);

        // if no possible moves;
        if (!moves.empty())
            return moves;
    }

    for (int outerX = 0; outerX < 3; ++outerX) {
        for (int outerY = 0; outerY < 3; ++outerY) {
            for (int innerX = 0; innerX < 3; ++innerX) {
                for (int innerY = 0; innerY < 3; ++innerY) {
                    if (smallState[outerX][outerY][innerX][innerY] == Players::neutral) {
                        moves.emplace_back(outerX, outerY, innerX, innerY);
                    }
                }
            }
        }
    }

    return moves;
}

void AI::update(Players _curPlayer, int nextMoveX, int nextMoveY, SimpleGrid large, LargeGrid small) {
    this->curPlayer = _curPlayer;
    this->nextOuterGrid.x = nextMoveX;
    this->nextOuterGrid.y = nextMoveY;
    this->largeState = large;
    this->smallState = small;
}

Move AI::findBestMove() {

    debugInfo.reset();

    EvalResult r = search(AI_SEARCH_DEPTH);

    debugInfo.printInfo();
    std::cout << "Move: " << r.move.outerX << "," << r.move.innerX << " | " << r.move.outerY << "," << r.move.innerY
              << " - Eval: " << r.eval << std::endl;


    if (r.eval == -std::numeric_limits<float>::infinity()) {
        std::cout << "No valid move found!" << std::endl;
        return getPossibleMoves().front();
    }

    return r.move;
}


std::vector<std::string> AI::getTranspositions() {
    LargeGrid flippedSmallState;
    SimpleGrid flippedLargeState;

    auto rotateSmallState = [](LargeGrid &matrix) {
        for (int i = 0; i < 2; i++) {
            for (int j = i; j < 2 - i; j++) {
                SimpleGrid tmp1 = matrix[i][j];
                matrix[i][j] = matrix[j][2 - i];
                matrix[j][2 - i] = matrix[2 - i][2 - j];
                matrix[2 - i][2 - j] = matrix[2 - j][i];
                matrix[2 - j][i] = tmp1;
            }
        }
    };

    auto rotateLargeState = [](SimpleGrid &matrix) {
        for (int i = 0; i < 2; i++) {
            for (int j = i; j < 2 - i; j++) {
                Players tmp1 = matrix[i][j];
                matrix[i][j] = matrix[j][2 - i];
                matrix[j][2 - i] = matrix[2 - i][2 - j];
                matrix[2 - i][2 - j] = matrix[2 - j][i];
                matrix[2 - j][i] = tmp1;
            }
        }
    };

    // Generate flipped grids
    for (int outerX = 0; outerX < 3; ++outerX) {
        for (int outerY = 0; outerY < 3; ++outerY) {
            for (int innerX = 0; innerX < 3; ++innerX) {
                for (int innerY = 0; innerY < 3; ++innerY) {
                    flippedSmallState[outerX][outerY][innerX][innerY]
                            = smallState[2 - outerX][outerY][2 - innerX][innerY];
                }
            }
            flippedLargeState[outerX][outerY] = largeState[2 - outerX][outerY];
        }
    }

    // rotate grids
    std::vector<std::string> returnVector;
    for (int i = 0; i < 4; ++i) {
        for (int outerX = 0; outerX < 3; ++outerX) {
            for (int outerY = 0; outerY < 3; ++outerY) {
                rotateLargeState(flippedSmallState[outerX][outerY]);
                rotateLargeState(smallState[outerX][outerY]);
            }
        }
        rotateSmallState(smallState);
        rotateSmallState(flippedSmallState);
        rotateLargeState(largeState);
        rotateLargeState(flippedLargeState);

        std::string fRet, ret;
        for (int outerX = 0; outerX < 3; ++outerX) {
            for (int outerY = 0; outerY < 3; ++outerY) {
                for (int innerX = 0; innerX < 3; ++innerX) {
                    for (int innerY = 0; innerY < 3; ++innerY) {
                        ret += (char) smallState[outerX][outerY][innerX][innerY];
                        fRet += (char) flippedSmallState[outerX][outerY][innerX][innerY];
                    }
                }
                ret += (char) largeState[outerX][outerY];
                fRet += (char) flippedLargeState[outerX][outerY];
            }
        }
        ret += (char) curPlayer;
        fRet += (char) curPlayer;
        returnVector.push_back(ret);
        returnVector.push_back(fRet);
    }

    return returnVector;
}

std::string AI::str() {
    std::string ret;
    for (int outerX = 0; outerX < 3; ++outerX) {
        for (int outerY = 0; outerY < 3; ++outerY) {
            for (int innerX = 0; innerX < 3; ++innerX) {
                for (int innerY = 0; innerY < 3; ++innerY) {
                    ret += (char) smallState[outerX][outerY][innerX][innerY];
                }
            }
            ret += (char) largeState[outerX][outerY];
        }
    }
    ret += (char) curPlayer;
    return ret;
}

void AI::printBoard() {
    // print small state
    std::cout << "Board: "<< str() << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << " -------------------------------------" << std::endl;
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                std::cout << " | ";
                for (int l = 0; l < 3; ++l) {
                    std::cout << " " << (char) smallState[k][i][l][j] << " ";
                }
            }
            std::cout << " | " << std::endl;
        }
    }
    std::cout << " -------------------------------------" << std::endl;
    std::cout << std::endl;
}