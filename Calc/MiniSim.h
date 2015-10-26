#pragma once
#include "IGameSim.h"
#include "CommonTypes.h"
#include "HandEvaluator.h"
#include "CardDeck.h"

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
	int _numPlayers = 10;
public:
	void runForLogger(int nPlayers, const int laps, StartHandLogger *pShl);
	float runSim(CardDeck* cards, const int nPlayers, const int laps, MiniSimResults* out);
};

