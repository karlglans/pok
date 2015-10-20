#pragma once
#include "IGameSim.h"
#include "CommonTypes.h"
#include "PreHandExamination.h"
#include "StartHandLogger.h"


template<unsigned int N>
struct bin {
	enum { value = (N % 10) + 2 * bin<N / 10>::value };
};
template<>
struct bin<0> {
	enum { value = 0 };
};



class HandEvaluator {
	PreHandExamination _preExam[10];
	HandRating _hr; // maybe move
	const static unsigned int straights[];
public:
	PreHandExamination* preExamCards(Card*, int nPlayers);
	void addCardCount(Card* pCards, int nCards, PreHandExamination* result);
	void addColorBits(Card* pCards, int nCards, PreHandExamination* result);
	HandRating* findStraightFlush(PreHandExamination* pHand);
	HandRating* makeRatedFlushHand(int bitHand);
	HandRating* findHouse(PreHandExamination* pHand);
	bool won(int nPlayers, Card *cards, HandRating* bestHand); // StartHandLogger
	bool won(int nPlayers, Card *cards, HandRating* bestHand, StartHandLogger* log);
	HandEvaluator() {}
};

