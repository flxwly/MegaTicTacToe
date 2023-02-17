#ifndef MEGATICTACTOE_AI_H
#define MEGATICTACTOE_AI_H

#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include "GameLogic.h"

#define AI_SEARCH_DEPTH 7

struct Move {
	Move() = default;
    Move(int outerX, int outerY,
		int innerX, int innerY) :
		innerX(innerX), innerY(innerY),
		outerX(outerX), outerY(outerY) {};

    int innerX, innerY;
    int outerX, outerY;
};

struct Coord {
	Coord(int x, int y) : x(x), y(y) {};
	void set(int x, int y) {this->x = x; this->y = y;};

	int x, y;
};

struct EvalResult {
	EvalResult(float eval) : move(), eval(eval) {};
	EvalResult(Move move, float eval) : move(move), eval(eval) {};

	Move move;
	float eval;
};


class AI {
public:
    AI() = default;

    AI(Players curPlayer, SimpleGrid large, LargeGrid small) :
	   curPlayer(curPlayer), largeState(large), smallState(small) {};

    Players curPlayer = Players::neutral;
    Players winner = Players::neutral;
	Coord nextOuterGrid = Coord(-1, -1);

    SimpleGrid largeState{}; // default all 0
    LargeGrid smallState{}; // default all 0

    void update(Players curPlayer, int nextMoveX, int nextMoveY,
                SimpleGrid large, LargeGrid small);

    float staticEval();
	EvalResult search(int depth,
		float alpha = -std::numeric_limits<float>::infinity(),
		float beta = std::numeric_limits<float>::infinity());

	int movesLookedAt = 0;
    int transpositionsUsed = 0;
    int transpositionsAdded = 0;
    int transpositionsOverridden = 0;
	Move findBestMove();

    std::vector<Move> getPossibleMoves();
    void orderMoves();

    template<typename T>
    void rotateMatrix(std::array<std::array<T, 3>, 3> &matrix);
    std::string str();

    std::vector<std::string> getTranspositions();
    static std::map<std::string, std::pair<float, int>> transpositions;
};


#endif //MEGATICTACTOE_AI_H
