#include <cmath>
#include <iostream>
#include "AI.h"

int AI::staticEval() {
	const int largeFieldWeight = 10;
	const int smallFieldWeight = 0;

	int score = 0;
	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			if (largeState[x][y] == curPlayer) {
				score += largeFieldWeight;
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

	if (depth == 0)
		return { static_cast<float>(staticEval()) };
	Move bestMove = Move();
	for (Move m: getPossibleMoves()) {
		movesLookedAt += 1;

		if (m.outerX < 0 || m.outerY < 0 || m.innerX < 0 || m.innerY < 0)
			std::cout << "Move: " << m.outerX << m.innerX << " | " << m.outerY << m.innerY << std::endl;

		// Save current state
		const Players _lS = largeState[m.outerX][m.outerY];
		const Players _w = winner;
		const Coord _nM = nextOuterGrid;

		// make the move
		smallState[m.outerX][m.outerY][m.innerX][m.innerY] = curPlayer;
		largeState[m.outerX][m.outerY] = GameLogic::checkBox(smallState[m.outerX][m.outerY], m.innerX, m.innerY);
		winner = GameLogic::checkWin(largeState, m.outerX, m.outerY);
		nextOuterGrid.set(m.innerX, m.innerY);
		curPlayer = (curPlayer == Players::blue) ? Players::red : Players::blue;

		float eval = (winner != Players::neutral) ? beta : -search(depth - 1, -alpha, -beta).eval;


		// unmake Move
		smallState[m.outerX][m.outerY][m.innerX][m.innerY] = Players::neutral;
		largeState[m.outerX][m.outerY] = _lS;
		nextOuterGrid = _nM;
		winner = _w;
		curPlayer = (curPlayer == Players::blue) ? Players::red : Players::blue;

		if (eval > alpha) {
			alpha = eval;
			bestMove = m;
			if (eval >= beta) {
				break;
			}
		}
	}
	return { bestMove, alpha };
}

std::vector<Move> AI::getPossibleMoves() {

	std::vector<Move> moves = {};

	if (nextOuterGrid.x != -1 && nextOuterGrid.y != -1) {
		for (int x = 0; x < 3; ++x) {
			for (int y = 0; y < 3; ++y) {
				if (smallState[nextOuterGrid.x][nextOuterGrid.y][x][y] == Players::neutral) {
					moves.emplace_back(nextOuterGrid.x, nextOuterGrid.y, x, y);
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
	movesLookedAt = 0;
	EvalResult r = search(AI_SEARCH_DEPTH);

	std::cout << "Looked at: " << movesLookedAt << " moves" << std::endl
	          << "Move: " << r.move.outerX * 3 + r.move.innerX << " | " << r.move.outerY * 3 + r.move.innerY
	          << " - Eval: " << r.eval << std::endl;

	if (r.eval == -std::numeric_limits<float>::infinity())
		return getPossibleMoves().front();

	return r.move;
}

