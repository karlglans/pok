#include "CardDeck.h"


CardDeck::CardDeck()
{
	// reset
	resetTaken();
	_prevNumbTableCards = 0;
	const int max = 10 * 2 + 5;
	for (int c = 0; c < max; c++)
		_keep[c] = false;
}

void CardDeck::resetTaken(){
	for (int n = 0; n < 52; n++)
		_taken[n] = 0;
}

// dont add the same card twice
void CardDeck::addToTaken(Card* card) {
	int pos = card[0].color * 13;
	pos += card[0].value;
#ifdef _DEBUG
	if (pos >= 52)
		throw std::invalid_argument("Invalid Card value");
	if (_taken[pos] == 1)
		throw std::invalid_argument("already added");
#endif
	_taken[pos] = 1;
}

void CardDeck::SetPlayerCards(Card* pair)
{
	addToTaken(&pair[0]);
	addToTaken(&pair[1]);
	_cards[5] = pair[0];
	_cards[6] = pair[1];
	_keep[5] = _keep[6] = true;
}

void CardDeck::setCardsForSeat(Card* pair, int seatIdx)
{
	int pos = seatIdx * 2 + 5;
	addToTaken(&pair[0]);
	addToTaken(&pair[1]);
	_cards[pos] = pair[0];
	_cards[pos + 1] = pair[1];
	_keep[pos] = _keep[pos + 1] = true;
}

void CardDeck::SetTableCards(Card* cards, int nCards)
{
	for (int c = _prevNumbTableCards; c < nCards; c++){
		addToTaken(&cards[c]);
		_cards[c] = cards[c];
		_keep[c] = true;
	}
	_prevNumbTableCards = nCards;
}

int CardDeck::countAddedCards(){
	int c = 0, nCardsInGame = 10 * 2 + 5; // maybe less
	for (int i = 0; i < nCardsInGame; i++)
		if (_keep[i]) 
			c++;
	return c;
}

// make sure the cards on the table and from players is added to taken again
void CardDeck::resetBeforeDeal(){
	resetTaken();

	// make count keept cards
	int nCardsInGame = 10 * 2 + 5; // maybe less 
	for (int i = 0; i < nCardsInGame; i++) {
		if (_keep[i])
			addToTaken(&_cards[i]);
	}
}

// player is seat 0
Card* CardDeck::getCardsForSeat(int seatIdx){
	return &_cards[5 + seatIdx * 2];
}

void CardDeck::deal(int nPlayers) {
	resetBeforeDeal();
	int pos;
	int nCardsInGame = nPlayers * 2 + 5;
	for (int k = 0; k < nCardsInGame; k++){
		if (_keep[k]) 
			continue;
		while (true){
			pos = rand() % 52;
			if (_taken[pos] == 0) {
				_taken[pos] = 1;
				_cards[k].color = pos / 13;
				_cards[k].value = pos % 13;
				break;
			}
		}
	}

}

