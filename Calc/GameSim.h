#pragma once
#include "IGameSim.h"
#include "SeatActionHistory.h"
#include "Precalculated.h"
#include "CardDeck.h"

// functor
struct RunPreFlopPhase {
	RunPreFlopPhase(Precalculated* p, Card* cards, int nSeats) 
		: _preCalc(p), _cards(cards), _nSeats(nSeats){}
	void operator()(SeatActionHistory* seatHist);
private:
	Precalculated* _preCalc;
	Card* _cards;
	int _nSeats;
};

class GameSim :
	public IGameSim
{
private:
	int _nPlayers;
	int _dealerSeat; // player is seat0
	int _pot;
	gamePhase _gamePhase;
	GameSimDesc _simDesc;
	SeatActionHistory _seatHist;
	CardDeck _deck; // keeps all cards added by, setOpenCards(), begin()
	Precalculated* _pPreCalc;
	int runSomeGames(int nLaps);
public:
	void runPreflopPhase(SeatActionHistory* seatHist, Precalculated* _pPreCalc, CardDeck* deck); 
	virtual void addBet(int amount);
	virtual float getWinChance(int nLaps);
	virtual void setGameDesc(GameSimDesc*);
	virtual void setOpenCards(Card*, int nCards);
	virtual void begin(Card* pair, int players, int dealer, int smallblind, int bigblind);

	GameSim(Precalculated* preCalc);
	~GameSim();
};

