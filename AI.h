#ifndef MEGATICTACTOE_AI_H
#define MEGATICTACTOE_AI_H

#include <vector>
#include <map>
#include "GameLogic.h"

#define AI_SEARCH_DEPTH 81

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

    int staticEval();
	EvalResult search(int depth,
		float alpha = -std::numeric_limits<float>::infinity(),
		float beta = std::numeric_limits<float>::infinity());

	int movesLookedAt = 0;
	Move findBestMove();

    std::vector<Move> getPossibleMoves();
    void orderMoves();
};


#endif //MEGATICTACTOE_AI_H
