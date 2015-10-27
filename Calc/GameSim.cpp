#include "GameSim.h"


GameSim::GameSim()
{
}

GameSim::~GameSim()
{
}

void GameSim::begin(Card* pair, int players, int dealerSeat, int smallb, int bigb) {
	_nPlayers = players;
	_gamePhase = gamePhase::preflop;
	_dealerSeat = dealerSeat;
	_pot = smallb + bigb;
	int sbSeat = (_dealerSeat + 1) % players;
	int bbSeat = (_dealerSeat + 2) % players;
	for (int p = 0; p < players; p++) {
		_seatStateReal[p] = SeatState::undefined;
		_seatStake[p] = 0;
	}
	_seatStake[sbSeat] = smallb;
	_seatStake[bbSeat] = bigb;
}

void GameSim::setOpenCards(Card*, int nCards){
}
void GameSim::setNumberOfFolds(int){
}
void GameSim::setPhase(gamePhase){
}

float GameSim::getWinChance(){
	return 2.0f;
}
