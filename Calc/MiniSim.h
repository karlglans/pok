#pragma once
#include "IGameSim.h"
#include "CommonTypes.h"
#include "HandEvaluator.h"

class Deck {
public:
	char taken[52];
	void deal(Card* cards, int keep, int more);
	int countTaken();
	void reset();
};

struct MiniSimResults {
	int nWinsByType[10];
	int nLaps;
	int nWins;
	MiniSimResults() {
		nWinsByType[0] = nWinsByType[1] = nWinsByType[2] = nWinsByType[3] = nWinsByType[4]
			= nWinsByType[5] = nWinsByType[6] = nWinsByType[7] = nWinsByType[8] = nWinsByType[9] = 0;
	}
	void print();
};

// small simulation made for rating hands.
// * no folding
class MiniSim {
	HandRating winner;
	HandEvaluator _handEval;
	Deck _deck;
	int _numPlayers = 10;
public:
	Card _cards[10 * 2 + 5]; // first 5 is table, then player, then others
	float runSim(int nOpponents, gamePhase phase, const int laps, MiniSimResults* out);
	void runForLogger(int nPlayers, const int laps, StartHandLogger *pShl);
};

