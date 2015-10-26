#pragma once
#include <vector>
#include "CommonTypes.h"
class CardDeck
{
	char _taken[52];
	void addToTaken(Card* card);
	std::vector<int> _added;
	int _prevNumbTableCards; // so same cards can be filterd away
	int _prevNumbPlayerCards;
	void resetTaken();
	void resetBeforeDeal();
public:
	Card _cards[5 + 2 * 10];
	CardDeck();
	void SetPlayerCards(Card* pair);
	void SetTableCards(Card* cards, int nCards);
	int countAddedCards();
	void deal(int nPlayers);
};

