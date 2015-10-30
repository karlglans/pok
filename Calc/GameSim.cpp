#include "GameSim.h"
#include "PairPos.h"
#include "HandEvaluator.h"


GameSim::GameSim(Precalculated* p)
:_pPreCalc(p)
{
}

GameSim::~GameSim(){}

void GameSim::setGameDesc(GameSimDesc*){

}

void GameSim::begin(Card* pair, int players, int dealerSeat, int smallb, int bigb) {
	_nPlayers = players;
	_gamePhase = gamePhase::preflop;
	_dealerSeat = dealerSeat;
	_pot = smallb + bigb;
	_deck.SetPlayerCards(pair);
	_seatHist.newGame(players, dealerSeat, smallb, bigb);
//	int sbSeat = (_dealerSeat + 1) % players;
//	int bbSeat = (_dealerSeat + 2) % players;
//	for (int p = 0; p < players; p++) {
////		_seatStateReal[p] = SeatState::undefined;
//		_seatStake[p] = 0;
//	}
//	_seatStake[sbSeat] = smallb;
//	_seatStake[bbSeat] = bigb;
}

// folds seats with poor start pairs, result can be found in seatHist
void GameSim::runPreflopPhase(SeatActionHistory* seatHist, Precalculated* pPreCalc, CardDeck* deck)
{
	int nPlayers = seatHist->playersFromStart();
	deck->deal(nPlayers);
	float* above = pPreCalc->GetAboveArray(nPlayers);
	int currSeat, maxSteps = nPlayers * 2;
	SeatStatusReport report;  Card* pCards;  PairPos pp;

	for (int t = 0; t < maxSteps; t++){
		seatHist->getStatus(&report);
		currSeat = report.currSeatIdx;
		if (report.phase != gamePhase::preflop)
			break;
		if (currSeat == -1)
			break;
		if (currSeat == 0) { // player
			seatHist->insertBet(report.curBet); // check
		}
		else {
			pCards = deck->getCardsForSeat(currSeat);
			pp.set(pCards); // could be done somewhere else   bug: kk blir 77
			if (report.betMissing > 0) {
				if (above[pp.pos] < 0.5)					// blir lågt värde
					seatHist->insertBet(0); // fold
				else
					seatHist->insertBet(report.curBet); // check
			}
			else seatHist->insertBet(report.curBet); // call
		}
	}

	// debugging:
	//Card cardsS1[2], cardsS2[2], cardsS3[2], cardsS4[2];
	//pCards = deck->getCardsForSeat(1);
	//cardsS1[0] = pCards[0]; cardsS1[1] = pCards[1];
	//cardsS2[0] = pCards[2]; cardsS2[1] = pCards[3];
	//cardsS3[0] = pCards[4]; cardsS3[1] = pCards[5];
	//cardsS4[0] = pCards[6]; cardsS4[1] = pCards[7];
	//bool foldedSeats[10];
	//seatHist->getFoldStatus(foldedSeats);
}

void GameSim::addBet(int amount) 
{
	_seatHist.insertBet(amount);
}

void GameSim::setOpenCards(Card*, int nCards)
{
}

// maybe run this in its own thread
int GameSim::runSomeGames(int nLaps) {
	HandRating bestHand;
	HandEvaluator _handEval;
	int cWins = 0;
	bool foldedSeats[10];
	for (int l = 0; l < nLaps; l++){
		SeatActionHistory simGame = _seatHist; // copy
		CardDeck deck = _deck;
		if (simGame.getGamePhase() == gamePhase::preflop) {
			runPreflopPhase(&simGame, _pPreCalc, &deck);
		}
		simGame.getFoldStatus(foldedSeats);
		if (_handEval.won(_nPlayers, deck._cards, &bestHand, foldedSeats)) {
			cWins++;
			//result->nWinsByType[bestHand.values[HandRating::bestHand]]++;
		}
	}
	return cWins;
}

float GameSim::getWinChance(int nLaps){
	int cWins = 0;
	cWins += runSomeGames(nLaps);
	return cWins / (float)nLaps;
}

void RunPreFlopPhase::operator()(SeatActionHistory* seatHist)
{

}
