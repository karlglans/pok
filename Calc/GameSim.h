#pragma once
#include "IGameSim.h"
class GameSim :
	public IGameSim
{
private:
	int numberOfPlayers = 10;

public:
	virtual void setNumberOfPlayers(int);
	virtual void setStartHand(Card*);
	virtual void setOpenCards(Card*, int nCards);
	//virtual void setNumberOfPlayers(int) = 0;
	virtual void setNumberOfFolds(int);
	virtual void setPhase(gamePhase);
	virtual float getWinChance();
	GameSim();
	~GameSim();
};

