
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include "PreHandExamination.h"
#include "MiniSim.h"
#include <stdexcept>


void Deck::deal(Card* cards, int keep, int more)
{
	int pos;
	for (int n = 0; n < keep; n++){
		pos = cards[n].color * 13;
		pos += cards[n].value;
#ifdef _DEBUG
		if (pos >= 52)
			throw std::invalid_argument("Invalid Card value");
#endif
		taken[pos] = 1;
	}
	Card* moreCards = &cards[keep];
	for (int n = 0; n < more; n++){
		while (true){
			pos = rand() % 52;
			if (taken[pos] == 0) {
				taken[pos] = 1;
				moreCards[n].color = pos / 13;
				moreCards[n].value = pos % 13;
				break;
			}
		}
	}
}

void Deck::reset() {
	// reset
	for (int n = 0; n < 52; n++)
		taken[n] = 0;
}

int Deck::countTaken() {
	int c = 0;
	for (int n = 0; n < 52; n++){
		if (taken[n] == 1)
			c++;
	}
	return c;
}

void showResult(char* str, int subWis, int nWins) {
	char buffer[100];
	sprintf_s(buffer, str, subWis, (100.0f *subWis) / (float)nWins);
	printf(buffer);
}

void MiniSimResults::print() {
	showResult("Total wins_______%7d\t%5.2f\n", nWins, nLaps);
	showResult("straFlush       :%7d\t%5.2f\n", nWinsByType[HandRating::Hand::straFlush], nWins);
	showResult("four            :%7d\t%5.2f\n", nWinsByType[HandRating::Hand::four], nWins);
	showResult("house           :%7d\t%5.2f\n", nWinsByType[HandRating::Hand::house], nWins);
	showResult("flush           :%7d\t%5.2f\n", nWinsByType[HandRating::Hand::flush], nWins);
	showResult("straight        :%7d\t%5.2f\n", nWinsByType[HandRating::Hand::straight], nWins);
	showResult("three           :%7d\t%5.2f\n", nWinsByType[HandRating::Hand::three], nWins);
	showResult("twoPair         :%7d\t%5.2f\n", nWinsByType[HandRating::Hand::twoPair], nWins);
	showResult("pair            :%7d\t%5.2f\n", nWinsByType[HandRating::Hand::pair], nWins);
	showResult("highcard        :%7d\t%5.2f\n", nWinsByType[HandRating::Hand::highcard], nWins);
}

// nOpponents, max 9, 
float MiniSim::runSim(int nPlayers, gamePhase phase, const int laps, MiniSimResults* result) {
	int keepCards = 0; // gamePhase::preflop
	if (phase == gamePhase::flop)
		keepCards = 3;
	else if (phase == gamePhase::turn)
		keepCards = 4;
	else if (phase == gamePhase::river)
		keepCards = 5;

#ifdef _DEBUG
	if (nPlayers > 10 || nPlayers < 2)
		__debugbreak();
#endif

	int newCardsForTable = 5 - keepCards;
	int newCardsForOtherPlayers = (nPlayers - 1)* 2;
	int nWins = 0;

	Card* pPlayerCards = &_cards[5]; // first 2 is the player
	HandRating bestHand;
	int debugFoundCards, cardsInThisGame = 5 + nPlayers * 2;

	for (int t = 0; t < laps; t++) {
		_deck.reset();
		_deck.deal(pPlayerCards, 2, newCardsForOtherPlayers); // keep
		_deck.deal(_cards, keepCards, newCardsForTable);
#ifdef _DEBUG
		debugFoundCards = _deck.countTaken();
		if (cardsInThisGame != debugFoundCards)
			__debugbreak();
#endif
		if (_handEval.won(nPlayers, _cards, &bestHand)) {
			nWins++;
			result->nWinsByType[bestHand.values[HandRating::bestHand]]++;
		}
	}
	result->nLaps = laps;
	result->nWins = nWins;
	return (float)nWins / (float)laps;
}
 
void MiniSim::runForLogger(int nPlayers, const int laps, StartHandLogger *pShl) {
#ifdef _DEBUG
	if (nPlayers > 10 || nPlayers < 2)
		__debugbreak();
#endif
	int newCardsForTable = 5 + (nPlayers)* 2;
	HandRating bestHand;
	int debugFoundCards, cardsInThisGame = 5 + nPlayers * 2;

	int c = 0;
	for (unsigned int t = 0; t < laps; t++) {
		if ( (t % 10000000) == 0){
			std::cout << " " << c++;
		}
		_deck.reset();
		_deck.deal(_cards, 0, newCardsForTable); // all cards 
#ifdef _DEBUG
		debugFoundCards = _deck.countTaken();
		if (cardsInThisGame != debugFoundCards)
			__debugbreak();
#endif
		_handEval.won(nPlayers, _cards, &bestHand, pShl);
	}
}
