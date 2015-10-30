#include "HandEvaluator.h"

// bitflags representing straights
const unsigned int HandEvaluator::straights[] = { 7936, 3968, 1984, 992, 496, 248, 124, 62, 31, 4111};

// cards:
//	2	3	4		5	6	7	8	9	10	kn	q		k		e
//	0	1	2		3	4	5	6	7	8	9	10		11		12		as value
//	1  10 100	 1000
//	1	2	4		8	16	32	64	128	256	512	1024	2048	4096	as bit flag


// 4096 + 2048 + 1024 + 512 + 256 = 7936    : top straight
// 2048 + 1024 + 512 + 256 + 128 =	3968    : top straight
// 1024 + 512 + 256 + 128 + 64 =	1984 
// 512 + 256 + 128 + 64 + 32 =		992 
// 256 + 128 + 64 + 32 + 16 =		496
// 32 + 16 + 8 + 4 + 2 =			62 
// 16 + 8 + 4 + 2 + 1 =				31	
// 8 + 4 + 2 + 1 + 4096 =			4111 : lowest, ace bottom

HandRating* HandEvaluator::findStraightFlush(PreHandExamination* pHand) {
	_hr.values[HandRating::bestHand] = HandRating::undefined; // found nothing yet

	HandRating::Hand bestHand = HandRating::undefined;
	bool flushFound = false;
	int flushColor, colorBitHand, straightBits, topCardValue = 0;

	for (int c = 0; c < 4; c++) {
		if (pHand->colorFreq[c] > 4) { // only one flush per 7 cards possible
			bestHand = HandRating::flush;
			flushFound = true;
			flushColor = c;
			colorBitHand = pHand->colorHand[flushColor];
			break;
		}
	}

	// loop thru all possible straight
	for (int s = 0; s < 10; s++) { // <-- looking for the most valuable straights first
		straightBits = straights[s];
		if (flushFound) {
			if ((colorBitHand & straightBits) == straightBits) {
				_hr.values[HandRating::bestHand] = HandRating::straFlush;
				_hr.values[HandRating::bestCard] = 100 - s; // might work,
				_hr.values[HandRating::bestCard + 1] = _hr.values[HandRating::bestCard + 2] =
					_hr.values[HandRating::bestCard + 3] = _hr.values[HandRating::bestCard + 4] = 0;
				return &_hr;
			}
		}
		else {
			if (bestHand == HandRating::undefined && (pHand->uniHand & straightBits) == straightBits) {
				_hr.values[HandRating::bestHand] = HandRating::straight;
				_hr.values[HandRating::bestCard] = 100 - s;
				_hr.values[HandRating::bestCard + 1] = _hr.values[HandRating::bestCard + 2] = 
					_hr.values[HandRating::bestCard + 3] = _hr.values[HandRating::bestCard + 4] = 0;
			}
		}
	}

	if (flushFound)
		return makeRatedFlushHand(colorBitHand);

	return &_hr;
}

HandRating* HandEvaluator::makeRatedFlushHand(int bitHand) {
	_hr.values[HandRating::bestHand] = HandRating::flush;
	int valueBit, n = 0;
	for (int v = 12; v >= 0; v--){
		valueBit = (1 << v);
		if ((bitHand & valueBit) == valueBit) {
			_hr.values[HandRating::bestCard + n] = valueBit;
			n++;
			if (n >= 5)
				break;
		}
	}
	return &_hr;
}

HandRating* HandEvaluator::findHouse(PreHandExamination* pHand) {
	_hr.values[HandRating::bestHand] = HandRating::undefined; // found nothing yet
	int val4 = -1, val3 = -1, val2 = -1, val22 = -1, freq;
	int top1 = -1, top2 = -1, top3 = -1, top4 = -1, top5 = -1;

	for (int c = 12; c >= 0; c--){
		freq = pHand->valueFreq[c];
		if (freq == 0)
			continue;
		else if (freq == 4 && val4 == -1) {
			val4 = c;
			// if a kicker was found:
			if (val3 > top1)
				top1 = val3;
			if (val2 > top1)
				top1 = val2;
		}
		else if (val4 != -1 && top1 == -1)
			top1 = c; // kicker for a Four of a hand
		else if (freq == 3 && val3 == -1)
			val3 = c;
		else if (freq > 1 && val2 == -1)
			val2 = c;
		else if (freq > 1 && val22 == -1)
			val22 = c;
		else if (top1 == -1 && freq > 0)
			top1 = c;
		else if (top2 == -1 && freq > 0)
			top2 = c;
		else if (top3 == -1 && freq > 0)
			top3 = c;
		else if (top4 == -1 && freq > 0)
			top4 = c;
		else if (top5 == -1 && freq > 0)
			top5 = c;
	}
	
	if (val4 != -1) {
		_hr.values[HandRating::bestHand] = HandRating::four;
		_hr.values[HandRating::bestCard] = val4;
		_hr.values[HandRating::bestCard + 1] = top1;
		_hr.values[HandRating::bestCard + 2] = _hr.values[HandRating::bestCard + 3] = 
			_hr.values[HandRating::bestCard + 4] = 0;
	}
	else if (val3 != -1 && val2 != -1 ) {
		_hr.values[HandRating::bestHand] = HandRating::house;
		_hr.values[HandRating::bestCard] = val3;
		_hr.values[HandRating::bestCard + 1] = val2;
		_hr.values[HandRating::bestCard + 2] = _hr.values[HandRating::bestCard + 3] =
			_hr.values[HandRating::bestCard + 4] = 0;
	}
	else if (val3 != -1) {
		_hr.values[HandRating::bestHand] = HandRating::three;
		_hr.values[HandRating::bestCard] = val3;
		_hr.values[HandRating::bestCard + 1] = top1;
		_hr.values[HandRating::bestCard + 2] = top2;
		_hr.values[HandRating::bestCard + 3] = _hr.values[HandRating::bestCard + 4] = 0;
	}
	else if (val22 != -1) {
		_hr.values[HandRating::bestHand] = HandRating::twoPair;
		_hr.values[HandRating::bestCard] = val2;
		_hr.values[HandRating::bestCard + 1] = val22;
		_hr.values[HandRating::bestCard + 2] = top1;
		_hr.values[HandRating::bestCard + 3] = _hr.values[HandRating::bestCard + 4] = 0;
	}
	else if (val2 != -1) {
		_hr.values[HandRating::bestHand] = HandRating::pair;
		_hr.values[HandRating::bestCard] = val2;
		_hr.values[HandRating::bestCard + 1] = top1;
		_hr.values[HandRating::bestCard + 2] = top2;
		_hr.values[HandRating::bestCard + 3] = top3;
		_hr.values[HandRating::bestCard + 4] = 0;
	}
	else { // top5 != -1 ?
		_hr.values[HandRating::bestHand] = HandRating::highcard;
		_hr.values[HandRating::bestCard] = top1;
		_hr.values[HandRating::bestCard + 1] = top2;
		_hr.values[HandRating::bestCard + 2] = top3;
		_hr.values[HandRating::bestCard + 3] = top4;
		_hr.values[HandRating::bestCard + 4] = top5;
	}

	return &_hr;
}

