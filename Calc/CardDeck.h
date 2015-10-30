#pragma once
#include <vector>
#include "CommonTypes.h"
class CardDeck
{
	char _taken[52];
	bool _keep[5 + 2 * 10]; // matches _cards
	void addToTaken(Card* card);
	int _prevNumbTableCards; // so same cards can be filterd away
	void resetTaken();
	void resetBeforeDeal();
public:
	Card _cards[5 + 2 * 10];
	CardDeck();
	void SetPlayerCards(Card* pair);
	void SetTableCards(Card* cards, int nCards);
	// mainly for testing
	void setCardsForSeat(Card* pair, int seatIdx);
	Card* getCardsForSeat(int);
	int countAddedCards();
	void deal(int nPlayers);
};

