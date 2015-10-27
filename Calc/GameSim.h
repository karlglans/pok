#pragma once
#include "IGameSim.h"

class GameSim :
	public IGameSim
{
private:
	enum SeatState { undefined, folded, check, raise, reraise, call };
	int _nPlayers;
	int _dealerSeat;
	int _pot;
	gamePhase _gamePhase;
	GameSimDesc _simDesc;
	SeatState _seatStateReal[10]; // 0: player
	int _seatStake[10]; // how much each seat has contributed during this phase
public:
	virtual void setGameDesc(GameSimDesc*);
	virtual float getWinChance();
	virtual void setOpenCards(Card*, int nCards);
	virtual void setNumberOfFolds(int);
	virtual void setPhase(gamePhase);
	virtual void begin(Card* pair, int players, int dealer, int smallblind, int bigblind);

	GameSim();
	~GameSim();
};

