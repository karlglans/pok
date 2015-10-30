#pragma once

#include "CommonTypes.h"

struct Seat{
	int betted;
	int raised;
	bool active;
	int nActions;
	gameAction action[3];
	int setAction(gameAction, int bet, int raise);
};

struct SeatStatusReport{
	int currSeatIdx; // player is 0
	int betMissing; // how much this seat has to pay
	gamePhase phase;
	int potSize;
	int curBet;
	bool canRaise;
};

struct SeatStatus{
	bool stillActive;
};

class SeatActionHistory
{
	Seat _seats[10];
	int _seatsFromStart;
	int _dealer;
	int _currSeat;
	int _currBet;
	int _currMaxNbActions; // the seat who has done most, how menny actions, max 2
	int _sumBets;
	int _whoRaised;
	int _firstCaller;
	gamePhase _currPhase;
	void newRound();
public:
	SeatActionHistory();

	int findNextActiveSeat(int seatIdx);

	// reset after each game
	void newGame(int nSeats, int dealer, int smallBlind, int bigBlind);

	gamePhase getGamePhase();

	// maybe remove or package differently
	int playersFromStart();

	// players who still not have folded
	int countActiveSeats();

	// players is pos 0
	int getActiveSeatPos();

	// status for current seat
	void getStatus(SeatStatusReport* report);

	// true if folded
	void getFoldStatus(bool*);

	void getSeatStatus(SeatStatus*, int seatIdx);

	// 0: fold, same previus means call
	void insertBet(int amount); 
};

