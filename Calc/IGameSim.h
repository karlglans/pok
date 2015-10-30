#pragma once

#include "CommonTypes.h"


struct GameSimDesc {
	float preFlopAvreageFold; // how menny ppl on average fold: [0,1]
	GameSimDesc() : preFlopAvreageFold(0.5f) {}
};

class IGameSim {
public:
	// game settins can be changed during a game
	virtual void addBet(int amount) = 0;
	virtual void setGameDesc(GameSimDesc*) = 0;
	virtual void setOpenCards(Card*, int nCards) = 0;
	virtual float getWinChance(int nLaps) = 0;

	// this will put game to preflop state
	virtual void begin(Card* pair, int players, int dealer, int smallblind, int bigblind) = 0;

	virtual ~IGameSim() {};
};