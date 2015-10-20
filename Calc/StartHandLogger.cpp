#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <stdexcept>
#include "StartHandLogger.h"
#include "PairPos.h"


StartHandLogger::StartHandLogger() {
	int nStartpair = numb_starthands * 9;
	pStartHandsCount = new int[nStartpair];
	pWinningPairCount = new int[nStartpair];
	for (int n = 0; n < nStartpair; n++) {
		pStartHandsCount[n] = 0;
		pWinningPairCount[n] = 0;
	}
}
StartHandLogger::~StartHandLogger() {
	if (pStartHandsCount != 0) {
		delete[] pStartHandsCount;
	}
	if (pStartHandsCount != 0) {
		delete[] pWinningPairCount;
	}
}

void StartHandLogger::getBuffers(int** winns, int** count) {
	*winns = pWinningPairCount;
	*count = pStartHandsCount;
}



// the structure can be used as key to index between 169 fields
int StartHandLogger::makeCardPairValue(const Card* pCards) {
	int highest = pCards[0].value;
	int lowest = pCards[1].value;
	int b;
	if (lowest > highest) {
		b = lowest;
		lowest = highest;
		highest = b;
	}
#ifdef _DEBUG
	if (lowest == highest && pCards[0].color == pCards[1].color)
		throw std::invalid_argument("Suited pair can not be pairs");
#endif
	PairPos pp;
	return pp.set(highest, lowest, pCards[0].color == pCards[1].color);
}

int StartHandLogger::getStartHandCount(int nPlayers, int startHandValue) {
	int idx = (nPlayers - 2) * numb_starthands + startHandValue;
	return pStartHandsCount[idx];
}

int StartHandLogger::getWinningPairCount(int nPlayers, int startHandValue) {
	int idx = (nPlayers - 2) * numb_starthands + startHandValue;
	return pWinningPairCount[idx];
}

int StartHandLogger::addCardPair(Card* pCards, int nPlayers, bool winningPair) {
	int nOpponents = nPlayers - 2; // 2 players = multiplier 0
	int cardPairValue = makeCardPairValue(pCards);
	if (cardPairValue > 168)
		__debugbreak();

	int idx = nOpponents * numb_starthands + cardPairValue;
	pStartHandsCount[idx]++;
	if (winningPair)
		pWinningPairCount[idx]++;

	return idx;
}



