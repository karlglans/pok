#pragma once
#include "CommonTypes.h"

class StartHandLogger
{
	int* pStartHandsCount; // numb_starthands * 9
	int* pWinningPairCount; // numb_starthands * 9
public:
	int getStartHandCount(int nPlayers, int startHandValue);
	int getWinningPairCount(int nPlayers, int startHandValue);
	int addCardPair(Card* pCards, int nPlayers, bool winningPair);

	static const int numb_starthands = 169;
	int makeCardPairValue(const Card* pCards);
	void getBuffers(int** winns, int** count);
	
	StartHandLogger();
	~StartHandLogger();
};



