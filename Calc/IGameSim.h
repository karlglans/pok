#pragma once

#include "CommonTypes.h"

// 
enum gamePhase {preflop, flop, turn, river};

class IGameSim {
public:
	// number of players since start
	virtual void setNumberOfPlayers(int) = 0;

	virtual void setStartHand(Card*) = 0;
	virtual void setOpenCards(Card*, int nCards) = 0;
	//virtual void setNumberOfPlayers(int) = 0;
	virtual void setNumberOfFolds(int) = 0;
	virtual void setPhase(gamePhase) = 0;
	virtual float getWinChance() = 0;
	virtual ~IGameSim() {};
};