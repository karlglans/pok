#include <stdexcept>
#include "SeatActionHistory.h"

gamePhase inc[] = { gamePhase::flop, gamePhase::turn, gamePhase::river, gamePhase::over, gamePhase::preflop };

int Seat::setAction(gameAction a, int bet, int raise) {
#ifdef _DEBUG
	if (nActions > 2)
		throw std::invalid_argument("No more actions");
#endif
	action[nActions] = a;
	if (a == gameAction::blinds) { // this action will later be overWritten
		betted = bet; 
	}
	if (a == gameAction::fold) {
		active = false;
	}
	else if (a == gameAction::call) {
		betted = bet;
	}
	else if (a == gameAction::raise) {
		raised += raise;
		betted = bet;
	}
	if (a != gameAction::blinds) {
		nActions++;
	}

	return nActions;
}

SeatActionHistory::SeatActionHistory()
:_seatsFromStart(10), _dealer(0), _currSeat(0), _currBet(0), _currPhase(preflop)
{ 
}

void SeatActionHistory::newGame(int seats, int dealer, int smallBlind, int bigBlind)
{
	_currPhase = over;
	_seatsFromStart = seats;
	_dealer = dealer;
	for (int s = 0; s < _seatsFromStart; s++){
		_seats[s].active = true;
	}
	newRound();

	_currSeat = _dealer;
	_currSeat = findNextActiveSeat(_currSeat);

	// adding blinds
	_seats[_currSeat].setAction(gameAction::blinds, smallBlind, 0);
	_currSeat = findNextActiveSeat(_currSeat);
	_seats[_currSeat].setAction(gameAction::blinds, bigBlind, 0);
	_currSeat = findNextActiveSeat(_currSeat);

	_sumBets = smallBlind + bigBlind;
	_currBet = bigBlind;
}

int SeatActionHistory::countActiveSeats(){
	int c = 0;
	for (int s = 0; s < _seatsFromStart; s++){
		if (_seats[s].active)
			c++;
	}
	return c;
}

int SeatActionHistory::findNextActiveSeat(int seatIdx) {
#ifdef _DEBUG
	if (seatIdx >= _seatsFromStart)
		__debugbreak();
#endif
	int nextSeat = seatIdx;
	for (int s = 0; s < _seatsFromStart; s++) {
		nextSeat = (++nextSeat) % _seatsFromStart;
		if (nextSeat == seatIdx)
			return -1;
		if (_seats[nextSeat].active)
			return nextSeat;
	}
	throw std::invalid_argument("No seat found");
	return -1; // should not happen
}

// takes a bet from current seat
void SeatActionHistory::insertBet(int bet) {
	if (bet == 0 && _currBet > 0) {
		_currMaxNbActions = _seats[_currSeat].setAction(gameAction::fold, 0, 0);
	}
	else if (bet > _currBet) {
		_currMaxNbActions = _seats[_currSeat].setAction(gameAction::raise, bet, bet - _currBet);
		_currBet = bet;
		_whoRaised = _currSeat;
		_firstCaller = -2;
	}
	else if(bet > 0) { // note: will checking as a BB end up here?
		_currMaxNbActions = _seats[_currSeat].setAction(gameAction::call, _currBet, 0);
		if (_firstCaller == -2)
			_firstCaller = _currSeat;
	}
	else if (bet == _seats[_currSeat].betted) {
		_currMaxNbActions = _seats[_currSeat].setAction(gameAction::check, 0, 0);
		if (_firstCaller == -2)
			_firstCaller = _currSeat;
	}
	else {
		__debugbreak();
	}

	_sumBets += bet; // fel
	_currSeat = findNextActiveSeat(_currSeat); // currSeat is steped upp, so its gonna be next seat to act

	if (_currSeat == -1)
		newRound();
	else if (_whoRaised == _currSeat) // called a raiser
		newRound();
	else if (_firstCaller == _currSeat) // round-checking
		newRound();
}

void SeatActionHistory::newRound(){
	_currPhase = inc[_currPhase];
	_currBet = 0;
	_whoRaised = -1;
	_firstCaller = -2;
	for (int s = 0; s < _seatsFromStart; s++){
		_seats[s].betted = 0;
		_seats[s].raised = 0;
		_seats[s].action[0] = gameAction::notActed;
		_seats[s].nActions = 0;
	}

	_currSeat = findNextActiveSeat(_dealer);
}

gamePhase SeatActionHistory::getGamePhase() {
	return _currPhase;
}

int SeatActionHistory::getActiveSeatPos() {
	return _currSeat;
}

int SeatActionHistory::playersFromStart(){
	return _seatsFromStart;
}

void SeatActionHistory::getStatus(SeatStatusReport *report)
{
	report->currSeatIdx = _currSeat;
	report->betMissing = _currBet - _seats[_currSeat].betted;
	report->phase = _currPhase;
	report->potSize = _sumBets;
	report->curBet = _currBet;
	report->canRaise = (_seats[_currSeat].nActions <  2); // can only raise once, per round
}

void SeatActionHistory::getSeatStatus(SeatStatus* s, int seatIdx)
{
	s->stillActive = _seats[seatIdx].active;
}

void SeatActionHistory::getFoldStatus(bool* hasFolded) {
	for (int s = 0; s < _seatsFromStart; s++){
		if (_seats[s].active == false)
			hasFolded[s] = true;
		else
			hasFolded[s] = false;
	}
}