void HandEvaluator::addCardCount(Card* pCards, int nCards, PreHandExamination* result) {
	int c;
	int value, color;
	for (c = 0; c < nCards; c++){
		value = pCards[c].value;
		result[0].valueFreq[value]++;
		color = pCards[c].color;
		result[0].colorFreq[color]++;
	}
}

// add bits made from pCard to the result. 
void HandEvaluator::addColorBits(Card* pCards, int nCards, PreHandExamination* result) {
	unsigned int colors[] = { 0, 0, 0, 0 };
	unsigned int uni = 0;
	int c;
	int value, color, valueFlag;
	for (c = 0; c < nCards; c++){
		value = pCards[c].value;
		color = pCards[c].color;
		valueFlag = (1 << value);
		colors[color] += valueFlag;
	}
	for (c = 0; c < 4; c++){
		(*result).colorHand[c] = ((*result).colorHand[c] | colors[c]);
		uni = (uni | colors[c]);
	}
	(*result).uniHand = ((*result).uniHand | uni);
}

//expects 5 first cards to be common cards, then 2 per player
PreHandExamination* HandEvaluator::preExamCards(Card* pCards, int nPlayers) {
	PreHandExamination commonExam;
	commonExam.reset();
	addCardCount(pCards, 5, &commonExam); // count those common cards
	addColorBits(pCards, 5, &commonExam);
	Card* playerCards = &pCards[5];
	for (int p = 0; p < nPlayers; p++){
		_preExam[p] = commonExam; // copy from common to this player
		addColorBits(&playerCards[2 * p], 2, &_preExam[p]);
		addCardCount(&playerCards[2 * p], 2, &_preExam[p]);
	}
	return _preExam;
}

//expects 5 first cards to be common cards, then 2 per player
//returns true if fist player had the winning hand
bool HandEvaluator::won(int nPlayers, Card *cards, HandRating* bestHand) {
	return won(nPlayers, cards, bestHand, (bool*)0);
}

bool HandEvaluator::won(int nPlayers, Card *cards, HandRating* bestHand, bool* foldedSeats) {
	PreHandExamination* pExamData = preExamCards(cards, nPlayers);
	HandRating* hand = 0;
	HandRating playerHand;
	bestHand->reset();

	for (int pl = 0; pl < nPlayers; pl++) {
		if (foldedSeats != 0 && foldedSeats[pl])
			continue;
		hand = findStraightFlush(&pExamData[pl]);
		if (*bestHand < *hand) {
			*bestHand = *hand;
		}
		hand = findHouse(&pExamData[pl]);
		if (*bestHand < *hand){
			*bestHand = *hand;
		}
		if (pl == 0)
			playerHand = *bestHand; // first loop elap, should be true
	}
	return playerHand == *bestHand;
}

bool HandEvaluator::won(int nPlayers, Card *cards, HandRating* bestHand, StartHandLogger* pLogHands) {
	PreHandExamination* pExamData = preExamCards(cards, nPlayers);
	HandRating* hand = 0;
	HandRating playerHand, hands[10];
	bestHand->reset();
	Card* pStartPairs = &cards[5];
	for (int pl = 0; pl < nPlayers; pl++) {
		hand = findStraightFlush(&pExamData[pl]);
		if (*bestHand < *hand) {
			*bestHand = *hand;
		}
		hands[pl] = *hand;
		hand = findHouse(&pExamData[pl]);
		if (*bestHand < *hand){
			*bestHand = *hand;
		}
		if (hands[pl] < *hand)
			hands[pl] = *hand;

		if (pl == 0)
			playerHand = *bestHand; // first loop elap, should be true
	}
	// adding stats from opponents this game:
	bool vicory;
	for (int pl = 0; pl < nPlayers; pl++) {
		vicory = (hands[pl] == *bestHand);
		pLogHands->addCardPair(&pStartPairs[2 * pl], nPlayers, vicory);
	}
	return playerHand == *bestHand;
}
