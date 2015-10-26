#include "CardDeck.h"


CardDeck::CardDeck()
{
	// reset
	resetTaken();
	_prevNumbTableCards = 0;
	_prevNumbPlayerCards = 0;
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
	_added.push_back(pos);
	_taken[pos] = 1;
}

void CardDeck::SetPlayerCards(Card* pair)
{
	addToTaken(&pair[0]);
	addToTaken(&pair[1]);
	_cards[5] = pair[0];
	_cards[6] = pair[1];
	_prevNumbPlayerCards = 2;
}

void CardDeck::SetTableCards(Card* cards, int nCards)
{
	for (int c = _prevNumbTableCards; c < nCards; c++){
		addToTaken(&cards[c]);
		_cards[c] = cards[c];
	}
	_prevNumbTableCards = nCards;
}

int CardDeck::countAddedCards(){
	return _added.size();
}

// make sure the cards on the table and from players is added to taken
void CardDeck::resetBeforeDeal(){
	resetTaken();
	int dealtCards = _prevNumbTableCards + _prevNumbPlayerCards;
	for (int i = 0; i < dealtCards; i++){
		_taken[_added[i] ] = 1;
	}
}

void CardDeck::deal(int nPlayers) {
	resetBeforeDeal();
	int pos;

	// deal the missing cards on the table
	for (int n = _prevNumbTableCards; n < 5; n++){
		while (true){
			pos = rand() % 52;
			if (_taken[pos] == 0) {
				_taken[pos] = 1;
				_cards[n].color = pos / 13;
				_cards[n].value = pos % 13;
				break;
			}
		}
	}
	Card* oppCards = &_cards[5 + _prevNumbPlayerCards]; // skip cards on table and cards for player
	int nCards = (nPlayers - 1) * 2;
	if (_prevNumbPlayerCards == 0) // player cards has not been added, lets roll those too
		nCards += 2;
	for (int n = 0; n < nCards; n++){
		while (true){
			pos = rand() % 52;
			if (_taken[pos] == 0) {
				_taken[pos] = 1;
				oppCards[n].color = pos / 13;
				oppCards[n].value = pos % 13;
				break;
			}
		}
	}
}
